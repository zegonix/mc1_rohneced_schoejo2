/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/** 
 *  \brief  Interface of module stop_watch_ctrl.
 * 
 *  $Id: stop_watch_ctrl.h 1064 2016-11-18 06:12:26Z feur $
 * ------------------------------------------------------------------------- */

/* Re-definition guard */
#ifndef _STOP_WATCH_CTRL_H
#define _STOP_WATCH_CTRL_H


/* Standard includes */
#include <stdint.h>


/* -- Public type definitions
 * ------------------------------------------------------------------------- */

/* enumerate the events */
typedef enum {
    SWC_NO_EVENT,
    SWC_DISPLAY_UPDATE_EVENT,
    SWC_BUTTON_EVENT
} stop_watch_ctrl_events_t;


/* -- Public function declarations
 * ------------------------------------------------------------------------- */

/**
 *  \brief  Put new event in this FSMs queue.
 *  \param  event : Event for the FSM to process.
 */
void sw_ctrl_put_queue(stop_watch_ctrl_events_t event);

/**
 *  \brief  Handle events in queue of this FSM.
 */
void sw_ctrl_handle_event(void);

/**
 *  \brief  Update content content for display for this FSM.
 */
void sw_ctrl_update_display(void);

#endif
