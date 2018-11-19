/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/** 
 *  \brief  Implementation of module reload_ctrl.
 * 
 *  $Id: reload_ctrl.c 1064 2016-11-18 06:12:26Z feur $
 * ------------------------------------------------------------------------- */
 

/* User includes */
#include "reload_ctrl.h"
#include "reload.h"
#include "queue.h"
#include "seg7_display.h"
#include "lcd.h"
#include "egg_timer.h"


/* -- Macros
 * ------------------------------------------------------------------------- */

//                           "                    "
#define TEXT_LINE_1         ("RELOAD        0>MODE")
#define TEXT_LINE_2_SEC     ("3>SEC- 2>SEC+ 1>MIN ")
#define TEXT_LINE_2_MIN     ("3>MIN- 2>MIN+ 1>SEC ")

/// STUDENTS: To be programmed
static queue_t rl_ctrl_queue;

void rl_ctrl_handle_event(void)
{
		uint32_t event;
	event=queue_dequeue(&rl_ctrl_queue);
	if (event!= RLC_NO_EVENT)
	{
		seg7_output_update();
		lcd_output_update();
	}
	switch(state)
		case 
}

void rl_ctrl_update_display(void)
{
    lcd_write(LCD_LINE_1, TEXT_LINE_1);
}

void rl_ctrl_put_queue(reload_ctrl_events_t event)
{
		queue_enqueue(&rl_ctrl_queue, event);
}
/// END: To be programmed
