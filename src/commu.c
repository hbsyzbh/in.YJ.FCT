/*
 * commu.c
 *
 *  Created on: 2020年11月3日
 *      Author: Administrator
 */

/*
#include "iodefine.h"
#include "r_cg_macrodriver.h"
#include "r_cg_serial.h"
#include "r_cg_userdefine.h"
#include "r_cg_rtc.h"
#include "string.h"
*/

#include "r_smc_entry.h"
#include "maxdoneYJBios.h"

unsigned char g_i2c_done = 0;
unsigned char g_u2_sendend = 0;
unsigned char debug = 0;

static void R_UART2_Receive(unsigned char * revBuff, unsigned char len)
{
	R_UART_Serial_Receive(revBuff, len);
}

static void R_UART2_Send(unsigned char * const tx_buf, unsigned char tx_num)
{
	R_UART_Serial_Send(tx_buf, tx_num);
}

void UART2_Sendstr(char *str)
{
	if(str == 0) return;
	if(! debug) return;

	g_u2_sendend = 0;
	R_UART2_Send(str, strlen(str));
	for(; g_u2_sendend == 0;)
	{
		;//R_WDT_Restart();
	}
}

typedef enum {
	Cmd_getType,
	Cmd_syncRTC,
	Cmd_getRTC,
	Cmd_testSPIFlash,
	Cmd_testI2CPort,
	Cmd_testSPIPort 			= 5,
	Cmd_testU3Out,
	Cmd_testADC,
	Cmd_testLED,
	Cmd_testLight,
	Cmd_testStatus				= 10,

	Cmd_FCTVer = 0xF2,
	Cmd_enterRFPingPong = 0xF3,
	Cmd_initI2CPort = 0xF4
}	CMD_TYPE;

#define ACK_HEAD (0x4F)

#define ACK_CMD_POS		(1)
#define ACK_LEN_POS		(2)
#define ACK_DATA_POS	(3)

typedef enum {
	Uart_StartwaitHead,
	Uart_waittigHead,
	Uart_revHeadStartwaitlen,
	Uart_Waittinglen,
	Uart_revlenStartwaitData,
	Uart_waittingData,
	Uart_analysisData
}	UART_STATE;

UART_STATE UartState;

unsigned char cmd = 0;
unsigned char datalen = 0;

unsigned char revBuff[256] = {0};

static void startWaitForNewCmd()
{
	UartState = 0;
}

static char isCmdDatalenValied()
{
	switch(cmd) {
		case Cmd_syncRTC:
			return datalen == 6;
			break;

		case Cmd_testLED:
		case Cmd_testU3Out:
		case Cmd_testLight:
			return datalen == 1;
			break;

		default:
			return datalen == 0;
			break;
	}

	return 0;
}

const unsigned char FCTVerMajor = 0x01;
const unsigned char FCTVerSub = 0x00;

const unsigned char notsupport[] = {0x40, 0, 0};
const unsigned char hardwareType[9] = {ACK_HEAD, 0, 6, 2, 0, 1, 2, 4, 9};
unsigned char ackbuff[16] = {ACK_HEAD, 0};


static void fillRTCRespBuf(rtc_calendarcounter_value_t *p)
{
	ackbuff[ACK_DATA_POS+0] = p->ryrcnt		;
	ackbuff[ACK_DATA_POS+1] = p->rmoncnt	;
	ackbuff[ACK_DATA_POS+2] = p->rdaycnt	;
	ackbuff[ACK_DATA_POS+3] = p->rhrcnt		;
	ackbuff[ACK_DATA_POS+4] = p->rmincnt	;
	ackbuff[ACK_DATA_POS+5] = p->rseccnt	;
}

static void getRTCValueFormCmd(rtc_calendarcounter_value_t *p)
{
	p->ryrcnt	= revBuff[ACK_DATA_POS+0];
	p->rmoncnt	= revBuff[ACK_DATA_POS+1];
	p->rdaycnt	= revBuff[ACK_DATA_POS+2];
	p->rhrcnt	= revBuff[ACK_DATA_POS+3];
	p->rmincnt	= revBuff[ACK_DATA_POS+4];
	p->rseccnt	= revBuff[ACK_DATA_POS+5];
    //uint8_t rwkcnt;
}

