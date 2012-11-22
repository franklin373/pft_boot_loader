#ifndef __P25_PROTOCOL_H__
#define __P25_PROTOCOL_H__

/**
 * Copyright 2004 - 2007 Blue Bamboo International Inc. 
 *           All rights reserved.
 *
 *
 * Blue Bamboo PROPRIETARY/CONFIDENTIAL.
 *
 */
//int p25_print_text(HANDLE hFile,BYTE *ucTextBuf,DWORD iDataLen);
//int p25_print_null_tail_text(HANDLE hFile,BYTE *ucTextBuf);
#if 0
typedef struct
{
    BYTE  i8TypeOfFrame;
    BYTE  i8DataId;
    BYTE * pCmdBuf;
    unsigned long  CmdBufLen;
} Prn_Frame_t;
#else
typedef struct
{
    BYTE  i8DataId;
    BYTE * pFlagBuf;//would inclued both i8TypeOfFrame and CmdBuf
    unsigned long  FlagBufLen;
} Prn_Frame_t;
#endif






typedef enum
{
    PRN_FRAME_ACK = 0x06,
    PRN_FRAME_NACK = 0x15,
    PRN_FRAME_ENQ = 0x05,
    PRN_FRAME_PRN_DATA = 0x44,
    PRN_FRAME_RESPONSE_STATUS = 0x53,
    PRN_FRAME_INQUIRY_STATUS = 0x51,
    PRN_FRAME_EOT = 0x04,
    PRN_FRAME_ETX = 0x03,
    PRN_FRAME_SOF = 0xC0,
    PRN_FRAME_EOF = 0xC1

} PRN_TypeOfFrame;

#define     PRN_FRAME_FLASH_ERASE 0x45
#define     PRN_FRAME_MODE_DOWNLOAD 0x46
#define 	PRN_FRAME_READ_SNVER	0x47






#if 0
typedef struct
{
    BYTE  i8CryptType;
//    BYTE  i8DataId;
    BYTE i8KeyId;
    BYTE * pCmdBuf;
    unsigned long  CmdBufLen;
} For_Crypt_Frame_t;
typedef struct
{
    BYTE  i8CryptType;
    BYTE  i8DataId;
    BYTE ksn_static[10];
    unsigned long count;
    BYTE * pCmdBuf;
    unsigned long  CmdBufLen;
}Dukpt_Plain_Frame_t;

#define CMD_PRN_FRAME_MSR (0x48)



#define CT_ENCRYPT_WITH_MASTER (0x80)
#define CT_ENCRYPT_WITH_SESSION (0x81)
#define CMD_PRN_FRAME_MSR_RESPONSE_IPHONE (0x84)
#define CMD_SYNC_SESSION_KEY_DOWN_REQ (0x85)
#define CMD_SYNC_SESSION_KEY_DOWN_RESPONSE (0x86)
#define CMD_PRN_REQ_WILL_ENCRYPT (0x87)
#define CMD_PRINT_RESPONSE (0x88)
#define CMD_MSR_REQ (0x89)
#define CT_ENCRYPT_WITH_DUKPT (0x90)
#define CMD_MSR_RESPONSE (0x8a)
#define CMD_SET_MASTER_KEY_DOWN_REQ (0x8b)
#define CMD_SET_MASTER_KEY_DOWN_RESPONSE (0x8c)
#define CMD_SET_DUKPT_KEY_DOWN_REQ (0x9b)
#define CMD_SET_DUKPT_KEY_DOWN_RESPONSE (0x9c)
#define CMD_CMD_LINE_LIKE_REQ (0x9d)
#define CMD_CMD_LINE_LIKE_REPLY (0x9d)
#define CMD_CMD_LINE_LIKE_LEN_HEAD_REQ (0x9e)
#define CMD_CMD_LINE_LIKE_LEN_HEAD_REPLY (0x9f)
//#define CMD_PRN_FRAME_MSR_DUKPT (0x91)



#define ENCRYPT_ALG_3DES (0x01)
#define ENCRYPT_ALG_TDUKPT (0x04)
#define ENCRYPT_ALG_DUKPT_JP (0x06)


#define CARD_TYPE_ISO (0x00)
#define CARD_TYPE_JIS2 (0x01)
#endif



void CalCheckSum(BYTE *Buf, UINT Len, BYTE * EvenChk, BYTE * OddChk);
#define SNDLEN 20020
int encodeFrame(/*BYTE *buf,*/Prn_Frame_t *pFrame);
int encodeFlagBuf(uint8 *flagBuf,int flagBufLen);
extern const char resp_data_recved[];
//int encrypt_dukpt_pkg(Dukpt_Plain_Frame_t *plain,Prn_Frame_t *encrypt);
//int decrypt_dukpt_pkg(Prn_Frame_t *encrypt,Dukpt_Plain_Frame_t *plain);
//int decrypt_pkg(uint8 *pFlagBuf,int FlagBufLen,For_Crypt_Frame_t *plain);
int CmdDecodeAndCheck(unsigned char *pCmdBuf,unsigned int uiCmdLen,Prn_Frame_t *frame);
int BlueBambooCmdDecodeAndCheck(unsigned char *pCmdBuf,unsigned int uiCmdLen,Prn_Frame_t *frame);
#endif
