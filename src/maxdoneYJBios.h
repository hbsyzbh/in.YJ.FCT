/*
 * maxdoneYJBios.h
 *
 *  Created on: 2020年10月29日
 *      Author: Administrator
 */

#ifndef MAXDONEYJBIOS_H_
#define MAXDONEYJBIOS_H_

void delay(unsigned char time);

void setD8(unsigned char on);
void setD9(unsigned char on);
void setD10(unsigned char on);

void checkW25JEDECID(void);
void checkSPI2JEDECID(void);

void W25_Erase(void);
void W25_read(unsigned int addr, unsigned char *buf, unsigned char len);
void W25_write(unsigned int addr, unsigned char *buf, unsigned char len);

void write_24AA02E48(unsigned int addr, unsigned char *buf, unsigned char len);
void read_24AA02E48(unsigned int addr, unsigned char *buf, unsigned char len);

void doUartTask(void);

#endif /* MAXDONEYJBIOS_H_ */
