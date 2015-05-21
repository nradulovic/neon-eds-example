/*
 * This file is part of Neon EDS Example.
 *
 * The most basic blinky implementation.
 */

/*=========================================================  INCLUDE FILES  ==*/

#include "neon_eds.h"

#include "bsp.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define BLINKY_PERIOD                   NTIMER_MS(300)

/*======================================================  LOCAL DATA TYPES  ==*/

struct blinky_workspace
{
    struct netimer              period;
};

enum blinky_local_events
{
    BLINKY_PERIOD_ELAPSED       = NEVENT_USER_ID
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static naction state_init(struct nsm *, const struct nevent *);
static naction state_on  (struct nsm *, const struct nevent *);
static naction state_off (struct nsm *, const struct nevent *);

/*=======================================================  LOCAL VARIABLES  ==*/

static struct nepa              g_blinky_epa;
static struct nevent *          g_blinky_queue_storage[10];
static struct blinky_workspace  g_blinky_workspace;
static struct nheap             g_event_mem;
static uint8_t                  g_event_mem_storage[1024];

static const struct nepa_define g_blinky_define =
{
    .sm.wspace                  = &g_blinky_workspace,
    .sm.init_state              = &state_init,
    .sm.type                    = NSM_TYPE_FSM,
    .working_queue.storage      = g_blinky_queue_storage,
    .working_queue.size         = sizeof(g_blinky_queue_storage),
    .thread.priority            = 1,
    .thread.name                = "blinky"
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/


static naction state_init(struct nsm * sm, const struct nevent * event)
{
    struct blinky_workspace *   ws = nsm_wspace(sm);

    switch (event->id) {
        case NSM_INIT: {
            netimer_init(&ws->period);
            netimer_every(&ws->period, BLINKY_PERIOD, BLINKY_PERIOD_ELAPSED);

            return (naction_transit_to(sm, state_on));
        }
        default: {
            return (naction_ignored());
        }
    }
}



static naction state_on(struct nsm * sm, const struct nevent * event)
{
    struct blinky_workspace *   ws = nsm_wspace(sm);

    switch (event->id) {
        case NSM_ENTRY: {
            bsp_led_on();

            return (naction_handled());
        }
        case BLINKY_PERIOD_ELAPSED: {
            return (naction_transit_to(sm, state_off));
        }
        default: {
            return (naction_ignored());
        }
    }
}



static naction state_off(struct nsm * sm, const struct nevent * event)
{
    struct blinky_workspace *   ws = nsm_wspace(sm);

    switch (event->id) {
        case NSM_ENTRY: {
            bsp_led_off();

            return (naction_handled());
        }
        case BLINKY_PERIOD_ELAPSED: {
            return (naction_transit_to(sm, state_on));
        }
        default: {
            return (naction_ignored());
        }
    }
}

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
    nheap_init(&g_event_mem, g_event_mem_storage, sizeof(g_event_mem_storage));

    /* Register the Heap memory for events. New events will allocate memory from
     * this heap.
     */
    nevent_register_mem(&g_event_mem.mem_class);

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
    nepa_init(&g_blinky_epa, &g_blinky_define);
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
 * END of blinky.c
 ******************************************************************************/
