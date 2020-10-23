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
void main(void);

void main(void)
{
	R_Systeminit();
	R_Pins_Create();

	while(1);
}
