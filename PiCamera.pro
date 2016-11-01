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
#    src/rtpudpv6transmitter.h \
#    src/rtpudpv4transmitter.h \
#    src/rtptypes.h \
#    src/rtptypes_win.h \
#    src/rtptransmitter.h \
#    src/rtptimeutilities.h \
#    src/rtpstructs.h \
#    src/rtpsources.h \
#    src/rtpsourcedata.h \
#    src/rtpsessionsources.h \
#    src/rtpsessionparams.h \
#    src/rtpsession.h \
#    src/rtprawpacket.h \
#    src/rtprandomurandom.h \
#    src/rtprandomrands.h \
#    src/rtprandomrand48.h \
#    src/rtprandom.h \
#    src/rtppollthread.h \
#    src/rtppacketbuilder.h \
#    src/rtppacket.h \
#    src/rtpmemoryobject.h \
#    src/rtpmemorymanager.h \
#    src/rtplibraryversion.h \
#    src/rtpkeyhashtable.h \
#    src/rtpipv6destination.h \
#    src/rtpipv6address.h \
#    src/rtpipv4destination.h \
#    src/rtpipv4address.h \
#    src/rtpinternalsourcedata.h \
#    src/rtphashtable.h \
#    src/rtpexternaltransmitter.h \
#    src/rtperrors.h \
#    src/rtpdefines.h \
#    src/rtpdebug.h \
#    src/rtpconfig.h.in \
#    src/rtpconfig.h \
#    src/rtpcollisionlist.h \
#    src/rtpbyteaddress.h \
#    src/rtpaddress.h \
#    src/rtcpunknownpacket.h \
#    src/rtcpsrpacket.h \
#    src/rtcpsdespacket.h \
#    src/rtcpsdesinfo.h \
#    src/rtcpscheduler.h \
#    src/rtcprrpacket.h \
#    src/rtcppacketbuilder.h \
#    src/rtcppacket.h \
#    src/rtcpcompoundpacketbuilder.h \
#    src/rtcpcompoundpacket.h \
#    src/rtcpbyepacket.h \
#    src/rtcpapppacket.h \
#    src/jthreadconfig.h.in \
#    src/jthreadconfig.h \
#    src/jthread.h \
#    src/jmutexautolock.h \
#    src/jmutex.h
    video.h \
    audiothread.h \
    audioget.h \
    au_encode.h \
    au_decode.h

FORMS    += widget.ui \
    video.ui

INCLUDEPATH+= /usr/include/opencv \
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
/usr/local/lib/libjrtp.so \
/usr/lib/arm-linux-gnueabihf/libportaudio.so.2 \

