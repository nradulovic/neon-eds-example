/*
 * This file is part of Neon-test.
 *
 * Board support package for: x86_64 Linux
 */

#ifndef BLINKY_FSM_X86_64_LINUX_BSP_H_
#define BLINKY_FSM_X86_64_LINUX_BSP_H_

/*=========================================================  INCLUDE FILES  ==*/
/*===============================================================  MACRO's  ==*/
/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

/*
 * Initialise the board support package
 */
void bsp_init(void);

/*
 * Turn on an user LED
 */
void bsp_led_on(void);

/*
 * Turn off an user LED
 */
void bsp_led_off(void);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of bsp.h
 ******************************************************************************/
#endif /* BLINKY_FSM_X86_64_LINUX_BSP_H_ */
