#include "camerathread.h"

#include<QDebug>

CameraThread::CameraThread(RTPSession *session)
{   
    cp= new cameraGet(tr("/dev/video0"));
    cr= new convert();
    en= new encode();
    rs= new rtpsend();

    // set paraments
    AVPixelFormat vfmt = AV_PIX_FMT_YUYV422;
    AVPixelFormat ofmt = AV_PIX_FMT_YUV420P;

    cr->inwidth = WIDTH;
    cr->inheight = HEIGHT;
    cr->inpixfmt = vfmt;
    cr->outwidth = WIDTH;
    cr->outheight = HEIGHT;
    cr->outpixfmt = ofmt;

    en->src_picwidth = WIDTH;
    en->src_picheight = HEIGHT;
    en->enc_picwidth = WIDTH;
    en->enc_picheight = HEIGHT;
    en->chroma_interleave = 0;
    en->fps = FRAMERATE;
    en->gop = 10;
    en->bitrate = 400;

    rs->max_pkt_len = MAXDATASIZE - 20;
    rs->ssrc = SSRC;

    pac_buf = (char *) malloc(MAXDATASIZE);

    ret = cp->open_device();
    if(-1==ret)
    {
//        QMessageBox::warning(this,tr("error"),tr("open /dev/dsp error"),QMessageBox::Yes);
        cp->close_device();
    }

    ret = cp->init_device();
    if(-1==ret)
    {
//        QMessageBox::warning(this,tr("error"),tr("init failed"),QMessageBox::Yes);
        cp->close_device();
    }

    ret = cp->start_capturing();
    if(-1==ret)
    {
//        QMessageBox::warning(this,tr("error"),tr("start capture failed"),QMessageBox::Yes);
        cp->close_device();
    }

    ret = cr->convert_open(cr->inwidth,cr->inheight, cr->inpixfmt, cr->outwidth, cr->outheight, cr->outpixfmt);

    ret = en->encode_open(en->src_picwidth,en->src_picheight,en->enc_picwidth, en->enc_picheight,
                    en->fps,en->bitrate,en->gop, en->chroma_interleave);

    ret = rs->pack_open(rs->max_pkt_len,rs->ssrc);

    ret = rs->net_open(session);

    stopped = false;

}

CameraThread::~CameraThread()
{
    free(pac_buf);
    rs->net_close();
    en->encode_close();
    cr->convert_close();

    cp->stop_capturing();
    cp->uninit_device();
    cp->close_device();
}

void CameraThread::run()
{
    while(!stopped)
    {
        // ret = cp->capture_get_data((void**)&p, &cap_len);
               ret = cp->get_frame(&cap_buf,(size_t *)&cap_len);
               if (ret != 0)
                       {
                           if (ret < 0)		// error
                           {
                              qDebug()<<"--- capture_get_data failed";
                           }
                           else	// again
                           {
                               usleep(10000);
                           }
                       }
                       if (cap_len <= 0)
                       {
                           qDebug()<<"!!! No capture data";
                       }

    emit captured();

    ret = cr->convert_do(cap_buf, cap_len, &cvt_buf, &cvt_len);
    if (ret < 0)
    {
       qDebug()<<"--- convert_do failed";
//         break;
    }
    if (cvt_len <= 0)
    {
        qDebug()<<"!!! No convert data\n";
//        continue;
    }
    // else

        // fetch h264 headers first!
//    while ((ret = en->encode_get_headers(&hd_buf, &hd_len, &ptype))
//            == 1)
//    {
//        //fwrite(hd_buf, 1, hd_len, dumpfile);
//        pa->pack_put(hd_buf, hd_len);
//        while (pa->pack_get(pac_buf, MAX_RTP_SIZE, &pac_len) == 1)
//        {
//            ret = ne->net_send(pac_buf, pac_len);
//            if (ret != pac_len)
//            {
//                printf("send pack data failed, size: %d, err: %s\n", pac_len,
//                        strerror(errno));
//            }
//        }
//    }


    ret = en->encode_do(cvt_buf, cvt_len, &enc_buf, &enc_len,
            &ptype);
    if (ret < 0)
    {
        qDebug()<<"--- encode_do failed\n";
    //    break;
    }
    if (enc_len <= 0)
    {
        qDebug()<<"!!! No encode data\n";
    //    continue;
    }
    // else
    //fwrite(enc_buf, 1, enc_len, dumpfile);
    // RTP pack and send

    rs->pack_put(enc_buf, enc_len);
    while(rs->pack_get(pac_buf, MAXDATASIZE, &pac_len) == 1)
    {

    }

    ret= cp->unget_frame();
    framecount++;
    }

    stopped = true;

// label->show();
// label->drawFrame();

//    QPixmap *pixImage = new QPixmap();
//    pixImage->loadFromData((uchar *)pp,sizeof(pp),0,Qt::AutoColor);
//    QPainter painter(this);
//    painter.begin(this);
//    painter.drawPixmap(0,0,QWidget::width(),QWidget::height(),*pixImage);
//    painter.end();
}

void CameraThread::stop()
{
    stopped = true;
}

