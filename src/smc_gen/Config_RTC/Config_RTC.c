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
* File Name    : Config_RTC.c
* Version      : 1.5.3
* Device(s)    : R5F51306AxFK
* Description  : This file implements device driver for Config_RTC.
* Creation Date: 2020-11-02
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
#include "Config_RTC.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_RTC_Create
* Description  : This function initializes the RTC module
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_RTC_Create(void)
{
    uint16_t w_count;
    uint32_t read_count;
    volatile uint32_t dummy;

    /* Disable ALM, PRD and CUP interrupts */
    IEN(RTC,ALM) = 0U;
    IEN(RTC,PRD) = 0U;
    IEN(RTC,CUP) = 0U;

    /* RTC cold start */
    if(0 == SYSTEM.RSTSR1.BIT.CWSF)
    {

        /* Set sub-clock oscillator */
        while (RTC.RCR3.BIT.RTCEN != 1U)
        {
            RTC.RCR3.BIT.RTCEN = 1U;
        }

        /* Wait for 6 sub-clock cycles */
        for (w_count = 0U; w_count < _0495_RTC_SUB_6_CYCLE_WAIT; w_count++)
        {
            nop();
        }

        /* Stop all counters */
        RTC.RCR2.BIT.START = 0U;
        while (RTC.RCR2.BIT.START != 0U)
        {
            /* Wait for stop all counters */
        }

        /* Select count mode */
        RTC.RCR2.BIT.CNTMD = 0U;
        while (RTC.RCR2.BIT.CNTMD != 0U)
        {
            /* Wait for the register modification to complete */
        } 

        /* Execute RTC software reset */
        RTC.RCR2.BIT.RESET = 1U;
        while (RTC.RCR2.BIT.RESET != 0U)
        {
            /* Wait for the register modification to complete */
        }

        /* Set time */
        RTC.RSECCNT.BYTE = _00_RTC_COUNT_SECOND_VALUE;
        RTC.RMINCNT.BYTE = _00_RTC_COUNT_MINUTE_VALUE;
        RTC.RHRCNT.BYTE = _00_RTC_COUNT_HOUR_VALUE ;
        RTC.RWKCNT.BYTE = _06_RTC_COUNT_WEEK_VALUE;
        RTC.RDAYCNT.BYTE = _01_RTC_COUNT_DAY_VALUE;
        RTC.RMONCNT.BYTE = _01_RTC_COUNT_MONTH_VALUE;
        RTC.RYRCNT.WORD = _0000_RTC_COUNT_YEAR_VALUE;

        /* Set control registers */
        dummy = _00_RTC_ALARM_INT_DISABLE | _00_RTC_PERIOD_INT_DISABLE | _00_RTC_PERIODIC_INT_PERIOD_DISABLE;
        RTC.RCR1.BYTE = dummy;
        while (dummy != RTC.RCR1.BYTE)
        {
            /* Wait for the register modification to complete */
        }
        RTC.RCR2.BYTE = _00_RTC_AUTO_ADJUSTMENT_DISABLE | _40_RTC_HOUR_MODE_24 |
                        (0x8FU & RTC.RCR2.BYTE);

        /* Perform 4 read operations after writing */
        for (read_count = 0U; read_count < _04_FOUR_READ_COUNT; read_count++)
        {
            dummy = RTC.RCR2.BYTE;
        }
    }

    /* Set control registers */
    dummy = RTC.RCR1.BYTE | _00_RTC_CARRY_INT_DISABLE;
    RTC.RCR1.BYTE = dummy;
    while (dummy != RTC.RCR1.BYTE)
    {
        /* Wait for the register modification to complete */
    }

    R_Config_RTC_Create_UserInit();

}

/***********************************************************************************************************************
* Function Name: R_Config_RTC_Start
* Description  : This function starts RTC counter
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_RTC_Start(void)
{

    /* Set the START bit to 1 */
    RTC.RCR2.BIT.START = 1U;
    while (RTC.RCR2.BIT.START != 1U)
    {
        /* Wait for the register modification to complete */
    }
}

/***********************************************************************************************************************
* Function Name: R_Config_RTC_Stop
* Description  : This function stops RTC counter
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_RTC_Stop(void)
{

    /* Stop all counters */
    RTC.RCR2.BIT.START = 0U;
    while (RTC.RCR2.BIT.START != 0U)
    {
        /* Wait for the register modification to complete */
    }
}

/***********************************************************************************************************************
* Function Name: R_Config_RTC_Get_CalendarCounterValue
* Description  : This function get RTC calendar counter value
* Arguments    : counter_read_val -
*                    counter read pointer
* Return Value : None
***********************************************************************************************************************/


