/*
 * This file is part of neon-test.
 *
 * Copyright (C) 2010 - 2015 nenad
 *
 * neon-test is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * neon-test is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with neon-test.  If not, see <http://www.gnu.org/licenses/>.
 *
 * web site:    
 * e-mail  :    
 */

#ifndef NEON_EDS_APP_CONFIG_H_
#define NEON_EDS_APP_CONFIG_H_

/*=========================================================  INCLUDE FILES  ==*/
/*===============================================================  MACRO's  ==*/

# define CONFIG_DEBUG                   1
# define CONFIG_API_VALIDATION          1
# define CONFIG_ASSERT_INTERNAL         1
# define CONFIG_CORE_LOCK_MAX_LEVEL     255u
# define CONFIG_CORE_TIMER_SOURCE       3
#if defined(STM32F429xx)
# define CONFIG_CORE_TIMER_CLOCK_FREQ   (SystemCoreClock / 4)
#elif defined(STM32F411xE)
# define CONFIG_CORE_TIMER_CLOCK_FREQ   (SystemCoreClock / 2)
#endif
# define CONFIG_CORE_TIMER_EVENT_FREQ   100ul
# define CONFIG_PRIORITY_LEVELS         32u
# define CONFIG_PRIORITY_BUCKETS        32u
# define CONFIG_REGISTRY                1u
# define CONFIG_REGISTRY_NAME_SIZE      16u
# define CONFIG_EVENT_STRUCT_ATTRIBUTE
# define CONFIG_EVENT_SIZE              1
# define CONFIG_EVENT_PRODUCER          1
# define CONFIG_EVENT_STORAGE_NPOOLS    2
# define CONFIG_SMP_HSM                 1
# define CONFIG_GPIO                    0
# define CONFIG_UART                    0
# define CONFIG_I2C                     0
# define CONFIG_SPI                     0

/*-------------------------------------------------------  C++ extern base  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/
/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of neon_eds_app_config.h
 ******************************************************************************/
#endif /* NEON_EDS_APP_CONFIG_H_ */
