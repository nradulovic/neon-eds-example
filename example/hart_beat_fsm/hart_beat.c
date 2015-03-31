/*
 * This file is part of Neon examples.
 */

/*=========================================================  INCLUDE FILES  ==*/

#include "port/core.h"
#include "sched/sched.h"
#include "mm/heap.h"
#include "eds/etimer.h"
#include "eds/event.h"
#include "eds/smp.h"
#include "eds/epa.h"

#include "bsp.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define PERIOD_PAUSE                    N_TIME_TO_TICK_MS(450)
#define PERIOD_SHORT                    N_TIME_TO_TICK_MS(30)
#define PERIOD_DELAY                    N_TIME_TO_TICK_MS(60)
#define PERIOD_LONG                     N_TIME_TO_TICK_MS(60)

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

static naction state_init (struct nsm *, const struct nevent *);
static naction state_pause(struct nsm *, const struct nevent *);
static naction state_short(struct nsm *, const struct nevent *);
static naction state_delay(struct nsm *, const struct nevent *);
static naction state_long (struct nsm *, const struct nevent *);

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
    struct blinky_workspace *   ws = sm->wspace;

    switch (event->id) {
        case NSMP_INIT: {
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
    struct blinky_workspace *   ws = sm->wspace;

    switch (event->id) {
        case NSMP_ENTRY: {
            netimer_after(&ws->period, PERIOD_PAUSE, BLINKY_PERIOD_ELAPSED);

            return (naction_handled());
        }
        case BLINKY_PERIOD_ELAPSED: {
            return (naction_transit_to(sm, state_short));
        }
        default: {
            return (naction_ignored());
        }
    }
}



static naction state_short(struct nsm * sm, const struct nevent * event)
{
    struct blinky_workspace *   ws = sm->wspace;

    switch (event->id) {
        case NSMP_ENTRY: {
            netimer_after(&ws->period, PERIOD_SHORT, BLINKY_PERIOD_ELAPSED);
            bsp_led_on();

            return (naction_handled());
        }
        case NSMP_EXIT: {
            bsp_led_off();

            return (naction_handled());
        }
        case BLINKY_PERIOD_ELAPSED: {
            return (naction_transit_to(sm, state_delay));
        }
        default: {
            return (naction_ignored());
        }
    }
}



static naction state_delay(struct nsm * sm, const struct nevent * event)
{
    struct blinky_workspace *   ws = sm->wspace;

    switch (event->id) {
        case NSMP_ENTRY: {
            netimer_after(&ws->period, PERIOD_DELAY, BLINKY_PERIOD_ELAPSED);

            return (naction_handled());
        }
        case BLINKY_PERIOD_ELAPSED: {
            return (naction_transit_to(sm, state_long));
        }
        default: {
            return (naction_ignored());
        }
    }
}



static naction state_long(struct nsm * sm, const struct nevent * event)
{
    struct blinky_workspace *   ws = sm->wspace;

    switch (event->id) {
        case NSMP_ENTRY: {
            netimer_after(&ws->period, PERIOD_LONG, BLINKY_PERIOD_ELAPSED);
            bsp_led_on();

            return (naction_handled());
        }
        case NSMP_EXIT: {
            bsp_led_off();

            return (naction_handled());
        }
        case BLINKY_PERIOD_ELAPSED: {
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
    bsp_init();

    ncore_init();
    ncore_timer_enable();
    nsched_init();

    nheap_init(&g_event_mem, g_event_mem_storage, sizeof(g_event_mem_storage));
    nevent_register_mem(&g_event_mem.mem_class);
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
