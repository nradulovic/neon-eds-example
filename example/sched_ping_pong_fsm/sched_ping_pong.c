/*
 * This file is part of Neon EDS Example.
 *
 * Scheduler ping pong FSM
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

struct workspace
{
    struct netimer              period;
};

enum hart_beat_local_events
{
    PERIOD_ELAPSED              = NEVENT_LOCAL_ID,
	BLINK
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static naction state_heart_beat_init (struct nsm *, const struct nevent *);
static naction state_heart_beat_pause(struct nsm *, const struct nevent *);
static naction state_heart_beat_short(struct nsm *, const struct nevent *);
static naction state_heart_beat_delay(struct nsm *, const struct nevent *);
static naction state_heart_beat_long (struct nsm *, const struct nevent *);

static naction state_timing_init     (struct nsm *, const struct nevent *);
static naction state_timing_on       (struct nsm *, const struct nevent *);
static naction state_timing_off      (struct nsm *, const struct nevent *);

/*=======================================================  LOCAL VARIABLES  ==*/

/*-- Event storage and heap --------------------------------------------------*/
static struct nheap             g_event_mem;
static uint8_t                  g_event_mem_storage[1024];

/*-- Heart beat EPA ----------------------------------------------------------*/
static struct nepa              g_heart_beat_epa;
static struct nevent *          g_heart_beat_queue_storage[10];
static struct workspace         g_heart_beat_workspace;

static const struct nepa_define g_heart_beat_define =
{
    .sm.wspace                  = &g_heart_beat_workspace,
    .sm.init_state              = &state_heart_beat_init,
    .sm.type                    = NSM_TYPE_FSM,
    .working_queue.storage      = g_heart_beat_queue_storage,
    .working_queue.size         = sizeof(g_heart_beat_queue_storage),
    .thread.priority            = 1,
    .thread.name                = "heart_beat"
};

/*-- Timing EPA --------------------------------------------------------------*/
static struct nepa              g_timing_epa;
static struct nevent *          g_timing_queue_storage[10];
static struct workspace         g_timing_workspace;

static const struct nepa_define g_timing_define =
{
    .sm.wspace                  = &g_timing_workspace,
    .sm.init_state              = &state_timing_init,
    .sm.type                    = NSM_TYPE_FSM,
    .working_queue.storage      = g_timing_queue_storage,
    .working_queue.size         = sizeof(g_timing_queue_storage),
    .thread.priority            = 1,
    .thread.name                = "timing"
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

/*-- Heart beat SM states ----------------------------------------------------*/
static naction state_heart_beat_init(struct nsm * sm, const struct nevent * event)
{
    struct workspace *          ws = nsm_wspace(sm);

    switch (event->id) {
        case NSM_INIT: {
            netimer_init(&ws->period);

            return (naction_transit_to(sm, state_heart_beat_pause));
        }
        default: {
            return (naction_ignored());
        }
    }
}



static naction state_heart_beat_pause(struct nsm * sm, const struct nevent * event)
{
    switch (event->id) {
        case BLINK: {
            return (naction_transit_to(sm, state_heart_beat_short));
        }
        default: {
            return (naction_ignored());
        }
    }
}



static naction state_heart_beat_short(struct nsm * sm, const struct nevent * event)
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
            return (naction_transit_to(sm, state_heart_beat_delay));
        }
        default: {
            return (naction_ignored());
        }
    }
}



static naction state_heart_beat_delay(struct nsm * sm, const struct nevent * event)
{
    struct workspace *          ws = nsm_wspace(sm);

    switch (event->id) {
        case NSM_ENTRY: {
            netimer_after(&ws->period, PERIOD_DELAY, PERIOD_ELAPSED);

            return (naction_handled());
        }
        case PERIOD_ELAPSED: {
            return (naction_transit_to(sm, state_heart_beat_long));
        }
        default: {
            return (naction_ignored());
        }
    }
}



static naction state_heart_beat_long(struct nsm * sm, const struct nevent * event)
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
            return (naction_transit_to(sm, state_heart_beat_pause));
        }
        default: {
            return (naction_ignored());
        }
    }
}


/*-- Timing SM states --------------------------------------------------------*/
static naction state_timing_init(struct nsm * sm, const struct nevent * event)
{
	struct workspace *			ws = nsm_wspace(sm);

	switch (event->id) {
		case NSM_INIT: {
			netimer_init(&ws->period);

			return (naction_transit_to(sm, state_timing_on));
		}
		default: {

			return (naction_ignored());
		}
	}
}



static naction state_timing_on(struct nsm * sm, const struct nevent * event)
{
	struct workspace *			ws = nsm_wspace(sm);

	switch (event->id) {
		case NSM_ENTRY: {
			netimer_after(&ws->period, PERIOD_PAUSE, PERIOD_ELAPSED);

			return (naction_handled());
		}
		case PERIOD_ELAPSED: {
			struct nevent * 	signal;

			signal = nevent_create(sizeof(*signal), BLINK);

			if (signal) {
				nepa_send_event(&g_heart_beat_epa, signal);
			}

			return (naction_transit_to(sm, state_timing_off));
		}
		default: {

			return (naction_ignored());
		}
	}
}



static naction state_timing_off(struct nsm * sm, const struct nevent * event)
{
	struct workspace *			ws = nsm_wspace(sm);

	switch (event->id) {
		case NSM_ENTRY: {
			netimer_after(&ws->period, PERIOD_PAUSE, PERIOD_ELAPSED);

			return (naction_handled());
		}
		case PERIOD_ELAPSED: {
			struct nevent * 	signal;

			signal = nevent_create(sizeof(*signal), BLINK);

			if (signal) {
				nepa_send_event(&g_heart_beat_epa, signal);
			}

			return (naction_transit_to(sm, state_timing_on));
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
    nepa_init(&g_heart_beat_epa, &g_heart_beat_define);
    nepa_init(&g_timing_epa, &g_timing_define);

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
