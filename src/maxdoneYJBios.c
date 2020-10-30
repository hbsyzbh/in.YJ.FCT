/*
 * maxdoneYJBios.c
 *
 *  Created on: 2020年10月29日
 *      Author: Administrator
 */


#include "r_smc_entry.h"
#include "maxdoneYJBios.h"
#include "string.h"

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

static void W25_WriteEable(void)
{
	unsigned char cmd[1] = { 0x06};
	unsigned char ret[1] = { 0x06};

	SPI_INT(cmd, 1, ret);
}

void W25_Erase(void)
{
	unsigned char cmd[1] = { 0x60};
	unsigned char ret[1] = { 0x60};

	W25_WriteEable();
	SPI_INT(cmd, 1, ret);
}

void W25_read(unsigned int addr, unsigned char *buf, unsigned char len)
{
	unsigned char cmd[32] = { 0x0B, 0, 0, 0, 0xFF };
	unsigned char ret[32];

	if (len > (32 - 5))  len = 32 - 5;

	cmd[1] = addr / 0x10000 % 0x100;
	cmd[2] = addr / 0x100 % 0x100;
	cmd[3] = addr % 0x100;

	SPI_INT(cmd, 5 + len, ret);
	memcpy(buf, &ret[5], len);
}

void W25_write(unsigned int addr, unsigned char *buf, unsigned char len)
{
	unsigned char cmd[32] = { 0x02, 0, 0, 0 };
	unsigned char ret[32];

	if (len > (32 - 4))  len = 32 - 4;

	memcpy(&cmd[4], buf, len);

	cmd[1] = addr / 0x10000 % 0x100;
	cmd[2] = addr / 0x100 % 0x100;
	cmd[3] = addr % 0x100;


	W25_WriteEable();
	SPI_INT(cmd, 4 + len, ret);
}

void checkW25JEDECID(void)
{
	unsigned char cmd[4] = {0x9f, 0x9f, 0x9f, 0x9f};
	unsigned char JedecID[4];

	SPI_INT(cmd, 4, JedecID);
}

