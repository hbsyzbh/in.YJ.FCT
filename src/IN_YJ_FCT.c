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

void main(void)
{
	R_Systeminit();
	R_Pins_Create();


	R_UART_Start();
	R_UART2_Start();
	R_UART3_Start();

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
	}
}
