/*
 * This file is part of Neon examples.
 */

/*=========================================================  INCLUDE FILES  ==*/

#include "neon_eds.h"

#include "bsp.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define PERIOD_PAUSE                    N_TIME_TO_TICK_MS(1000)
#define PERIOD_SHORT                    N_TIME_TO_TICK_MS(60)
#define PERIOD_DELAY                    N_TIME_TO_TICK_MS(180)
#define PERIOD_LONG                     N_TIME_TO_TICK_MS(180)

/*======================================================  LOCAL DATA TYPES  ==*/

struct hart_beat_workspace
{
    struct netimer              period;
};

enum hart_beat_local_events
{
    PERIOD_ELAPSED       = NEVENT_USER_ID
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static naction state_init (struct nsm *, const struct nevent *);
static naction state_pause(struct nsm *, const struct nevent *);
static naction state_short(struct nsm *, const struct nevent *);
static naction state_delay(struct nsm *, const struct nevent *);
static naction state_long (struct nsm *, const struct nevent *);

/*=======================================================  LOCAL VARIABLES  ==*/

static struct nepa              g_hart_beat_epa;
static struct nevent *          g_hart_beat_queue_storage[10];
static struct hart_beat_workspace  g_hart_beat_workspace;
static struct nheap             g_event_mem;
static uint8_t                  g_event_mem_storage[1024];

static const struct nepa_define g_hart_beat_define =
{
    .sm.wspace                  = &g_hart_beat_workspace,
    .sm.init_state              = &state_init,
    .sm.type                    = NSM_TYPE_FSM,
    .working_queue.storage      = g_hart_beat_queue_storage,
    .working_queue.size         = sizeof(g_hart_beat_queue_storage),
    .thread.priority            = 1,
    .thread.name                = "hart_beat"
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/


static naction state_init(struct nsm * sm, const struct nevent * event)
{
    struct hart_beat_workspace *   ws = sm->wspace;

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
    struct hart_beat_workspace *   ws = sm->wspace;

    switch (event->id) {
        case NSMP_ENTRY: {
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
    struct hart_beat_workspace *   ws = sm->wspace;

    switch (event->id) {
        case NSMP_ENTRY: {
            netimer_after(&ws->period, PERIOD_SHORT, PERIOD_ELAPSED);
            bsp_led_on();

            return (naction_handled());
        }
        case NSMP_EXIT: {
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
    struct hart_beat_workspace *   ws = sm->wspace;

    switch (event->id) {
        case NSMP_ENTRY: {
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
    struct hart_beat_workspace *   ws = sm->wspace;

    switch (event->id) {
        case NSMP_ENTRY: {
            netimer_after(&ws->period, PERIOD_LONG, PERIOD_ELAPSED);
            bsp_led_on();

            return (naction_handled());
        }
        case NSMP_EXIT: {
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
    bsp_init();

    ncore_init();
    ncore_timer_enable();
    nsched_init();

    nheap_init(&g_event_mem, g_event_mem_storage, sizeof(g_event_mem_storage));
    nevent_register_mem(&g_event_mem.mem_class);
    nepa_init(&g_hart_beat_epa, &g_hart_beat_define);
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
