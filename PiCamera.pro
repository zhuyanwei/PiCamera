#-------------------------------------------------
#
# Project created by QtCreator 2015-08-23T13:45:54
#
#-------------------------------------------------

QT       += core gui
QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PiCamera
TEMPLATE = app


SOURCES += main.cpp\
        cameraget.cpp \
    convert.cpp \
    encode.cpp \
    widget.cpp\
    rtpsend.cpp \
    decode.cpp \
    rtpreceive.cpp \
    camerathread.cpp \
#    src/rtpudpv6transmitter.cpp \
#    src/rtpudpv4transmitter.cpp \
#    src/rtptimeutilities.cpp \
#    src/rtpsources.cpp \
#    src/rtpsourcedata.cpp \
#    src/rtpsessionsources.cpp \
#    src/rtpsessionparams.cpp \
#    src/rtpsession.cpp \
#    src/rtprandomurandom.cpp \
#    src/rtprandomrands.cpp \
#    src/rtprandomrand48.cpp \
#    src/rtprandom.cpp \
#    src/rtppollthread.cpp \
#    src/rtppacketbuilder.cpp \
#    src/rtppacket.cpp \
#    src/rtplibraryversion.cpp \
#    src/rtpipv6address.cpp \
#    src/rtpipv4address.cpp \
#    src/rtpinternalsourcedata.cpp \
#    src/rtpexternaltransmitter.cpp \
#    src/rtperrors.cpp \
#    src/rtpdebug.cpp \
#    src/rtpcollisionlist.cpp \
#    src/rtpbyteaddress.cpp \
#    src/rtcpsrpacket.cpp \
#    src/rtcpsdespacket.cpp \
#    src/rtcpsdesinfo.cpp \
#    src/rtcpscheduler.cpp \
#    src/rtcprrpacket.cpp \
#    src/rtcppacketbuilder.cpp \
#    src/rtcppacket.cpp \
#    src/rtcpcompoundpacketbuilder.cpp \
#    src/rtcpcompoundpacket.cpp \
#    src/rtcpbyepacket.cpp \
#    src/rtcpapppacket.cpp \
#    src/jthread.cpp \
#    src/jmutex.cpp
    video.cpp \
    audiothread.cpp \
    audioget.cpp \
    au_encode.cpp \
    au_decode.cpp


HEADERS  += cameraget.h \
    comdef.h \
    config.h \
    convert.h \
    encode.h \
    ffmpeg_common.h \
    widget.h \
    rtpsend.h \
    decode.h \
    rtpreceive.h \
    camerathread.h \
    video.h \
    audiothread.h \
    audioget.h \
    au_encode.h \
    au_decode.h

FORMS    += widget.ui \
    video.ui

INCLUDEPATH+= /usr/include/ \
/usr/local/FFMpeg/include \
/usr/local/portaudio/include \
#/usr/local/include/jrtplib3 \

LIBS+=/usr/lib/arm-linux-gnueabihf/libopencv_core.so \
/usr/lib/arm-linux-gnueabihf/libopencv_highgui.so \
 /usr/local/FFMpeg/lib/libavformat.so \
/usr/local/FFMpeg/lib/libavcodec.so \
/usr/local/FFMpeg/lib/libavutil.so\
/usr/local/FFMpeg/lib/libswscale.so \
/usr/local/FFMpeg/lib/libswresample.so \
/usr/local/FFMpeg/lib/libswresample.so.1 \
#/usr/local/JRtp/lib/libjrtp.so \
/usr/lib/arm-linux-gnueabihf/libportaudio.so.2 \

