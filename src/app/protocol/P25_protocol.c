/**
 * Copyright 2004 - 2007 Blue Bamboo International Inc. 
 *           All rights reserved.
 *
 *
 * Blue Bamboo PROPRIETARY/CONFIDENTIAL.
 *
 */
//#include "stdafx.h"
#include "global.h"
#include "P25_protocol.h"

void CalCheckSum(BYTE *Buf, UINT Len, BYTE * EvenChk, BYTE * OddChk)
{
	BYTE ChkSum;
	UINT i;

	ChkSum = 0;
	for(i=0; i<Len; i+=2)
		ChkSum ^= Buf[i];
	*EvenChk = ChkSum;

	ChkSum = 0;
	for(i=1; i<Len; i+=2)
		ChkSum ^= Buf[i];
	*OddChk = ChkSum;
}
unsigned int InnerCmdEncode(BYTE *dest, BYTE* src,unsigned long uiCmdLen)
{
	BYTE xch;
	int i=0,k;
		
	for(k=0,i=0; k<(int)uiCmdLen; k++)
	{
		xch = *(src+k);
		if((xch == 0x7d) || (xch == 0xc0) || (xch == 0xc1))
		{
			xch ^= 0x20;
			if(dest){
			    dest[i] = 0x7d;
			}
			i++;
		}
		if(dest){
		    dest[i]	= xch;
		}
		i++;
	}
	
	return i;
}
unsigned int CmdEncode(BYTE *buf, unsigned long len)
{
    unsigned int resultLen;
	int i,j;

    resultLen=InnerCmdEncode(NULL,buf,len);
    for(i=len-1,j=resultLen-1;i>=0;i--,j--){
        buf[j]=buf[i];
    }
    resultLen=InnerCmdEncode(buf,buf+resultLen-len,len);
    return resultLen;
}
#if 0
/********************************************************************
 ****expand alternative character process, pMSRBufCal to TempBuf, ************
 ****return result length*************************************************
 ********************************************************************/
unsigned int CmdEncode(BYTE *TempBuf, BYTE* pMSRBufCal,unsigned long uiCmdLen)
{
	BYTE xch;
	int i=0,k;
		
	for(k=0,i=0; k<(int)uiCmdLen; k++)
	{
		xch = *(pMSRBufCal+k);
		if((xch == 0x7d) || (xch == 0xc0) || (xch == 0xc1))
		{
			xch ^= 0x20;
			TempBuf[i++] = 0x7d;
		}
		TempBuf[i++]	= xch;
	}
	
	return i;
}
#endif
/********************************************************************
 ***********pFrame to buf**********************************************
 ********************************************************************/
