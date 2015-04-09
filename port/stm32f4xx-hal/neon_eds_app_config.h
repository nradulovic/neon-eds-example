/*
 * This file is part of Neon EDS Example.
 *
 * Configuration file for Neon EDS
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
