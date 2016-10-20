#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QUdpSocket>
#include "comdef.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
class QUdpSocket;

class network : public QObject
{
    Q_OBJECT
public:
    network();
    ~network();

    int sktfd;
    struct sockaddr_in server_sock;
    int sersock_len;

    enum net_t
    {
        UDP = 0, TCP
    };

    enum net_t type;		// UDP or TCP?
    char * serip;			// server ip, eg: "127.0.0.1"
    int serport;			// server port, eg: 8000

    int net_open(enum net_t type,char *serip,int serport);

    int net_send(void *data, int size);

    int net_recv(void *data, int size);

    void net_close();

   QUdpSocket *udpSocket;
   int port;



signals:
    
public slots:

    
};

#endif // NETWORK_H
