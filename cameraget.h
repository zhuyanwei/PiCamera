#ifndef cameraGET_H
#define cameraGET_H

#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>

#include "comdef.h"
#include <sys/ioctl.h>
#include <sys/mman.h>

#include <asm/types.h>
#include <linux/videodev2.h>

#include <QString>
#include <QObject>


class cameraGet : public QObject
{
    Q_OBJECT
public:
    cameraGet(QString dev_name);
    ~cameraGet();

    //VideoDevice();
    int open_device();
    int close_device();
    int init_device();
    int start_capturing();
    int stop_capturing();
    int uninit_device();
    int get_frame(void **, size_t*);
    int unget_frame();

private:
    int init_mmap();

    struct buffer
    {
        void * start;
        size_t length;
    };
    QString dev_name;
    int fd;
    buffer* buffers;
    unsigned int n_buffers;
    int index;

signals:
     void display_error(QString);


};

#endif // cameraGET_H
