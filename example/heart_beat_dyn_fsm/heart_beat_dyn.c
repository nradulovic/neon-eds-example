/*
 * This file is part of Neon EDS Example.
 *
 * Heart Beat FSM (Dynamic EPA creation)
 */

/*=========================================================  INCLUDE FILES  ==*/

#include "neon_eds.h"

#include "bsp.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define PERIOD_PAUSE                    NTIMER_MS(1000)
#define PERIOD_SHORT                    NTIMER_MS(60)
#define PERIOD_DELAY                    NTIMER_MS(180)
#define PERIOD_LONG                     NTIMER_MS(180)

/*======================================================  LOCAL DATA TYPES  ==*/

/* This is Heart Beat workspace. The workspace is visible only to this EPA.
 *
 * When creating threads there is usually an API to supply initial arguments to
 * newly created thread. In this case if you need to pass some initialisation
 * data to the EPA this is done through an event.
 */
struct workspace
{
    struct netimer              period;
};

enum hart_beat_local_events
{
    PERIOD_ELAPSED              = NEVENT_USER_ID
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static naction state_init (struct nsm *, const struct nevent *);
static naction state_pause(struct nsm *, const struct nevent *);
static naction state_short(struct nsm *, const struct nevent *);
static naction state_delay(struct nsm *, const struct nevent *);
static naction state_long (struct nsm *, const struct nevent *);

/*=======================================================  LOCAL VARIABLES  ==*/

static struct nheap             g_event_mem;
static uint8_t                  g_event_mem_storage[1024];

static const struct nepa_define g_heart_beat_define =
{
    .sm.init_state              = &state_init,
    .sm.type                    = NSM_TYPE_FSM,
    .working_queue.size         = NEQUEUE_SIZEOF(10),
    .thread.priority            = 1,
    .thread.name                = "heart_beat"
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/


static naction state_init(struct nsm * sm, const struct nevent * event)
{
    struct workspace *          ws;

    switch (event->id) {
        case NSM_INIT: {
            void *              ws_storage;

            /* Create the workspace with same memory class as EPA was created.
             */
            ws_storage = nepa_create_storage(sizeof(struct workspace));

            if (!ws_storage) {
                /* Destroy itself since no workspace storage is available.
                 */
                nepa_destroy(nepa_get_current());

                return (naction_ignored());
            }
            /* Set the new workspace storage
             */
            nsm_set_wspace(sm, ws_storage);

            /* Get the new workspace
             */
            ws = nsm_wspace(sm);

            /* Initialise the event timer which will be used for various LED
             * timing.
             */
            netimer_init(&ws->period);

            return (naction_transit_to(sm, state_pause));
        }
        default: {
            return (naction_ignored());
        }
    }
}



static naction state_pause(struct nsm * sm, const struct nevent * event)
{
    struct workspace *          ws = nsm_wspace(sm);

    switch (event->id) {
        case NSM_ENTRY: {
            netimer_after(&ws->period, PERIOD_PAUSE, PERIOD_ELAPSED);

            return (naction_handled());
        }
        case PERIOD_ELAPSED: {
            return (naction_transit_to(sm, state_short));
        }
        default: {
            return (naction_ignored());
        }
    }
}



static naction state_short(struct nsm * sm, const struct nevent * event)
{
    struct workspace *          ws = nsm_wspace(sm);

    switch (event->id) {
        case NSM_ENTRY: {
            netimer_after(&ws->period, PERIOD_SHORT, PERIOD_ELAPSED);
            bsp_led_on();

            return (naction_handled());
        }
        case NSM_EXIT: {
            bsp_led_off();

            return (naction_handled());
        }
        case PERIOD_ELAPSED: {
            return (naction_transit_to(sm, state_delay));
        }
        default: {
            return (naction_ignored());
        }
    }
}



static naction state_delay(struct nsm * sm, const struct nevent * event)
{
    struct workspace *          ws = nsm_wspace(sm);

    switch (event->id) {
        case NSM_ENTRY: {
            netimer_after(&ws->period, PERIOD_DELAY, PERIOD_ELAPSED);

            return (naction_handled());
        }
        case PERIOD_ELAPSED: {
            return (naction_transit_to(sm, state_long));
        }
        default: {
            return (naction_ignored());
        }
    }
}



static naction state_long(struct nsm * sm, const struct nevent * event)
{
    struct workspace *          ws = nsm_wspace(sm);

    switch (event->id) {
        case NSM_ENTRY: {
            netimer_after(&ws->period, PERIOD_LONG, PERIOD_ELAPSED);
            bsp_led_on();

            return (naction_handled());
        }
        case NSM_EXIT: {
            bsp_led_off();

            return (naction_handled());
        }
        case PERIOD_ELAPSED: {
            return (naction_transit_to(sm, state_pause));
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

    /* Register a memory for events memory allocation. New events will allocate
     * memory from this heap.
     */
    nevent_register_mem(&g_event_mem.mem_class);

    /* Create an EPA. The function nepa_create() returns a pointer to newly
     * created EPA, but in this example we don't need it. This pointer can be
     * passed between EPA's in order to send events to each other.
     */
    nepa_create(&g_heart_beat_define, &g_event_mem.mem_class);

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
