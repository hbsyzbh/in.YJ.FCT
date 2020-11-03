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
void SPI_COMMU(unsigned char * const tx_buf, unsigned char tx_num, unsigned char * const rx_buf, unsigned char channel)
{
	RSPI0.SPCMD0.BIT.SSLA = channel;
	RSPI0.SPCR.BIT.SPE = 1U;

	 for(int i = 0; i < tx_num; i++)
	 {

		 RSPI0.SPDR.WORD.H = tx_buf[i];
		 while(RSPI0.SPSR.BIT.IDLNF);

		 if(rx_buf)
			 rx_buf[i] = RSPI0.SPDR.WORD.H;
	 }

	 R_SPI_Stop();
}

void SPI_INT(unsigned char * const tx_buf, unsigned char tx_num, unsigned char * const rx_buf)
{
	SPI_COMMU(tx_buf, tx_num, rx_buf,  2);
	//SPI2_COM(tx_buf, rx_buf, tx_num );
}

void SPI_595(unsigned char * const tx_buf, unsigned char tx_num, unsigned char * const rx_buf)
{
	SPI_COMMU(tx_buf, tx_num, rx_buf,  0);
}

void Set_595(unsigned char Q595)
{
	unsigned char tx_buf[1];

	tx_buf[0] = Q595;
	SPI_595(tx_buf, 1, 0);
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

static unsigned char W25_readStatus(void)
{
	unsigned char cmd[2] = { 0x05};
	unsigned char ret[2] = { 0xFF, 0xFF};

	SPI_INT(cmd, 2, ret);

	return ret[1];
}

static void checkWriteDone(void)
{
	unsigned short times;

	for(times = 0; times < 5000; times++)
	{

		if((W25_readStatus() & 0x01) == 0x00)
			return;

		delay(50);
	}
}

void W25_Erase(void)
{
	unsigned char cmd[4] = {0x20, 0, 0, 0};
	unsigned char ret[4];

	W25_WriteEable();
	SPI_INT(cmd, 4, ret);
	checkWriteDone();
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
	checkWriteDone();
}

void checkW25JEDECID(void)
{
	unsigned char cmd[4] = {0x9f, 0x9f, 0x9f, 0x9f};
	unsigned char JedecID[4];

	SPI_INT(cmd, 4, JedecID);
}

void I2C_sendData(unsigned char data)
{
	while(! RIIC0.ICSR2.BIT.TDRE);
	RIIC0.ICDRT = data;
	while(! RIIC0.ICSR2.BIT.TEND);
}

void I2C_Start(void)
{
	RIIC0.ICSR2.BIT.START = 0U;
	RIIC0.ICCR2.BIT.ST = 1U;    /* Set start condition flag */
	while( ! RIIC0.ICSR2.BIT.START);
}

void I2C_reStart(void)
{
	RIIC0.ICSR2.BIT.START = 0U;
	RIIC0.ICCR2.BIT.RS = 1U;
	while( ! RIIC0.ICSR2.BIT.START);
}

void I2C_Stop(void)
{
	RIIC0.ICSR2.BIT.STOP = 0u;
	RIIC0.ICCR2.BIT.SP = 1U;    /* Set stop condition flag */
	while( ! RIIC0.ICSR2.BIT.STOP);
}

void I2C_StartRecvData(unsigned char slaveAddr)
{
	I2C_reStart();

	while(! RIIC0.ICSR2.BIT.TDRE);
	RIIC0.ICDRT = (slaveAddr |= 0x01);
}


void read_24AA02E48(unsigned int addr, unsigned char *buf, unsigned char len)
{
	int i;
	unsigned char dummy;
	I2C_Start();
	I2C_sendData(0xA0);
	I2C_sendData(addr);

	I2C_StartRecvData(0xA1);

	dummy = RIIC0.ICDRR;
	for(i = 0; i < len; i++)
	{
		while(! RIIC0.ICSR2.BIT.RDRF);
		buf[i] = RIIC0.ICDRR;
	}

	//多读一个 再停止，防止主循环逻辑过于复杂
	while(!RIIC0.ICSR2.BIT.RDRF);	//ICDRRレジスタに受信データあり
	RIIC0.ICSR2.BIT.STOP = 0;		//ストップコンディション未検出
	RIIC0.ICCR2.BIT.SP = 1;			//停止条件発行
	dummy = RIIC0.ICDRR;	//受信した最後のデータ格納

}

void write_24AA02E48(unsigned int addr, unsigned char *buf, unsigned char len)
{
	unsigned char i,j;
	unsigned char curRead;

	i = 0;
	for(; i < len;)
	{
		curRead = len - i;
		if(curRead > 8) curRead = 8;

		I2C_Start();
		I2C_sendData(0xA0);
		I2C_sendData(addr + i);

		for(j = 0; j < curRead; j++)
		{
			I2C_sendData(buf[i++]);
		}

		I2C_Stop();
		delay(100);
	}
}

void SetU16EN(unsigned char v)
{
	PORT3.PODR.BIT.B1 = v ? 1 : 0;
}

void SetSPI2CS(unsigned char v)
{
	PORT1.PODR.BIT.B5 = v ? 1 : 0;
}


void SPI2Delay(unsigned char time)
{
	unsigned char delay;

	while(time--)
	{
		delay = 10;
		while(delay--);
	}
}

void SetSPI2SCK(unsigned char v)
{
	PORTB.PODR.BIT.B3 = v ? 1 : 0;
}

void SetSPI2MOSI(unsigned char v)
{
	PORT3.PODR.BIT.B2 = v ? 1 : 0;
}

unsigned char getSPI2MISO(void)
{
	return PORTB.PIDR.BIT.B0;
}

void SPI2_SEND_REV(unsigned char t, unsigned char *r)
{
	int i;
	unsigned char buf;

	buf = 0;
	for(i = 0; i < 8; i++)
	{
		SetSPI2SCK(0);
		SetSPI2MOSI( t & (1 << (7 - i)));

		SPI2Delay(1);

		buf <<= 1;
		if(getSPI2MISO()) {
			buf |= 1;
		}

		SetSPI2SCK(1);
		SPI2Delay(1);

	}



	*r = buf;
}

void SPI2_COM(unsigned char *tx, unsigned char *rx, unsigned char len)
{
	unsigned char i;

	SetSPI2CS(1);
	SetSPI2MOSI(1);
	SetSPI2SCK(0);
	SetU16EN(0);
	delay(1);
	SetSPI2CS(0);

	for(i = 0; i < len; i++)
	{
		SPI2_SEND_REV(tx[i] , &rx[i]);
		SPI2Delay(10);
	}


	SetSPI2CS(1);
	SetSPI2MOSI(1);
	SetSPI2SCK(0);
	SetU16EN(1);
}

void checkSPI2JEDECID(void)
{
	unsigned char cmd[7] = {0x9f, 0x9f, 0x9f, 0x9f};
	unsigned char JedecID[7];

	for(;;)
	{
		checkW25JEDECID();
		SPI2_COM(cmd, JedecID, 4);
		delay(10);
	}
}

