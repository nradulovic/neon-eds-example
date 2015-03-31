/*
 * This file is part of Neon-test.
 *
 * Board support package source for: x86_64 Linux
 */

/*=========================================================  INCLUDE FILES  ==*/

#include <stdio.h>

#include "bsp.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===========================================  GLOBAL FUNCTION DEFINITIONS  ==*/

void bsp_init(void)
{
    /*
     * Do not need to initialise the BSP.
     */
}

void bsp_led_on(void)
{
    printf("LED on\n");
}

void bsp_led_off(void)
{
    printf("LED off\n");
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of bsp.c
 ******************************************************************************/
