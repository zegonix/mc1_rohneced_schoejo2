/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/** 
 *  \brief  Implementation of module stop_watch_ctrl.
 * 
 *  $Id: stop_watch_ctrl.c 1064 2016-11-18 06:12:26Z feur $
 * ------------------------------------------------------------------------- */
 

/* User includes */
#include "stop_watch_ctrl.h"
#include "stop_watch.h"
#include "queue.h"
#include "lcd.h"

/* -- Macros
 * ------------------------------------------------------------------------- */

//                           "                    "
#define TEXT_LINE_1         ("STOP WATCH    0>MODE")
#define TEXT_LINE_2_START   ("              1>GO  ")
#define TEXT_LINE_2_RESET   ("              1>RST ")
#define TEXT_LINE_2_STOP    ("              1>STOP")

/// STUDENTS: To be programmed

typedef enum {
	STATE_GO,
	STATE_STOP,
	STATE_RESET
} sw_state_t;

sw_state_t state = STATE_RESET;
queue_t stop_watch_ctrl_queue;

void sw_ctrl_handle_event(void)
{
	stop_watch_ctrl_events_t event = queue_dequeue(&stop_watch_ctrl_queue);
	if(event == SWC_BUTTON_EVENT)
	{
		switch(state)
		{
			case STATE_GO:
				state = STATE_STOP;
				sw_ctrl_update_display();
				break;
			case STATE_STOP:
				state = STATE_RESET;
				sw_ctrl_update_display();
				break;
			case STATE_RESET:
				state = STATE_GO;
				sw_ctrl_update_display();
				break;
			default:
				state = STATE_RESET;
				sw_ctrl_update_display();
				break;
		}
	}
}

void sw_ctrl_put_queue(stop_watch_ctrl_events_t event)
{
	queue_enqueue(&stop_watch_ctrl_queue, event);
}

void sw_ctrl_update_display(void)
{
	lcd_write(LCD_LINE_1, TEXT_LINE_1);
	switch(state)
	{
		case STATE_GO:
			lcd_write(LCD_LINE_2, TEXT_LINE_2_STOP);
			break;
		case STATE_STOP:
			lcd_write(LCD_LINE_2, TEXT_LINE_2_RESET);
			break;
		case STATE_RESET:
			lcd_write(LCD_LINE_2, TEXT_LINE_2_START);
			break;
		default:
			break;
	}
}

/// END: To be programmed