#if 0 
int encodeFrame(BYTE *buf,Prn_Frame_t *pFrame)
{
	static BYTE TempBuf[10000];
	BYTE EvenChk,OddChk;
	int len;

	TempBuf[0] = pFrame->i8TypeOfFrame;
	TempBuf[1] = pFrame->i8DataId;
	TempBuf[2] = (BYTE)(pFrame->CmdBufLen/1000+0x30);
	TempBuf[3] = (BYTE)(pFrame->CmdBufLen%1000)/100+0x30;
	TempBuf[4] = (BYTE)(pFrame->CmdBufLen%100)/10+0x30;
	TempBuf[5] = (BYTE)pFrame->CmdBufLen%10+0x30;
	memcpy(TempBuf+6,pFrame->pCmdBuf,pFrame->CmdBufLen);
		
	CalCheckSum(pFrame->pCmdBuf,pFrame->CmdBufLen,&EvenChk,&OddChk);
	
	TempBuf[pFrame->CmdBufLen+6] = EvenChk;
	TempBuf[pFrame->CmdBufLen+7] = OddChk;

	buf[0]=0xc0;
	len=CmdEncode(buf+1,TempBuf,pFrame->CmdBufLen+8);
//	ASSERT_MINE(len>0);
	buf[len+1]=0xc1;
	return len+2;
}
#else
/*
int encodeFrame(BYTE *buf,Prn_Frame_t *pFrame)
{
	BYTE EvenChk,OddChk;
	int len;

	buf[0]=0xc0;

	buf[1] = pFrame->i8TypeOfFrame;
	buf[2] = pFrame->i8DataId;
	buf[3] = (BYTE)(pFrame->CmdBufLen/1000+0x30);
	buf[4] = (BYTE)(pFrame->CmdBufLen%1000/100)+0x30;
	buf[5] = (BYTE)(pFrame->CmdBufLen%100/10)+0x30;
	buf[6] = (BYTE)(pFrame->CmdBufLen%10)+0x30;
	memcpy(buf+7,pFrame->pCmdBuf,pFrame->CmdBufLen);
		
	CalCheckSum(pFrame->pCmdBuf,pFrame->CmdBufLen,&EvenChk,&OddChk);
	
	buf[pFrame->CmdBufLen+7] = EvenChk;
	buf[pFrame->CmdBufLen+8] = OddChk;

	len=CmdEncode(buf+1,pFrame->CmdBufLen+8);
//	ASSERT_MINE(len>0);
	buf[len+1]=0xc1;
	return len+2;
}
*/
#if 0
int encodeFrame(Prn_Frame_t *pFrame)
{
	BYTE EvenChk,OddChk;
	int len;

	CalCheckSum(pFrame->pCmdBuf,pFrame->CmdBufLen,&EvenChk,&OddChk);

	memmove(pFrame->pCmdBuf+7,pFrame->pCmdBuf,pFrame->CmdBufLen);
		

	pFrame->pCmdBuf[0]=0xc0;

	pFrame->pCmdBuf[1] = pFrame->i8TypeOfFrame;
	pFrame->pCmdBuf[2] = pFrame->i8DataId;
	pFrame->pCmdBuf[3] = (BYTE)(pFrame->CmdBufLen/1000+0x30);
	pFrame->pCmdBuf[4] = (BYTE)(pFrame->CmdBufLen%1000/100)+0x30;
	pFrame->pCmdBuf[5] = (BYTE)(pFrame->CmdBufLen%100/10)+0x30;
	pFrame->pCmdBuf[6] = (BYTE)(pFrame->CmdBufLen%10)+0x30;
	
	pFrame->pCmdBuf[pFrame->CmdBufLen+7] = EvenChk;
	pFrame->pCmdBuf[pFrame->CmdBufLen+8] = OddChk;

	len=CmdEncode(pFrame->pCmdBuf+1,pFrame->CmdBufLen+8);
	pFrame->pCmdBuf[len+1]=0xc1;
	return len+2;
}
#else
int encodeFrame(Prn_Frame_t *pFrame)
{
	BYTE EvenChk,OddChk;
	int len;
    int CmdBufLen;

	CalCheckSum(pFrame->pFlagBuf+1,pFrame->FlagBufLen-1,&EvenChk,&OddChk);
    CmdBufLen=pFrame->FlagBufLen-1;
	memmove(pFrame->pFlagBuf+7,pFrame->pFlagBuf+1,CmdBufLen);
		

	pFrame->pFlagBuf[1] = pFrame->pFlagBuf[0];
	pFrame->pFlagBuf[0]=0xc0;

	pFrame->pFlagBuf[2] = pFrame->i8DataId;
	pFrame->pFlagBuf[3] = (BYTE)(CmdBufLen/1000+0x30);
	pFrame->pFlagBuf[4] = (BYTE)(CmdBufLen%1000/100)+0x30;
	pFrame->pFlagBuf[5] = (BYTE)(CmdBufLen%100/10)+0x30;
	pFrame->pFlagBuf[6] = (BYTE)(CmdBufLen%10)+0x30;
	
	pFrame->pFlagBuf[CmdBufLen+7] = EvenChk;
	pFrame->pFlagBuf[CmdBufLen+8] = OddChk;

	len=CmdEncode(pFrame->pFlagBuf+1,CmdBufLen+8);
	pFrame->pFlagBuf[len+1]=0xc1;
	return len+2;
}
#if 0
int encodeFlagBuf(uint8 *pFlagBuf,int FlagBufLen)
{
    Prn_Frame_t frame;

    frame.pFlagBuf=pFlagBuf;
    frame.FlagBufLen=FlagBufLen;
    frame.i8DataId=global_context.i8DataId;
    return encodeFrame(&frame);
}
#endif
#endif
#endif
#if 0
int p25_print_text(HANDLE hFile,BYTE *ucTextBuf,DWORD iDataLen)
{
	int iIndex;
	static BYTE bDataID = 0;
	BYTE ucSendBuf[SNDLEN];
	BYTE ucRecvBuf[1000];
	long iALen;
	Prn_Frame_t frame;
	char eResponse[8];

	Com_Port_Reset(hFile);
	iIndex = 0;
	if(bDataID==10)
		bDataID=0;
	memset(ucSendBuf,0,SNDLEN);
	
	frame.CmdBufLen=iDataLen;
	frame.i8DataId=bDataID+0x30;bDataID++;
	frame.i8TypeOfFrame=0x44;
	frame.pCmdBuf=ucTextBuf;

	iDataLen=encodeFrame(ucSendBuf,&frame);
	Com_Port_Send(hFile,ucSendBuf,iDataLen);
	memset(ucRecvBuf, 0 ,6);
	iALen=Com_Port_Recv(hFile,ucRecvBuf, 6, 2000);
	if(iALen>0&&((memcmp(ucRecvBuf, resp_data_recved, 4) == 0)||(memcmp(ucRecvBuf, resp_data_recved+1, 3) == 0)))
	{
	}	
	else
	{	
		return -5;
	}

		
	memset(ucRecvBuf, 0 ,8);
	memcpy(eResponse,resp_print_finished,5);
	eResponse[3] = ucSendBuf[2];

	iALen=Com_Port_Recv(hFile, ucRecvBuf, 7, 10000);
	if(iALen<=0||((memcmp(ucRecvBuf, eResponse, 5) != 0)&&(memcmp(ucRecvBuf, eResponse+1, 4)!=0)))
	{
		return -6;
	}
	return 0;
}
int p25_print_null_tail_text(HANDLE hFile,BYTE *ucTextBuf)
{
	return p25_print_text(hFile,ucTextBuf,(DWORD)strlen((char *)ucTextBuf));
}
#endif
