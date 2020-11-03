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
#if 0

	W25_read(0, buf, TST_LEN);
	W25_Erase();
	W25_read(0, buf, TST_LEN);
	for(i = 0; i < TST_LEN; i++) {
		if(buf[i] != 0xFF){
			return 1;
		}
	}

	W25_write(0, TstStr, TST_LEN);
	W25_read(0, buf, TST_LEN);
	for(i = 0; i < TST_LEN; i++) {
		if(buf[i] != TstStr[i]) {
			return 1;
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
	return 0;
}

void TestE2P()
{
	unsigned char buff[TST_LEN] = {0};

	//write_24AA02E48(0, "1234567890", TST_LEN);
	//write_24AA02E48(0, "M", 1);

	while(1)
	{
		read_24AA02E48(0, buff, TST_LEN);
		delay(100);
	}
}

void TestSPI_ext(void)
{
	checkSPI2JEDECID();
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