void R_Config_RTC_Get_CalendarCounterValue(rtc_calendarcounter_value_t * const counter_read_val)
{

    /* Enable RTC CUP interrupt */
    RTC.RCR1.BYTE |= _02_RTC_CARRY_INT_ENABLE;

    do
    {
        /* Clear IR flag of CUP interrupt */
        IR(RTC, CUP) = 0U;

        counter_read_val->rseccnt = RTC.RSECCNT.BYTE;
        counter_read_val->rmincnt = RTC.RMINCNT.BYTE;
        counter_read_val->rhrcnt = RTC.RHRCNT.BYTE;
        counter_read_val->rwkcnt = RTC.RWKCNT.BYTE;
        counter_read_val->rdaycnt = RTC.RDAYCNT.BYTE;
        counter_read_val->rmoncnt = RTC.RMONCNT.BYTE;
        counter_read_val->ryrcnt = RTC.RYRCNT.WORD;
    } while (IR(RTC,CUP) == 1U);

    /* Disable RTC CUP interrupt */
    RTC.RCR1.BYTE &= (~_02_RTC_CARRY_INT_ENABLE);

}

/***********************************************************************************************************************
* Function Name: R_Config_RTC_Set_CalendarCounterValue
* Description  : This function get RTC calendar counter value
* Arguments    : counter_write_val -
*                    counter write value
* Return Value : None
***********************************************************************************************************************/

void R_Config_RTC_Set_CalendarCounterValue(rtc_calendarcounter_value_t counter_write_val)
{
    uint32_t read_count;
    volatile uint32_t dummy;
    /* Stop all counters */
    RTC.RCR2.BIT.START = 0U;
    while (RTC.RCR2.BIT.START != 0U)
    {
        /* Wait for the register modification to complete */
    }

    /* Execute RTC software reset */
    RTC.RCR2.BIT.RESET = 1U;
    while (RTC.RCR2.BIT.RESET != 0U)
    {
        /* Wait for the register modification to complete */
    }
    /* Set control registers */
    RTC.RCR2.BYTE |= (_00_RTC_RTCOUT_OUTPUT_DISABLE | _00_RTC_AUTO_ADJUSTMENT_DISABLE | _40_RTC_HOUR_MODE_24);
    /* Perform 4 read operations after writing */
    for (read_count = 0U; read_count < _04_FOUR_READ_COUNT; read_count++)
    {
        dummy = RTC.RCR2.BYTE;
    }

    /* Set counter values */
    RTC.RSECCNT.BYTE = counter_write_val.rseccnt;
    RTC.RMINCNT.BYTE = counter_write_val.rmincnt;
    RTC.RHRCNT.BYTE = counter_write_val.rhrcnt;
    RTC.RWKCNT.BYTE = counter_write_val.rwkcnt;
    RTC.RDAYCNT.BYTE = counter_write_val.rdaycnt;
    RTC.RMONCNT.BYTE = counter_write_val.rmoncnt;
    RTC.RYRCNT.WORD = counter_write_val.ryrcnt;

}

/***********************************************************************************************************************
* Function Name: R_Config_RTC_Restart
* Description  : This function restarts RTC counter
* Arguments    : counter_write_val -
*                    counter write value
* Return Value : None
***********************************************************************************************************************/

void R_Config_RTC_Restart(rtc_calendarcounter_value_t counter_write_val)
{
    uint32_t read_count;
    volatile uint32_t dummy;
    /* Disable ALM, PRD and CUP interrupts */
    IEN(RTC, ALM) = 0U;
    IEN(RTC, PRD) = 0U;
    IEN(RTC, CUP) = 0U;


    /* Stop all counters */
    RTC.RCR2.BIT.START = 0U;
    while (RTC.RCR2.BIT.START != 0U)
    {
        /* Wait for the register modification to complete */
    }

    /* Execute RTC software reset */
    RTC.RCR2.BIT.RESET = 1U;
    while (RTC.RCR2.BIT.RESET != 0U)
    {
        /* Wait for the register modification to complete */
    }

    /* Set control registers */
    RTC.RCR2.BYTE |= (_00_RTC_RTCOUT_OUTPUT_DISABLE | _00_RTC_AUTO_ADJUSTMENT_DISABLE | _40_RTC_HOUR_MODE_24);

    /* Perform 4 read operations after writing */
    for (read_count = 0U; read_count < _04_FOUR_READ_COUNT; read_count++)
    {
        dummy = RTC.RCR2.BYTE;
    }

    /* Set counter values */
    RTC.RSECCNT.BYTE = counter_write_val.rseccnt;
    RTC.RMINCNT.BYTE = counter_write_val.rmincnt;
    RTC.RHRCNT.BYTE = counter_write_val.rhrcnt;
    RTC.RWKCNT.BYTE = counter_write_val.rwkcnt;
    RTC.RDAYCNT.BYTE = counter_write_val.rdaycnt;
    RTC.RMONCNT.BYTE = counter_write_val.rmoncnt;
    RTC.RYRCNT.WORD = counter_write_val.ryrcnt;
    /* Set the START bit to 1 */
    RTC.RCR2.BIT.START = 1U;
    while (RTC.RCR2.BIT.START != 1U)
    {
        /* Wait for the register modification to complete */
    }

}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

