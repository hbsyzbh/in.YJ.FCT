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


void SPI_INT(unsigned char * const tx_buf, unsigned char tx_num, unsigned char * const rx_buf);

#endif /* MAXDONEYJBIOS_H_ */
