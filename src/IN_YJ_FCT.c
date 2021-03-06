/***********************************************************************
*
*  FILE        : IN_YJ_FCT.c
*  DATE        : 2020-10-22
*  DESCRIPTION : Main Program
*
*  NOTE:THIS IS A TYPICAL EXAMPLE.
*
***********************************************************************/
#include "r_smc_entry.h"
#include "Pin.h"
#include "maxdoneYJBios.h"


#define TST_LEN (10)
unsigned char TstStr[] = "MaxdoneFCT";
unsigned char TestW25(void)
{
	unsigned char buf[TST_LEN];
	int i;
#if 1

	W25_read(0, buf, TST_LEN);
	W25_Erase();
	W25_read(0, buf, TST_LEN);
	for(i = 0; i < TST_LEN; i++) {
		if(buf[i] != 0xFF){
			return ACK_NG;
		}
	}

	W25_write(0, TstStr, TST_LEN);
	W25_read(0, buf, TST_LEN);
	for(i = 0; i < TST_LEN; i++) {
		if(buf[i] != TstStr[i]) {
			return ACK_NG;
		}
	}

	W25_Erase();
#else
	W25_Erase();
	W25_write(0, "UUUUUUUUUU", TST_LEN);
	while(1)
	{
		W25_read(0, buf, TST_LEN);
		delay(1);
	}
#endif
	return ACK_OK;
}


unsigned char TestE2P(void)
{
	unsigned char buff[TST_LEN] = {0};

	read_24AA02E48(0, buff, TST_LEN);

	for(int i = 0; i < TST_LEN; i++) {
		if(buff[i] != TstStr[i]) {
			return ACK_NG;
		}
	}

	return ACK_OK;
}

unsigned char  InitE2P(void)
{
	write_24AA02E48(0, TstStr, TST_LEN);
	delay(1);
	return TestE2P();
}

unsigned char TestSPI_ext(void)
{
	//checkSPI2JEDECID();

	unsigned char cmd[4] = {0x9f, 0x9f, 0x9f, 0x9f};
	unsigned char JedecID[4] = {0};

	SPI2_COM(cmd, JedecID, 4);

	if( JedecID[1] == 0x1F ) {
		return ACK_OK;
	}

	return ACK_NG;
}

void debugFunc(void)
{
	//TestW25();
	//TestE2P();

	//while(1)
	//TestSPI_ext();

	while(1) {
		setD8(1);
		R_UART_Serial_Send("UART1\r\n", 7);
		delay(10);
		setD9(1);
		R_UART2_Serial_Send("UART2\r\n", 7);
		delay(10);
		setD10(1);
		R_UART3_Serial_Send("UART3\r\n", 7);
		delay(10);
		setD8(0);
		setD9(0);
		setD10(0);
		delay(10);
		checkW25JEDECID();
	}
}

void main(void)
{
	R_Systeminit();
	R_Pins_Create();

	R_Config_RTC_Start();
	R_UART_Start();
	R_UART2_Start();
	R_UART3_Start();

	//debugFunc();

	while(1)
		doUartTask();
}
