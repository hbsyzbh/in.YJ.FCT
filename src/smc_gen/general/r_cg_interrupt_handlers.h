/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_interrupt_handlers.h
* Version      : 1.1.102
* Device(s)    : R5F51306AxFK
* Description  : This file declares interrupt handlers.
* Creation Date: 2020-11-02
***********************************************************************************************************************/

#ifndef INTERRUPT_HANDLERS_H
#define INTERRUPT_HANDLERS_H

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
/* RSPI0 SPEI0 */
void r_SPI_error_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(RSPI0,SPEI0))));

/* RSPI0 SPRI0 */
void r_SPI_receive_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(RSPI0,SPRI0))));

/* RSPI0 SPTI0 */
void r_SPI_transmit_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(RSPI0,SPTI0))));

/* RSPI0 SPII0 */
void r_SPI_idle_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(RSPI0,SPII0))));

/* S12AD S12ADI0 */
void r_Config_S12AD0_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(S12AD,S12ADI0))));

/* SCI1 ERI1 */
void r_UART_receiveerror_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(SCI1,ERI1))));

/* SCI1 RXI1 */
void r_UART_receive_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(SCI1,RXI1))));

/* SCI1 TXI1 */
void r_UART_transmit_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(SCI1,TXI1))));

/* SCI1 TEI1 */
void r_UART_transmitend_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(SCI1,TEI1))));

/* SCI5 ERI5 */
void r_UART2_receiveerror_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(SCI5,ERI5))));

/* SCI5 RXI5 */
void r_UART2_receive_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(SCI5,RXI5))));

/* SCI5 TXI5 */
void r_UART2_transmit_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(SCI5,TXI5))));

/* SCI5 TEI5 */
void r_UART2_transmitend_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(SCI5,TEI5))));

/* SCI6 ERI6 */
void r_SPI2_receiveerror_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(SCI6,ERI6))));

/* SCI6 RXI6 */
void r_SPI2_receive_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(SCI6,RXI6))));

/* SCI6 TXI6 */
void r_SPI2_transmit_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(SCI6,TXI6))));

/* SCI6 TEI6 */
void r_SPI2_transmitend_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(SCI6,TEI6))));

/* SCI12 ERI12 */
void r_UART3_receiveerror_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(SCI12,ERI12))));

/* SCI12 RXI12 */
void r_UART3_receive_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(SCI12,RXI12))));

/* SCI12 TXI12 */
void r_UART3_transmit_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(SCI12,TXI12))));

/* SCI12 TEI12 */
void r_UART3_transmitend_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(SCI12,TEI12))));

/* RIIC0 EEI0 */
void r_Config_RIIC0_error_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(RIIC0,EEI0))));

/* RIIC0 RXI0 */
void r_Config_RIIC0_receive_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(RIIC0,RXI0))));

/* RIIC0 TXI0 */
void r_Config_RIIC0_transmit_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(RIIC0,TXI0))));

/* RIIC0 TEI0 */
void r_Config_RIIC0_transmitend_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(RIIC0,TEI0))));

/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif
