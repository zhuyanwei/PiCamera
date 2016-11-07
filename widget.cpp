#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    this->setAttribute(Qt::WA_PaintOutsidePaintEvent);
    ui->setupUi(this);

    pp = (unsigned char *)malloc(WIDTH * HEIGHT/*QWidget::width()*QWidget::height()*/* 3 * sizeof(char));
    //painter = new QPainter(this);
    frame = new QImage(pp,WIDTH,HEIGHT,QImage::Format_RGB888);

    timer = new QTimer(this);

    au = new audioget();
    ae = new au_encode();
    ad = new au_decode();

    ae->encode_open();
    ad->decode_open();
    au->init_audio(&ae->auin_fifo,&ad->auout_fifo);

    connect(timer, SIGNAL(timeout()), this, SLOT(processaudio()));
    timer->start(10);

    isServer = 0 ;
    isStart = 0;
    isStart2 = 0;

}

Widget::~Widget()
{
    if(CT)
    {
    CT->stop();
    free(CT);
    }

    if(Vid)
    {
    Vid->close();
    free(Vid);
    }

    if(AT)
    {
     AT->stop();
     free(AT);
    }

    ae->encode_close();
    ad->decode_close();
    au->close_audio();

    session.BYEDestroy(RTPTime(10,0),0,0);
    delete ui;
}

void Widget::paintEvent(QPaintEvent *)
{
    if (isStart == 1)
    {
        convert_yuv_to_rgb_buffer((uchar *)CT->cap_buf,pp,WIDTH,HEIGHT/*QWidget::width(),QWidget::height()*/);
        frame->loadFromData((uchar *)pp,/*len*/WIDTH * HEIGHT* 3 * sizeof(char));
        ui->label->setPixmap(QPixmap::fromImage(*frame,Qt::AutoColor));
   }

}

int Widget::processaudio()
{
    if(isStart == 1)
    {qDebug()<<av_audio_fifo_size(ae->auin_fifo);
        while (av_audio_fifo_size(ae->auin_fifo)>=FRAMES_PER_BUFFER)
        {
        ae->encode_do(&ae_buf,&ae_size);
        qDebug()<<"send"<<ae_size;
        session.SendPacket(ae_buf,ae_size,AAC,false,FRAMES_PER_BUFFER);
        }
    }

        return 0;
}

bool Widget::CheckError(int rtperr)
{
    if (rtperr < 0)
    {
        std::cout<<"ERROR: "<<RTPGetErrorString(rtperr)<<std::endl;
        return false;
    }
    return true;
}

void Widget::add_dest(uint32_t dest_ip,uint16_t dest_port)
{
    dest_ip = ntohl(dest_ip);

    RTPIPv4Address addr(dest_ip,dest_port);
    status = session.AddDestination(addr);
    CheckError(status);
}

void Widget::sendMessage(MessageType type,char* destip)
{
    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);
    out<<type;
    switch(type)
    {
    case Request:
    {
        QString name = QHostInfo::localHostName();
        out<<name<<iplist[0]<<portlist[0]<<ssrclist[0];
        break;
    }
    case Callback:
    {
        out<<isStart2<<iplist[0]<<portlist[0]<<ssrclist[0];
        break;
    }
    case Callback2:
    {
        break;
    }
    case Invite:
    {
        out<<iplist[0]<<portlist[0]<<ssrclist[0]<<iplist[1]<<portlist[1]<<ssrclist[1];
        break;
    }
    case CutIn:
    {
        out<<QCip<<QCport;
        break;
    }
    case CutInCB:
    {
        out<<ssrclist[0]<<ssrclist[1]<<ssrclist[2];
        break;
    }
    }
    udpSocket->writeDatagram(data,data.length(),QHostAddress(destip),port);

}

