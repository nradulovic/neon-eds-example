/*
 * This file is part of Neon EDS Example.
 *
 * Scheduler ping pong FSM
 */

/*=========================================================  INCLUDE FILES  ==*/

#include "sched_ping_pong.h"
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

/*-- Heart beat EPA ----------------------------------------------------------*/
static struct nevent *          g_heart_beat_queue_storage[10];
static struct workspace         g_heart_beat_workspace;

/*-- Timing EPA --------------------------------------------------------------*/
static struct nevent *          g_timing_queue_storage[10];
static struct workspace         g_timing_workspace;

static struct nepa              g_timing_epa;
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

struct nepa                     g_epa_heart_beat;
const struct nepa_define        g_epa_define_heart_beat =
{
    .sm.wspace                  = &g_heart_beat_workspace,
    .sm.init_state              = &state_heart_beat_init,
    .sm.type                    = NSM_TYPE_FSM,
    .working_queue.storage      = g_heart_beat_queue_storage,
    .working_queue.size         = sizeof(g_heart_beat_queue_storage),
    .thread.priority            = 1,
    .thread.name                = "heart_beat"
};

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

/*-- Heart beat SM states ----------------------------------------------------*/
static naction state_heart_beat_init(struct nsm * sm, const struct nevent * event)
{
    struct workspace *          ws = nsm_wspace(sm);

    switch (event->id) {
        case NSM_INIT: {
            nepa_init(&g_timing_epa, &g_timing_define);
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
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//***************************************************************
 * END of hart_beat.c
 ******************************************************************************/
