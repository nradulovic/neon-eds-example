/*
 * This file is part of Neon EDS Example.
 *
 * Scheduler ping pong FSM
 */

/*=========================================================  INCLUDE FILES  ==*/

#include "neon_eds.h"
#include "bsp.h"
#include "sched_ping_pong.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/

/*-- Event storage and heap --------------------------------------------------*/
static struct nheap             g_event_heap;
static uint8_t                  g_event_heap_storage[1024];

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===========================================  GLOBAL FUNCTION DEFINITIONS  ==*/


int main(void)
{
    /* Initialise the Board Support Package for this example.
     * BSP should setup required clocks, power supply for MCU peripherals and
     * setup GPIO for driving a state LED.
     */
    bsp_init();

    /* Initialise the portable core. Portable core handles interrupts, timer
     * and CPU initialisation. After initialisation the core timer is started
     * so event timer can be used in this example.
     */
    ncore_init();
    ncore_timer_enable();

    /* Initialise internal scheduler data structures. Event Processing is
     * relaying on scheduler to provide efficient task switching.
     */
    nsched_init();

    /* Initialise a memory for events. Since events are dynamic they require
     * either a heap memory or a pool memory. Currently only Neon Heap and Pool
     * memory are supported. In the future a standard malloc/free support will
     * be added.
     *
     * Function nheap_init() requires a pointer to heap memory structure,
     * pointer to statically allocated storage and it's size.
     */
    nheap_init(&g_event_heap, g_event_heap_storage, sizeof(g_event_heap_storage));

    /* Register the Heap memory for events. New events will allocate memory from
     * this heap.
     */
    nevent_register_mem(&g_event_heap.mem_class);

    /* Register a user implementated IDLE routine. The idle routine is called
     * when there are no ready EPA for execution. Keep this routine short as
     * possible because it's execution time may impact system response time.
     *
     * When the NULL pointer is given the system will use default ncore_idle()
     * portable function which usually puts CPU to sleep.
     */
    neds_set_idle(NULL);

    /* Initialise the EPA.
     */
    nepa_init(&g_epa_heart_beat, &g_epa_define_heart_beat);

    /* Start the scheduler under Event Processing supervision.
     */
    neds_run();

    return (0);
}



PORT_C_NORETURN
void hook_at_assert(
    const struct ncomponent_info * component_info,
    const char *                fn,
    uint32_t                    line,
    const char *                expr,
    const char *                msg)
{
    (void)component_info;
    (void)fn;
    (void)line;
    (void)expr;
    (void)msg;

    for (;;);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//***************************************************************
 * END of hart_beat.c
 ******************************************************************************/