void Widget::processRequest()
{
    while(udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(),datagram.size());
        QDataStream in(&datagram,QIODevice::ReadOnly);
        int messageType;
        in>>messageType;
        switch(messageType)
        {
        case Callback:
        {
            int YesorNo;
            uint16_t srcport;
            uint32_t srcip;
            uint32_t ssrc;
            in>>YesorNo>>srcip>>srcport>>ssrc;
            if (YesorNo == 1)
            {

//                add_dest(srcip,srcport);

                if (isStart == 0)
                {
                 add_dest(m_ip,m_port);
                CT = new CameraThread(&session);
                connect(CT,SIGNAL(captured()),this,SLOT(update()),Qt::BlockingQueuedConnection);
                isStart = 1;
                CT->start();



                AT = new AudioThread(au);
                AT->start();



                iplist[1] = srcip;
                portlist[1] = srcport;
                ssrclist[1] = ssrc;
                sleep(1);

                Vid = new Video(this,&session,&ad);
                Vid->ssrc[0]=ssrc;
                Vid->show();
                isStart2 =1;
                }
                else
                {
                iplist[2] =srcip;
                portlist[2] = srcport;
                ssrclist[2] = ssrc;
                Vid->ssrc[1] = ssrc;
                }

                struct in_addr tempip;
                tempip.s_addr = srcip;
                char *temp_ip = inet_ntoa(tempip);
                sendMessage(Callback2,temp_ip);
            }
            else if (YesorNo == 0)
            {
                QMessageBox::information(this,"Refused","Refused!!");
            }
            break;
        }
        case Request:
        {
            QString name;
            uint16_t srcport;
            uint32_t srcip;
            uint32_t ssrc;
            in>>name>>srcip>>srcport>>ssrc;

            int btn = QMessageBox::information(this,"Request",tr("Request from%1").arg(name),QMessageBox::Yes,QMessageBox::No);
            if (btn == QMessageBox::Yes)
            {

                add_dest(m_ip,m_port);

                iplist[1] = srcip;
                portlist[1] = srcport;
                ssrclist[1] = ssrc;

                Vid = new Video(this,&session,&ad);
                Vid->ssrc[0] = ssrc;
                Vid->show();
                isStart2 =1;
            }
            else if (btn == QMessageBox::No)
            {
                isStart2 = 0;
            }
            struct in_addr tempip;
            tempip.s_addr = srcip;
            char *temp_ip= inet_ntoa(tempip);
            sendMessage(Callback,temp_ip);

            break;
        }
        case Callback2:
        {
            if (isStart == 0)
            {
            CT = new CameraThread(&session);
            connect(CT,SIGNAL(captured()),this,SLOT(update()),Qt::BlockingQueuedConnection);
            isStart = 1;
            CT->start();

            AT = new AudioThread(au);
            AT->start();
            }

            break;
        }
        case Invite:
        {
        in>>iplist[1]>>portlist[1]>>ssrclist[1]>>iplist[2]>>portlist[2]>>ssrclist[2];
        int btn = QMessageBox::information(this,"Invitation","invitation",QMessageBox::Yes,QMessageBox::No);
        if (btn == QMessageBox::Yes)
        {
//            add_dest(iplist[1],portlist[1]);
//            add_dest(iplist[2],portlist[2]);

            add_dest(m_ip,m_port);

            Vid = new Video(this,&session,&ad);
            Vid->ssrc[0] = ssrclist[1];
            Vid->ssrc[1] = ssrclist[2];
            Vid->show();
            isStart2 =1;
        }
        else if (btn == QMessageBox::No)
        {
            isStart2 = 0;
        }
        struct in_addr tempip;
        tempip.s_addr = iplist[1];
        char *temp_ip= inet_ntoa(tempip);
        sendMessage(Callback,temp_ip);
        tempip.s_addr = iplist[2];
        temp_ip= inet_ntoa(tempip);
        sendMessage(Callback,temp_ip);
        break;
        }
        case CutIn:
        {
            in>>QCip>>QCport;
            add_dest(QCip,QCport);
            if (isServer == 1)
            {
                struct in_addr tempip;
                tempip.s_addr = iplist[1];
                char *temp_ip= inet_ntoa(tempip);
                sendMessage(CutIn,temp_ip);
                tempip.s_addr = iplist[2];
                temp_ip= inet_ntoa(tempip);
                sendMessage(CutIn,temp_ip);
                tempip.s_addr = QCip;
                temp_ip= inet_ntoa(tempip);
                sendMessage(CutInCB,temp_ip);
            }

            break;
        }
        }
    }
}

void Widget::on_open_clicked()
{
    QByteArray ba = ui->IP->text().toLatin1();
    char *serip = ba.data();
    int port = ui->port->text().toInt();


    CT = new CameraThread(&session);
    connect(CT,SIGNAL(captured()),this,SLOT(update()),Qt::BlockingQueuedConnection);
    isStart = 1;
    CT->start();

    AT = new AudioThread(au);
    AT->start();

    sleep(1);

    Vid = new Video(this,&session,&ad);
    Vid->show();
    isStart2 =1;

}

void Widget::on_closeCam_clicked()
{
    isStart = 0;
    CT->stop();
    free(CT);
    ui->label->setText("");

    AT->stop();
    free(AT);

    isStart2 = 0;
//    VT->stop();
//    free(VT);
    Vid->close();
    free(Vid);
//    ui->label2->setText("");
}


void Widget::on_Connect_clicked()
{
    QByteArray ba = ui->IP->text().toLatin1();
    char* destip=ba.data();
    sendMessage(Request,destip);
}


