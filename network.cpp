#include "network.h"
#include <QDebug>

network::network()
{
    udpSocket = new QUdpSocket(this);
    port = 8080;
    udpSocket->bind(port,QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);
}

network::~network()
{
}

int network::net_open(enum net_t type,char *serip,int serport)
{

    this->type = type;
    this->serip = serip;
    this->serport = serport;

    if (this->type == TCP)
        this->sktfd = socket(AF_INET, SOCK_STREAM, 0);
    else
        // UDP
        this->sktfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (this->sktfd < 0)
    {
        qDebug()<<"--- create socket failed\n";
        free(this);
        return -1;
    }
    this->server_sock.sin_family = AF_INET;
    this->server_sock.sin_port = htons(this->serport);
    this->server_sock.sin_addr.s_addr = inet_addr(this->serip);
    this->sersock_len = sizeof(this->server_sock);


    int ret = ::connect(this->sktfd, (struct sockaddr *) &this->server_sock,
            this->sersock_len);
    if (ret < 0)
    {
        qDebug()<<"--- connect to server failed";
        ::close(this->sktfd);
        free(this);
        return -1;
    }

    printf("+++ Network Opened\n");
    return 0;
}

void network::net_close()
{
    ::close(this->sktfd);
    free(this);
    printf("+++ Network Closed\n");
}

int network::net_send(void *data, int size)
{
    return send(this->sktfd, data, size, 0);
}

int network::net_recv(void *data, int size)
{
    return recv(this->sktfd, data, size, 0);
}


