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
* File Name    : SPI2.c
* Version      : 1.9.1
* Device(s)    : R5F51306AxFK
* Description  : This file implements device driver for SPI2.
* Creation Date: 2020-10-23
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "SPI2.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
volatile uint8_t * gp_sci6_tx_address;               /* SCI6 transmit buffer address */
volatile uint16_t  g_sci6_tx_count;                  /* SCI6 transmit data number */
volatile uint8_t * gp_sci6_rx_address;               /* SCI6 receive buffer address */
volatile uint16_t  g_sci6_rx_count;                  /* SCI6 receive data number */
volatile uint16_t  g_sci6_rx_length;                 /* SCI6 receive data length */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_SPI2_Create
* Description  : This function initializes SCI6
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_SPI2_Create(void)
{
    /* Cancel SCI6 module stop state */
    MSTP(SCI6) = 0U;

    /* Set interrupt priority */
    IPR(SCI6,TXI6) = _0F_SCI_PRIORITY_LEVEL15;

    /* Clear the control register */
    SCI6.SCR.BYTE = 0x00U;

    /* Set clock enable */
    SCI6.SCR.BYTE |= _01_SCI_INTERNAL_SCK_OUTPUT;

    /* Clear the SIMR1.IICM */
    SCI6.SIMR1.BIT.IICM = 0U;

    /* Set control registers */
    SCI6.SPMR.BYTE = _00_SCI_SS_PIN_DISABLE | _00_SCI_SPI_MASTER | _00_SCI_CLOCK_NOT_INVERTED | 
                     _00_SCI_CLOCK_NOT_DELAYED;
    SCI6.SMR.BYTE = _80_SCI_CLOCK_SYNCHRONOUS_OR_SPI_MODE | _00_SCI_CLOCK_PCLK;
    SCI6.SCMR.BYTE = _00_SCI_SERIAL_MODE | _00_SCI_DATA_INVERT_NONE | _00_SCI_DATA_LSB_FIRST | 
                     _10_SCI_DATA_LENGTH_8_OR_7 | _62_SCI_SCMR_DEFAULT;
    SCI6.SEMR.BYTE = _00_SCI_BIT_MODULATION_DISABLE;

    /* Set bit rate */
    SCI6.BRR = 0x07U;

    /* Set SMISO6 pin */
    MPC.PB0PFS.BYTE = 0x0BU;
    PORTB.PMR.BYTE |= 0x01U;

    /* Set SMOSI6 pin */
    MPC.P32PFS.BYTE = 0x0AU;
    PORT3.PMR.BYTE |= 0x04U;

    /* Set SCK6 pin */
    MPC.PB3PFS.BYTE = 0x0BU;
    PORTB.PMR.BYTE |= 0x08U;

    R_SPI2_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_SPI2_Start
* Description  : This function starts SCI6
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_SPI2_Start(void)
{
    /* Enable TXI and TEI interrupt */
    IR(SCI6,TXI6) = 0U;
    IEN(SCI6,TXI6) = 1U;
    IEN(SCI6,TEI6) = 1U;

    /* Enable RXI interrupt */
    IR(SCI6,RXI6) = 0U;
    IEN(SCI6,RXI6) = 1U;

    /* Enable ERI interrupt */
    IEN(SCI6,ERI6) = 1U;
}

/***********************************************************************************************************************
* Function Name: R_SPI2_Stop
* Description  : This function stops SCI6
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_SPI2_Stop(void)
{
    /* Set SMOSI6 pin */
    PORT3.PMR.BYTE &= 0xFBU;

    /* Disable serial transmit and receive */
    SCI6.SCR.BYTE &= 0xCFU;

    /* Disable TXI and TEI interrupt */
    IEN(SCI6,TXI6) = 0U;
    IEN(SCI6,TEI6) = 0U;

    /* Disable RXI interrupt */
    IEN(SCI6,RXI6) = 0U;

    /* Disable ERI interrupt */
    IEN(SCI6,ERI6) = 0U;

    /* Clear interrupt flags */
    IR(SCI6,TXI6) = 0U;
    IR(SCI6,RXI6) = 0U;
}

/***********************************************************************************************************************
* Function Name: R_SPI2_SPI_Master_Send_Receive
* Description  : This function sends and receives SCI6 data to and from slave device
* Arguments    : tx_buf -
*                    transfer buffer pointer (not used when data is handled by DTC)
*                tx_num -
*                    transfer buffer size
*                rx_buf -
*                    receive buffer pointer (not used when data is handled by DTC)
*                rx_num -
*                    receive buffer size
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/

MD_STATUS R_SPI2_SPI_Master_Send_Receive(uint8_t * const tx_buf, uint16_t tx_num, uint8_t * const rx_buf, uint16_t rx_num)
{
    MD_STATUS status = MD_OK;

    if (1U > tx_num)
    {
        status = MD_ARGERROR;
    }
    else
    {
        g_sci6_tx_count = tx_num;
        gp_sci6_tx_address = tx_buf;
        gp_sci6_rx_address = rx_buf;
        g_sci6_rx_count = 0U;
        g_sci6_rx_length = rx_num;

        /* Set SMOSI6 pin */
        PORT3.PMR.BYTE |= 0x04U;

        /* Set TE, TIE, RE, RIE bits simultaneously */
        SCI6.SCR.BYTE |= 0xF0U;
    }

    return (status);
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