void Widget::on_adddest_clicked()
{
    QByteArray ba = ui->IP->text().toLatin1();
    char *serip = ba.data();
    int port = ui->port->text().toInt();

    uint16_t dest_port = port;
    uint32_t dest_ip = inet_addr(serip);
    dest_ip = ntohl(dest_ip);

    RTPIPv4Address addr(dest_ip,dest_port);
    status = session.AddDestination(addr);
}

void Widget::on_Initial_clicked()
{
    //RTP+RTCP库初始化SOCKET环境
    sessionparams.SetOwnTimestampUnit(1.0/90000.0); //时间戳单位
   sessionparams.SetAcceptOwnPackets(true);   //接收自己发送的数据包
    sessionparams.SetUsePredefinedSSRC(true);  //设置使用预先定义的SSRC

    uint32_t ssrc = ui->ssrc->text().toInt();
    sessionparams.SetPredefinedSSRC(ssrc);     //定义SSRC
    ssrclist[0] = ssrc;

    sessionparams.SetMaximumPacketSize(MAXDATASIZE);

    uint16_t baseport = ui->baseport->text().toInt();
    transparams.SetPortbase(baseport);
    portlist[0] = baseport;

    transparams.SetMulticastTTL(255);

    status = session.Create(sessionparams,&transparams);
    CheckError(status);

    session.SetDefaultPayloadType(H264);
    session.SetDefaultMark(false);
    session.SetDefaultTimestampIncrement(90000.0 /FRAMERATE);

    udpSocket = new QUdpSocket(this);
    port = 8080;
    udpSocket->bind(port,QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);

    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress address, list)
    {
        if(address.protocol() == QAbstractSocket::IPv4Protocol && address.toString().contains("192.168."))
        {
            QByteArray ba = address.toString().toLatin1();
            iplist[0] = inet_addr(ba.data());
        }
    }

    status = session.SupportsMulticasting();
    CheckError(status);

    m_ip =inet_addr("224.1.1.1");
    m_port = 8090;
    RTPIPv4Address m_addr(ntohl(m_ip),m_port);
    status = session.JoinMulticastGroup(m_addr);
    CheckError(status);

    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(processRequest()));
}


void Widget::on_invite_clicked()
{
    QByteArray ba = ui->IP->text().toLatin1();
    char* destip=ba.data();
    sendMessage(Invite,destip);
}

/*yuv格式转换为rgb格式*/
int Widget::convert_yuv_to_rgb_buffer(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height)
{
 unsigned int in, out = 0;
 unsigned int pixel_16;
 unsigned char pixel_24[3];
 unsigned int pixel32;
 int y0, u, y1, v;
 for(in = 0; in < width * height * 2; in += 4) {
  pixel_16 =
   yuv[in + 3] << 24 |
   yuv[in + 2] << 16 |
   yuv[in + 1] <<  8 |
   yuv[in + 0];
  y0 = (pixel_16 & 0x000000ff);
  u  = (pixel_16 & 0x0000ff00) >>  8;
  y1 = (pixel_16 & 0x00ff0000) >> 16;
  v  = (pixel_16 & 0xff000000) >> 24;
  pixel32 = convert_yuv_to_rgb_pixel(y0, u, v);
  pixel_24[0] = (pixel32 & 0x000000ff);
  pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
  pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;
  rgb[out++] = pixel_24[0];
  rgb[out++] = pixel_24[1];
  rgb[out++] = pixel_24[2];
  pixel32 = convert_yuv_to_rgb_pixel(y1, u, v);
  pixel_24[0] = (pixel32 & 0x000000ff);
  pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
  pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;
  rgb[out++] = pixel_24[0];
  rgb[out++] = pixel_24[1];
  rgb[out++] = pixel_24[2];

 }
 return 0;
}

int Widget::convert_yuv_to_rgb_pixel(int y, int u, int v)
{
 unsigned int pixel32 = 0;
 unsigned char *pixel = (unsigned char *)&pixel32;
 int r, g, b;
 r = y + (1.370705 * (v-128));
 g = y - (0.698001 * (v-128)) - (0.337633 * (u-128));
 b = y + (1.732446 * (u-128));
 if(r > 255) r = 255;
 if(g > 255) g = 255;
 if(b > 255) b = 255;
 if(r < 0) r = 0;
 if(g < 0) g = 0;
 if(b < 0) b = 0;
 pixel[0] = r * 220 / 256;
 pixel[1] = g * 220 / 256;
 pixel[2] = b * 220 / 256;
 return pixel32;
}
/*yuv格式转换为rgb格式*/
