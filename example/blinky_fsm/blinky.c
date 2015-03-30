/*
 * This file is part of Neon.
 *
 * Copyright (C) 2010 - 2015 Nenad Radulovic
 *
 */

/*=========================================================  INCLUDE FILES  ==*/

#include "base/port/core.h"
#include "kernel/sched.h"
#include "kernel/mm/mem.h"
#include "kernel/mm/heap.h"
#include "eds/etimer.h"
#include "eds/event.h"
#include "eds/smp.h"
#include "eds/epa.h"

#include "bsp.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/

struct blinky_workspace
{
    struct netimer              period;
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static naction state_init(struct nsm *, const struct nevent *);
static naction state_on  (struct nsm *, const struct nevent *);
static naction state_off (struct nsm *, const struct nevent *);

/*=======================================================  LOCAL VARIABLES  ==*/

static struct nepa              g_blinky_epa;
static struct nevent *          g_blinky_queue_storage[10];
static struct nheap             g_blinky_mem;
static uint8_t                  g_blinky_mem_storage[1024];
static struct blinky_workspace  g_blinky_workspace;

static const struct nepa_define g_blinky_define =
{
    .sm.wspace                  = &g_blinky_workspace,
    .sm.init_state              = &state_init,
    .sm.type                    = NSM_TYPE_FSM,
    .working_queue.storage      = g_blinky_queue_storage,
    .working_queue.size         = sizeof(g_blinky_queue_storage),
    .thread.priority            = 1
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static naction state_init(struct nsm * sm, const struct nevent * event)
{
    return (NACTION_HANDLED);
}

static naction state_on  (struct nsm * sm, const struct nevent * event)
{
    return (NACTION_HANDLED);
}

static naction state_off (struct nsm * sm, const struct nevent * event)
{
    return (NACTION_HANDLED);
}

/*===========================================  GLOBAL FUNCTION DEFINITIONS  ==*/

int main(void)
{
    bsp_init();

    nmodule_core_init();
    nmodule_sched_init();
    ncore_timer_enable();

    nheap_init(&g_blinky_mem, g_blinky_mem_storage, sizeof(g_blinky_mem_storage));
    nevent_register_mem(&g_blinky_mem.mem_class);
    nepa_init(&g_blinky_epa, &g_blinky_define);
    neds_run();

    for (;;);
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
