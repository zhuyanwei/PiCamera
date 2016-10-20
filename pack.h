#ifndef PACK_H
#define PACK_H

#include <QObject>
#include "comdef.h"
extern "C"{
#include <stdio.h>
#include <stdlib.h>
}
#include <string.h>
#include <assert.h>

#include <winsock2.h>

#include <sys/timeb.h>

class pack : public QObject
{
    Q_OBJECT
public:
    pack();

    typedef struct
    {
        // Little Endian
        // byte 0
        unsigned char csrc_len :4;    // contains the number of CSRC identifiers that follow the fixed header.
        unsigned char extension :1;    // extension bit
        unsigned char padding :1;    // padding
        unsigned char version :2;    // version, current is 2
        // byte 1
        unsigned char payload :7;    // payload
        unsigned char marker :1;    // marker
        // bytes 2, 3
        unsigned short seq_no;
        // bytes 4-7
        unsigned long timestamp;
        // bytes 8-11
        unsigned long ssrc;    // sequence number
    } rtp_header;

    typedef struct
    {
        unsigned char TYPE :5;
        unsigned char NRI :2;
        unsigned char F :1;
    } nalu_header;    // 1 Bytes

    typedef struct
    {
        //byte 0
        unsigned char TYPE :5;
        unsigned char NRI :2;
        unsigned char F :1;
    } fu_indicator;    // 1 BYTE

    typedef struct
    {
        //byte 0
        unsigned char TYPE :5;
        unsigned char R :1;
        unsigned char E :1;
        unsigned char S :1;
    } fu_header;    // 1 BYTE

    typedef struct
    {
        int startcodeprefix_len;    // 0x000001 or 0x00000001
        char *data;    // nalu data
        int len;    // nalu length
        int forbidden_bit;    //
        int nal_reference_idc;    //
        int nal_unit_type;    // nalu types
    } nalu_t;

    void *inbuf;
    char *next_nalu_ptr;
    int inbuf_size;
    int FU_counter;
    int last_FU_size;
    int FU_index;
    int inbuf_complete;
    int nalu_complete;
    nalu_t nalu;
    unsigned short seq_num;
    U32 ts_start_millisec;		// timestamp in millisecond
    U32 ts_current_sample;		// timestamp in 1/90000.0 unit

    int max_pkt_len;    // maximum packet length, better be less than MTU(1500)
    int ssrc;			// identifies the synchronization source, set the value randomly, with the intent that no two synchronization sources within the same RTP session will have the same SSRC


   int pack_open(int max_pkt_len,int ssrc);


    void pack_put(void *inbuf,int isize);

    int pack_get(void *outbuf, int bufsize,
            int *outsize);

    void pack_close();

private:
    U32 get_current_millisec(void);

    int is_start_code4(char *buf);
    int is_start_code3(char *buf);
    int get_next_nalu();
    void dump_nalu(const nalu_t *nalu);

signals:
    
public slots:
    
};

#endif
