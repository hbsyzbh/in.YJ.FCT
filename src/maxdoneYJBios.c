/*
 * maxdoneYJBios.c
 *
 *  Created on: 2020年10月29日
 *      Author: Administrator
 */


#include "r_smc_entry.h"
#include "maxdoneYJBios.h"

void delay(unsigned char time)
{
	unsigned int delay = 0;

	while(time--) {
		delay = 50000;
		while(delay--);
	}
}

void setD8(unsigned char on)
{
	PORTH.PODR.BIT.B3 = on > 0 ? 1 : 0;
}

void setD9(unsigned char on)
{
	PORTH.PODR.BIT.B1 = on > 0 ? 1 : 0;
}

void setD10(unsigned char on)
{
	PORT5.PODR.BIT.B5 = on > 0 ? 1 : 0;
}

unsigned char SPI_INT_DONE_FLAG = 0;
void SPI_INT(unsigned char * const tx_buf, unsigned char tx_num, unsigned char * const rx_buf)
{
	 RSPI0.SPCR.BIT.SPE = 1U;

	 for(int i = 0; i < tx_num; i++)
	 {

		 RSPI0.SPDR.WORD.H = tx_buf[i];
		 while(RSPI0.SPSR.BIT.IDLNF);

		 rx_buf[i] = RSPI0.SPDR.WORD.H;
	 }

	 R_SPI_Stop();
}

void SPI_INT_Done(void)
{
	SPI_INT_DONE_FLAG = 1;
}