static void analysisCmd()
{
	rtc_calendarcounter_value_t counter_write_val;

	ackbuff[ACK_CMD_POS] = cmd;
	ackbuff[ACK_LEN_POS] = 1;

	switch(cmd) {
	case Cmd_getType:
		R_UART2_Send(hardwareType, 9);
		break;
	case Cmd_syncRTC:

		getRTCValueFormCmd(&counter_write_val);
		R_Config_RTC_Restart(counter_write_val);
		fillRTCRespBuf(&counter_write_val);
		R_UART2_Send(ackbuff, 	ACK_DATA_POS+6);
		break;

	case Cmd_getRTC:

		ackbuff[ACK_LEN_POS] = 6;
		memset(&ackbuff[ACK_DATA_POS], 0, 6);

		R_Config_RTC_Get_CalendarCounterValue(&counter_write_val);

		fillRTCRespBuf(&counter_write_val);
		R_UART2_Send(ackbuff, 	ACK_DATA_POS+6);
		break;

	case Cmd_testSPIFlash:
		ackbuff[ACK_DATA_POS] = TestW25();
		R_UART2_Send(ackbuff, 	ACK_DATA_POS + 1);
		break;

	case Cmd_testI2CPort:
		ackbuff[ACK_DATA_POS] = TestE2P();
		R_UART2_Send(ackbuff, 	ACK_DATA_POS + 1);
		break;

	case Cmd_testSPIPort:
		break;

	case Cmd_testU3Out:
		break;

	case Cmd_testADC:
		break;

	case Cmd_testLED:
		//SetLed(revBuff[ACK_DATA_POS]);
		//ackbuff[ACK_DATA_POS] = revBuff[ACK_DATA_POS];
		R_UART2_Send(ackbuff, 	ACK_DATA_POS + 1);
		break;


	case Cmd_initI2CPort:
		ackbuff[ACK_DATA_POS] = InitE2P();
		R_UART2_Send(ackbuff, 	ACK_DATA_POS + 1);
		break;


	case Cmd_FCTVer:
		ackbuff[ACK_LEN_POS] = 2;
		ackbuff[ACK_DATA_POS + 0] = FCTVerMajor;
		ackbuff[ACK_DATA_POS + 1] = FCTVerSub;
		R_UART2_Send(ackbuff, 	ACK_DATA_POS + 2);
		break;


	default:
		R_UART2_Send(notsupport, 3);
		break;
	}
	startWaitForNewCmd();
}

void doUartTask()
{
	switch(UartState)
	{
		case Uart_StartwaitHead:
			R_UART2_Receive(revBuff, 1);
			UartState++;
			break;
		case Uart_revHeadStartwaitlen:
			R_UART2_Receive(&revBuff[1], 2);
			UartState++;
			break;
		case Uart_revlenStartwaitData:
			if(datalen > 0 ) {
				R_UART2_Receive(&revBuff[3], datalen);
				UartState++;
			}
			break;

		case Uart_analysisData:
			analysisCmd();
			break;

		default:
			break;
	}
}

void on_uart2_receiveend(void)
{
	switch(UartState)
	{
		case Uart_waittigHead:
			if((*revBuff) == 0xF4){
				UartState = Uart_revHeadStartwaitlen;
			} else {
				UartState = Uart_StartwaitHead;
			}
			break;

			break;
		case Uart_Waittinglen:
			cmd = revBuff[1];
			datalen = revBuff[2];

			if(isCmdDatalenValied())
			{
				if(datalen > 0) {
					UartState = Uart_revlenStartwaitData;
				} else {
					UartState = Uart_analysisData;
				}
			} else {
				UartState  = Uart_StartwaitHead;
			}
			break;
		case Uart_waittingData:
			UartState = Uart_analysisData;
			break;
	}
}

