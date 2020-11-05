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
* File Name    : Config_PORT.c
* Version      : 2.1.1
* Device(s)    : R5F51306AxFK
* Description  : This file implements device driver for Config_PORT.
* Creation Date: 2020-11-05
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
#include "Config_PORT.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_PORT_Create
* Description  : This function initializes the PORT
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_PORT_Create(void)
{
    /* Set PORT1 registers */
    PORT1.PODR.BYTE = _00_Pm4_OUTPUT_0 | _00_Pm5_OUTPUT_0;
    PORT1.ODR1.BYTE = _00_Pm4_CMOS_OUTPUT | _00_Pm5_CMOS_OUTPUT | _00_Pm6_CMOS_OUTPUT | _00_Pm7_CMOS_OUTPUT;
    PORT1.DSCR.BYTE = _00_Pm4_HIDRV_OFF | _00_Pm5_HIDRV_OFF;
    PORT1.PMR.BYTE = _00_Pm4_PIN_GPIO | _00_Pm5_PIN_GPIO;
    PORT1.PDR.BYTE = _10_Pm4_MODE_OUTPUT | _20_Pm5_MODE_OUTPUT | _0F_PDR1_DEFAULT;

    /* Set PORT3 registers */
    PORT3.PODR.BYTE = _00_Pm1_OUTPUT_0 | _04_Pm2_OUTPUT_1;
    PORT3.ODR0.BYTE = _00_Pm0_CMOS_OUTPUT | _00_Pm1_CMOS_OUTPUT | _00_Pm2_CMOS_OUTPUT;
    PORT3.ODR1.BYTE = _00_Pm6_CMOS_OUTPUT | _00_Pm7_CMOS_OUTPUT;
    PORT3.DSCR.BYTE = _00_Pm1_HIDRV_OFF | _00_Pm2_HIDRV_OFF;
    PORT3.PMR.BYTE = _00_Pm1_PIN_GPIO | _00_Pm2_PIN_GPIO;
    PORT3.PDR.BYTE = _02_Pm1_MODE_OUTPUT | _04_Pm2_MODE_OUTPUT | _18_PDR3_DEFAULT;

    /* Set PORT5 registers */
    PORT5.PODR.BYTE = _00_Pm5_OUTPUT_0;
    PORT5.PCR.BYTE = _00_Pm4_PULLUP_OFF;
    PORT5.DSCR.BYTE = _00_Pm5_HIDRV_OFF;
    PORT5.PMR.BYTE = _00_Pm4_PIN_GPIO | _00_Pm5_PIN_GPIO;
    PORT5.PDR.BYTE = _00_Pm4_MODE_INPUT | _20_Pm5_MODE_OUTPUT | _CF_PDR5_DEFAULT;

    /* Set PORTA registers */
    PORTA.PODR.BYTE = _00_Pm0_OUTPUT_0;
    PORTA.ODR0.BYTE = _00_Pm0_CMOS_OUTPUT | _00_Pm1_CMOS_OUTPUT | _00_Pm3_CMOS_OUTPUT;
    PORTA.ODR1.BYTE = _00_Pm6_CMOS_OUTPUT;
    PORTA.PCR.BYTE = _00_Pm4_PULLUP_OFF;
    PORTA.DSCR.BYTE = _00_Pm0_HIDRV_OFF;
    PORTA.PMR.BYTE = _00_Pm0_PIN_GPIO | _00_Pm4_PIN_GPIO;
    PORTA.PDR.BYTE = _01_Pm0_MODE_OUTPUT | _00_Pm4_MODE_INPUT | _A4_PDRA_DEFAULT;

    /* Set PORTB registers */
    PORT.PSRA.BYTE = _00_PORT_PSEL6_PB6 | _00_PORT_PSEL7_PB7;
    PORTB.PODR.BYTE = _00_Pm1_OUTPUT_0 | _08_Pm3_OUTPUT_1 | _00_Pm5_OUTPUT_0;
    PORTB.ODR0.BYTE = _00_Pm1_CMOS_OUTPUT | _00_Pm3_CMOS_OUTPUT;
    PORTB.PCR.BYTE = _00_Pm0_PULLUP_OFF | _00_Pm6_PULLUP_OFF | _00_Pm7_PULLUP_OFF;
    PORTB.DSCR.BYTE = _00_Pm1_HIDRV_OFF | _00_Pm3_HIDRV_OFF | _00_Pm5_HIDRV_OFF;
    PORTB.PMR.BYTE = _00_Pm0_PIN_GPIO | _00_Pm1_PIN_GPIO | _00_Pm3_PIN_GPIO | _00_Pm5_PIN_GPIO | _00_Pm6_PIN_GPIO | 
                     _00_Pm7_PIN_GPIO;
    PORTB.PDR.BYTE = _00_Pm0_MODE_INPUT | _02_Pm1_MODE_OUTPUT | _08_Pm3_MODE_OUTPUT | _20_Pm5_MODE_OUTPUT | 
                     _00_Pm6_MODE_INPUT | _00_Pm7_MODE_INPUT | _14_PDRB_DEFAULT;

    /* Set PORTH registers */
    PORTH.PODR.BYTE = _00_Pm1_OUTPUT_0 | _00_Pm3_OUTPUT_0;
    PORTH.DSCR.BYTE = _00_Pm1_HIDRV_OFF | _00_Pm3_HIDRV_OFF;
    PORTH.PMR.BYTE = _00_Pm1_PIN_GPIO | _00_Pm3_PIN_GPIO;
    PORTH.PDR.BYTE = _02_Pm1_MODE_OUTPUT | _08_Pm3_MODE_OUTPUT | _F0_PDRH_DEFAULT;

    R_Config_PORT_Create_UserInit();
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
