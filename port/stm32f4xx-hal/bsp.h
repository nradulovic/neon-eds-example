/*
 * This file is part of Neon EDS Example.
 *
 * Board support package source for: stm32f4xx-hal
 */

#ifndef STM32F4XX_HAL_BSP_H_
#define STM32F4XX_HAL_BSP_H_

/*=========================================================  INCLUDE FILES  ==*/
/*===============================================================  MACRO's  ==*/
/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

void bsp_init(void);
void bsp_led_on(void);
void bsp_led_off(void);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of bsp.h
 ******************************************************************************/
#endif /* STM32F4XX_HAL_BSP_H_ */
