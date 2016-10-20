#ifndef CAMERATHREAD_H
#define CAMERATHREAD_H

#include "cameraget.h"
#include "convert.h"
#include "encode.h"
#include "rtpsend.h"

#include <QMessageBox>

#include <QThread>

namespace Ui {
class CameraThread;
}

class CameraThread : public QThread
{
    Q_OBJECT

public:
    explicit CameraThread(RTPSession *session);
    ~CameraThread();

    void *cap_buf;

    void stop();

private: 
    cameraGet *cp;
    convert *cr;
    encode *en;
    rtpsend *rs;

    int ret;
    bool stopped;

    void *cvt_buf,*hd_buf,*enc_buf;
    char *pac_buf ;
    int cap_len, cvt_len, hd_len, enc_len, pac_len;
    enum encode::pic_t ptype;

    unsigned long framecount;

 signals:
    void captured();

protected:
  void run();
};



#endif // CAMERATHREAD_H
