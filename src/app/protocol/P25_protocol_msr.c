/**
 * Copyright 2004 - 2007 Blue Bamboo International Inc. 
 *           All rights reserved.
 *
 *
 * Blue Bamboo PROPRIETARY/CONFIDENTIAL.
 *
 */
#include "global.h"
#include "P25_protocol.h"
/***********************************************************************
 ***********contract pCmdBuf to pCmdBuf, reduce alternative character***********
 ***********************************************************************/
unsigned int CmdDecode(unsigned char *pCmdBuf,unsigned int uiCmdLen)
{
	unsigned int i=0,j=0;
	unsigned char Xch;
	
	for(i=0;i<uiCmdLen;)
	{
		Xch = pCmdBuf[i];					//Get each byte in pCmdBuf
		if(Xch==0x7d)						//If the byte is 0x7d,
		{									//then next byte must be XOR with 0x7d
			i++;
			pCmdBuf[j] = pCmdBuf[i]^0x20;
		}
		else 
			pCmdBuf[j] = Xch;
		i++;j++;
	}
	
	return j;
}
int CalcLen(BYTE *pLenString)
{
	int result=0;
	int i;

	for(i=0;i<4;i++){
		if(pLenString[i]<0x30||pLenString[i]>0x39){
			return -1;
		}
		result=result*10+pLenString[i]-0x30;
	}
	return result;
}
/*************************************************************************
 *****************pCmdBuf to frame*****************************************
 *************************************************************************/
#if 0	
/*	if(pCmdBuf[1]==PRN_FRAME_ENQ||pCmdBuf[1]==PRN_FRAME_INQUIRY_STATUS||
			pCmdBuf[1]==PRN_FRAME_RESPONSE_STATUS||pCmdBuf[1]==PRN_FRAME_EOT){
		frame->i8TypeOfFrame=pCmdBuf[1];
		frame->i8DataId=pCmdBuf[2];
	}else*/ if(1/*pCmdBuf[1]==PRN_FRAME_MSR||pCmdBuf[1]==PRN_FRAME_PRN_DATA*/){
#endif	
#if 0
int BlueBambooCmdDecodeAndCheck(unsigned char *pCmdBuf,unsigned int uiCmdLen,Prn_Frame_t *frame)
{
	int decodeLen;
	BYTE eventChk,OddChk;
        int calcLen;

        if(pCmdBuf[0]!=PRN_FRAME_SOF){
                return -12;
        }
	memset(frame,'\0',sizeof(Prn_Frame_t));
	decodeLen=CmdDecode(pCmdBuf,uiCmdLen);
	if(decodeLen<=4&&pCmdBuf[2]==PRN_FRAME_EOF){
		frame->i8TypeOfFrame=pCmdBuf[1];
		frame->i8DataId='0';
                frame->CmdBufLen=0;
		frame->pCmdBuf=pCmdBuf;
	}else if(decodeLen>7){
	    if(pCmdBuf[decodeLen-1]!=PRN_FRAME_EOF){
            return -13;
        }
		frame->i8TypeOfFrame=pCmdBuf[1];
		frame->i8DataId=pCmdBuf[2];
		calcLen=CalcLen(pCmdBuf+3);
		if(calcLen<0||calcLen!=decodeLen-10){
//			Log2FreeUart(LOG_MASK_PRN,FL("CalcLen=%d,decodLen=%d\r\n"),calcLen,decodeLen);
			fprintf(stderr,"CalcLen=%d,decodLen=%d\r\n",calcLen,decodeLen);
			return -4;
		}
		CalCheckSum(pCmdBuf+7,decodeLen-10,&eventChk,&OddChk);
		if(eventChk!=pCmdBuf[decodeLen-3]||OddChk!=pCmdBuf[decodeLen-2]){
			return -3;
		}
		frame->CmdBufLen=decodeLen-10;
		memmove(pCmdBuf,pCmdBuf+7,frame->CmdBufLen);
		frame->pCmdBuf=pCmdBuf;
	}else{
		return -9;
	}
//	global_context.protocol_type=PROTOCAL_TYPE_BLUEBAMBOO;
	return 0;
}
#else
int BlueBambooCmdDecodeAndCheck(unsigned char *pCmdBuf,unsigned int uiCmdLen,Prn_Frame_t *frame)
{
	int decodeLen;
	BYTE eventChk,OddChk;
        int calcLen;

        if(pCmdBuf[0]!=PRN_FRAME_SOF){
                return -12;
        }
	memset(frame,'\0',sizeof(Prn_Frame_t));
	decodeLen=CmdDecode(pCmdBuf,uiCmdLen);
	if(decodeLen<=4&&pCmdBuf[2]==PRN_FRAME_EOF){
		frame->pFlagBuf=pCmdBuf;
		frame->pFlagBuf[0]=pCmdBuf[1];
		frame->i8DataId='0';
        frame->FlagBufLen=1;
	}else if(decodeLen>7){
	    if(pCmdBuf[decodeLen-1]!=PRN_FRAME_EOF){
            return -13;
        }
		frame->pFlagBuf=pCmdBuf;
		frame->pFlagBuf[0]=pCmdBuf[1];
		frame->i8DataId=pCmdBuf[2];
		calcLen=CalcLen(pCmdBuf+3);
		if(calcLen<0||calcLen!=decodeLen-10){
//			fprintf(stderr,"CalcLen=%d,decodLen=%d\r\n",calcLen,decodeLen);
			return -4;
		}
		CalCheckSum(pCmdBuf+7,decodeLen-10,&eventChk,&OddChk);
		if(eventChk!=pCmdBuf[decodeLen-3]||OddChk!=pCmdBuf[decodeLen-2]){
			return -3;
		}
#if 0        
		frame->CmdBufLen=decodeLen-10;
		memmove(pCmdBuf,pCmdBuf+7,frame->CmdBufLen);
#else
		frame->FlagBufLen=decodeLen-10+1;
		memmove(pCmdBuf+1,pCmdBuf+7,frame->FlagBufLen-1);
#endif
	}else{
		return -9;
	}
	return 0;
}
#endif
