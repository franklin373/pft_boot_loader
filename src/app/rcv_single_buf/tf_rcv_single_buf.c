#include "global.h"
//#include "fifo/serlDrvInner.h"
#include "rcv_single_buf.h"
#include "protocol/P25_protocol.h"
#include "flash/flash_common.h"

//uint8 pkg_buf[SERL_RCV_BUF_SIZE];
Prn_Frame_t tCurFrame;
static int in_download_mode;
void tf_rcv_single_buf()
{
	int nRtn;
    static uint32	uiStartAddr;/*must be static, for program command need erase command's data*/
    uint32  uiFileSize;
	
        if(tUartBlock.i8UartValidCmd > 0)
        {
            if(tUartBlock.i8UartValidCmd)
            	tUartBlock.i8UartValidCmd = 0;
			nRtn=BlueBambooCmdDecodeAndCheck(tUartBlock.UartBuf,tUartBlock.uartBufOff,&tCurFrame);
			if(nRtn){
				return;
			}
            switch(tCurFrame.pFlagBuf[0])
            {
                case PRN_FRAME_ENQ:
                    in_download_mode=0;
                    myfputchar(PRN_DEV_WIRE,0x00);
                    myfputchar(PRN_DEV_WIRE,PRN_FRAME_SOF);
                    myfputchar(PRN_DEV_WIRE,PRN_FRAME_ACK);                         //ack to ENQ
                    myfputchar(PRN_DEV_WIRE,PRN_FRAME_EOF);
                    myfputchar(PRN_DEV_WIRE,0x0D);
                    myfputchar(PRN_DEV_WIRE,0x0A);
                    break;

                case PRN_FRAME_INQUIRY_STATUS:
                    myfputchar(PRN_DEV_WIRE,0x00);
                    myfputchar(PRN_DEV_WIRE,PRN_FRAME_SOF);
                    myfputchar(PRN_DEV_WIRE,PRN_FRAME_RESPONSE_STATUS);
                    myfputchar(PRN_DEV_WIRE,0x02);                                  //status be fixed to 0x02
                    myfputchar(PRN_DEV_WIRE,0x31);                                  //random number
                    myfputchar(PRN_DEV_WIRE,PRN_FRAME_EOF);
                    myfputchar(PRN_DEV_WIRE,0x0D);
                    myfputchar(PRN_DEV_WIRE,0x0A);
                    break;
#if 0 //to save space                   
                case PRN_FRAME_READ_SNVER:
                	LPC_UartInterruptCtrl(LPC_UART_1, 0);
		    	
					memset(SNBuf,0,80);
					ReadSNVer(pSNBuf,pSNBuf+0x20,pSNBuf+0x28);

	    			if(pSNBuf[0]>0x20 && pSNBuf[0]<0x7e)
	    			{
		    			uiRLen = CmdEncode(SNBuf,48,0x30);
	        			myfputchar(whichPort,0x00);
		        		myfprints(whichPort,SNBuf+3,uiRLen);
	                    myfputchar(whichPort,0x0D);
    		            myfputchar(whichPort,0x0A);
		    		}
		    		else
		    		{
	    	    		myfprints(whichPort,NAckStr,6);
	    			}
		            
        	        LPC_UartInterruptCtrl(LPC_UART_1, LPC_UART_REV_INTERRUPT_EN|LPC_UART_RX_INTERRUPT_EN);
            	    break;
#endif                    
				case PRN_FRAME_FLASH_ERASE:
				       if(!in_download_mode){
                                        myfputchar(PRN_DEV_WIRE,0x00);
                                        myfputchar(PRN_DEV_WIRE,PRN_FRAME_SOF);
                                        myfputchar(PRN_DEV_WIRE,PRN_FRAME_NACK);
                                        myfputchar(PRN_DEV_WIRE,PRN_FRAME_EOF);
                                        myfputchar(PRN_DEV_WIRE,0x0D);
                                        myfputchar(PRN_DEV_WIRE,0x0A);
				            break;
				       }
//					LPC_UartInterruptCtrl(LPC_UART_1, 0);
#if 0					
					tCurFrame.pCmdBuf = pUart_X_Block->pUartOutAddr;
					CmdDecode(tCurFrame.pCmdBuf,tCurFrame.CmdBufLen,&tCurFrame.i8EvenChk,&tCurFrame.i8OddChk);
					
                    tCurFrame.i8DataId = *pUart_X_Block->pUartOutAddr++;
                    tCurFrame.CmdBufLen = 1000 * (*pUart_X_Block->pUartOutAddr++ - 0x30);
                    tCurFrame.CmdBufLen += 100 * (*pUart_X_Block->pUartOutAddr++ - 0x30);
                    tCurFrame.CmdBufLen += 10 * (*pUart_X_Block->pUartOutAddr++ - 0x30);
                    tCurFrame.CmdBufLen += *pUart_X_Block->pUartOutAddr++ - 0x30;//?
                    tCurFrame.pCmdBuf = pUart_X_Block->pUartOutAddr;
                    
                    CalCheckSum(tCurFrame.pCmdBuf,tCurFrame.CmdBufLen, &i8CalEvenChk, &i8CalOddChk);
                    
                    if((tCurFrame.CmdBufLen < 9999) &&
                       (tCurFrame.i8EvenChk == i8CalEvenChk) &&
                       (tCurFrame.i8OddChk == i8CalOddChk))
#endif                       
                    {
                        myfputchar(PRN_DEV_WIRE,0x00);
                        myfputchar(PRN_DEV_WIRE,PRN_FRAME_SOF);
                        myfputchar(PRN_DEV_WIRE,PRN_FRAME_EOT);      //printer will send this frame after it receives the print data frame successfully
                        myfputchar(PRN_DEV_WIRE,PRN_FRAME_EOF);
                        myfputchar(PRN_DEV_WIRE,0x0D);
                        myfputchar(PRN_DEV_WIRE,0x0A);
#if 0                       
						memcpy(&uiStartAddr, tCurFrame.pCmdBuf, 4);
			        	StartSector = getSector(uiStartAddr);
						puiStartAddr = (uint16*)uiStartAddr;
						memcpy(&uiFileSize, tCurFrame.pCmdBuf+4, 4);
						puiEndAddr	 = puiStartAddr + uiFileSize/2+(uiFileSize%2);
                        #if 0
            			SectorNr = getSectorNr(StartSector, uiFileSize);
                        #else
                        SectorNr = getSector(uiStartAddr+uiFileSize-1/*last char to update*/)-StartSector+1;
                        #endif
#if 0						
            {
                static char tmp[200];
                
                sprintf(tmp,"StartSector,SectorNr(%d,%d)\r\n",StartSector,SectorNr);
                LPC_UartPrintf(1-whichPort,(uint8 *)tmp);
            }
#endif
            			if(StartSector >= 0)//not maybe download bootload, sector may be 0
			            {
			                IntEnable(Uart0Int, LPC_INT_DISABLE);
				            IntEnable(Uart1Int, LPC_INT_DISABLE);
				            for(i=StartSector; i<StartSector + SectorNr; i++)
				        	    EraseSector(i);
                            IntEnable(Uart0Int, LPC_INT_ENABLE);
				            IntEnable(Uart1Int, LPC_INT_ENABLE);
				        }
#else//TODO				
						memcpy(&uiStartAddr, tCurFrame.pFlagBuf+1, 4);
						memcpy(&uiFileSize, tCurFrame.pFlagBuf+5, 4);
						_DBG("recv sect<uiStartAddr,uiFileSize> ");
						_DBH32(uiStartAddr);
						_DBG(",");
						_DBH32(uiFileSize);
						_DBG("\r\n");
						ASSERT_MINE(0==flash_erase(uiStartAddr,uiFileSize));
#endif
                        myfputchar(PRN_DEV_WIRE,0x00);
                        myfputchar(PRN_DEV_WIRE,PRN_FRAME_SOF);
                        myfputchar(PRN_DEV_WIRE,PRN_FRAME_ETX);     //printer will send this frame after ti finishes all of the requested printing.
                        myfputchar(PRN_DEV_WIRE,tCurFrame.i8DataId);
                        myfputchar(PRN_DEV_WIRE,PRN_FRAME_EOF);
                        myfputchar(PRN_DEV_WIRE,0x0D);
                        myfputchar(PRN_DEV_WIRE,0x0A);
                    }
#if 0
                    else
                    {
                        myfputchar(whichPort,0x00);
                        myfputchar(whichPort,PRN_FRAME_SOF);
                        myfputchar(whichPort,PRN_FRAME_NACK);
                        myfputchar(whichPort,PRN_FRAME_EOF);
                        myfputchar(whichPort,0x0D);
                        myfputchar(whichPort,0x0A);
                    }
#endif					
//                    LPC_UartInterruptCtrl(LPC_UART_1, LPC_UART_REV_INTERRUPT_EN);
                    break;
                case PRN_FRAME_MODE_DOWNLOAD:
                    in_download_mode=1;
                    myfputchar(PRN_DEV_WIRE,0x00);
                    myfputchar(PRN_DEV_WIRE,PRN_FRAME_SOF);
                    myfputchar(PRN_DEV_WIRE,PRN_FRAME_ACK);                         //ack to ENQ
                    myfputchar(PRN_DEV_WIRE,PRN_FRAME_EOF);
                    myfputchar(PRN_DEV_WIRE,0x0D);
                    myfputchar(PRN_DEV_WIRE,0x0A);
                break;
                case PRN_FRAME_PRN_DATA:
				       if(!in_download_mode){
                                        myfputchar(PRN_DEV_WIRE,0x00);
                                        myfputchar(PRN_DEV_WIRE,PRN_FRAME_SOF);
                                        myfputchar(PRN_DEV_WIRE,PRN_FRAME_NACK);
                                        myfputchar(PRN_DEV_WIRE,PRN_FRAME_EOF);
                                        myfputchar(PRN_DEV_WIRE,0x0D);
                                        myfputchar(PRN_DEV_WIRE,0x0A);
				            break;
				       }
//                    LPC_UartInterruptCtrl(LPC_UART_1, 0);
#if 0                    
                    tCurFrame.pCmdBuf = pUart_X_Block->pUartOutAddr;
					uiRLen = CmdDecode(tCurFrame.pCmdBuf,tCurFrame.CmdBufLen,&tCurFrame.i8EvenChk,&tCurFrame.i8OddChk);
					
                    tCurFrame.i8DataId = *pUart_X_Block->pUartOutAddr++;
                    tCurFrame.CmdBufLen = 1000 * (*pUart_X_Block->pUartOutAddr++ - 0x30);
                    tCurFrame.CmdBufLen += 100 * (*pUart_X_Block->pUartOutAddr++ - 0x30);
                    tCurFrame.CmdBufLen += 10 * (*pUart_X_Block->pUartOutAddr++ - 0x30);
                    tCurFrame.CmdBufLen += *pUart_X_Block->pUartOutAddr++ - 0x30;	//?
                    tCurFrame.pCmdBuf = pUart_X_Block->pUartOutAddr;
                    
                    if(tCurFrame.CmdBufLen!=(uiRLen-7))
                    	break;
                    
                    CalCheckSum(tCurFrame.pCmdBuf,tCurFrame.CmdBufLen, &i8CalEvenChk, &i8CalOddChk);

                    if((tCurFrame.CmdBufLen < 9999) &&
                       (tCurFrame.i8EvenChk == i8CalEvenChk) &&
                       (tCurFrame.i8OddChk == i8CalOddChk))
#endif                       
                    {
                        myfputchar(PRN_DEV_WIRE,0x00);
                        myfputchar(PRN_DEV_WIRE,PRN_FRAME_SOF);
                        myfputchar(PRN_DEV_WIRE,PRN_FRAME_EOT);      //printer will send this frame after it receives the print data frame successfully
                        myfputchar(PRN_DEV_WIRE,PRN_FRAME_EOF);
                        myfputchar(PRN_DEV_WIRE,0x0D);
                        myfputchar(PRN_DEV_WIRE,0x0A);
#if 0
			            IntEnable(Uart0Int, LPC_INT_DISABLE);
				        IntEnable(Uart1Int, LPC_INT_DISABLE);
						for(i=0;i<tCurFrame.CmdBufLen;i+=2)
       					{
       						uiSend =  tCurFrame.pCmdBuf[i] + (tCurFrame.pCmdBuf[i+1]<<8);
       						Program1word(puiStartAddr++,uiSend);
        				}
                        IntEnable(Uart0Int, LPC_INT_ENABLE);
				        IntEnable(Uart1Int, LPC_INT_ENABLE);
#else					
						_DBG("will program<uiStartAddr,len> ");
						_DBH32(uiStartAddr);
						_DBG(",");
						_DBH32(tCurFrame.FlagBufLen-1);
						_DBG("\r\n");
						ASSERT_MINE(0==flash_program(uiStartAddr,tCurFrame.pFlagBuf+1,tCurFrame.FlagBufLen-1));
						uiStartAddr+=(tCurFrame.FlagBufLen-1);
#endif
                        myfputchar(PRN_DEV_WIRE,0x00);
                        myfputchar(PRN_DEV_WIRE,PRN_FRAME_SOF);
                        myfputchar(PRN_DEV_WIRE,PRN_FRAME_ETX);     //printer will send this frame after ti finishes all of the requested printing.
                        myfputchar(PRN_DEV_WIRE,tCurFrame.i8DataId);
                        myfputchar(PRN_DEV_WIRE,PRN_FRAME_EOF);
                        myfputchar(PRN_DEV_WIRE,0x0D);
                        myfputchar(PRN_DEV_WIRE,0x0A);
                    }
#if 0
                    else
                    {
                        myfputchar(whichPort,0x00);
                        myfputchar(whichPort,PRN_FRAME_SOF);
                        myfputchar(whichPort,PRN_FRAME_NACK);
                        myfputchar(whichPort,PRN_FRAME_EOF);
                        myfputchar(whichPort,0x0D);
                        myfputchar(whichPort,0x0A);
                    }
#endif					
//                    LPC_UartInterruptCtrl(LPC_UART_1, LPC_UART_REV_INTERRUPT_EN);
                    break;
				
				case PRN_FRAME_EOT:
				in_download_mode=0;
	  	    		myfputchar(PRN_DEV_WIRE,0x00);
                    myfputchar(PRN_DEV_WIRE,PRN_FRAME_SOF);
                    myfputchar(PRN_DEV_WIRE,PRN_FRAME_ACK);                         //ack to ENQ
                    myfputchar(PRN_DEV_WIRE,PRN_FRAME_EOF);
                    myfputchar(PRN_DEV_WIRE,0x0D);
                    myfputchar(PRN_DEV_WIRE,0x0A);
					break;
					
                default:
                    break;
            }
//            pUart_X_Block->pUartInAddr = Uart_X_Buf;
//            pUart_X_Block->pUartOutAddr = Uart_X_Buf;
            //memset(Uart1Buf,0,Uart1BufLen);
        }

}
