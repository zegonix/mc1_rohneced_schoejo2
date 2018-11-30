/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/** 
 *  \brief  Implementation of module button_count.
 * 
 *  $Id: button_count.c 1064 2016-11-18 06:12:26Z feur $
 * ------------------------------------------------------------------------- */
 

/* User includes */
#include "button_count.h"
#include "queue.h"
#include "lcd.h"
#include "reg_ctboard.h"

/* -- Macros
 * ------------------------------------------------------------------------- */

//                           "                    "
#define TEXT_LINE_1         ("BUTTONS       0>MODE")
#define TEXT_LINE_2_T0      ("              1>T0  ")
#define TEXT_LINE_2_T1      ("              1>T1  ")
#define TEXT_LINE_2_T2      ("              1>T2  ")
#define TEXT_LINE_2_T3      ("              1>T3  ")

/// STUDENTS: To be programmed

typedef enum {
	STATE_T0,
	STATE_T1,
	STATE_T2,
	STATE_T3
} bcc_state_t;

// local variables
static queue_t bcc_queue;
static bcc_state_t state = STATE_T0;
static uint16_t counter_t0 = 0;
static uint16_t counter_t1 = 0;
static uint16_t counter_t2 = 0;
static uint16_t counter_t3 = 0;

// local function declarations
static void button_event_callback(uint32_t data);
static void count_button_event(button_count_events_t event);

void button_count_init(void)
{
	buttons_register_observer(button_event_callback);
}

void button_count_put_queue(button_count_events_t event)
{
	queue_enqueue(&bcc_queue, event);
}

void button_count_handle_event(void)
{
	button_count_events_t event = queue_dequeue(&bcc_queue);
	count_button_event(event);
	if (event == BCC_BUTTON_T1_EVENT)
	{
		switch(state)
		{
			case STATE_T0:
				state = STATE_T1;
				break;
			case STATE_T1:
				state = STATE_T2;
				break;
			case STATE_T2:
				state = STATE_T3;
				break;
			case STATE_T3:
				state = STATE_T0;
				break;
			default:
				state = STATE_T0;
				break;
		}
		button_count_update_display();
	}
	seg7_output_update();
}

void button_count_get_output(seg7_output_t *output)
{
	uint16_t count;
	uint16_t bcd;
	switch(state)
	{
		case STATE_T0:
			count = counter_t0;
			break;
		case STATE_T1:
			count = counter_t1;
			break;
		case STATE_T2:
			count = counter_t2;
			break;
		case STATE_T3:
			count = counter_t3;
			break;
	}
	
	bcd = count % 10u;
	bcd |= ((count / 10u) % 10u) << 4u;
	bcd |= ((count / 100u) % 10u) << 8u;
	bcd |= ((count / 1000u) % 10u) << 12u;
	
	output->value = bcd;
	output->dots = 0;
	
}

void button_count_update_display(void)
{
	lcd_write(LCD_LINE_1, TEXT_LINE_1);
	switch(state)
	{
		case STATE_T0:
			lcd_write(LCD_LINE_2, TEXT_LINE_2_T1);
			break;
		case STATE_T1:
			lcd_write(LCD_LINE_2, TEXT_LINE_2_T2);
			break;
		case STATE_T2:
			lcd_write(LCD_LINE_2, TEXT_LINE_2_T3);
			break;
		case STATE_T3:
			lcd_write(LCD_LINE_2, TEXT_LINE_2_T0);
			break;
		default:
			break;
	}
}

static void button_event_callback (uint32_t data)
{
	queue_enqueue(&bcc_queue, data);
}

static void count_button_event(button_count_events_t event)
{
	switch(event)
	{
		case BCC_BUTTON_T0_EVENT:
			counter_t0++;
			if (counter_t0 >= 9999u)
			{
				counter_t0 = 0;
			}
			break;
		case BCC_BUTTON_T1_EVENT:
			counter_t1++;
			if (counter_t1 >= 9999u)
			{
				counter_t1 = 0;
			}
			break;
		case BCC_BUTTON_T2_EVENT:
			counter_t2++;
			if (counter_t2 >= 9999u)
			{
				counter_t2 = 0;
			}
			break;
		case BCC_BUTTON_T3_EVENT:
			counter_t3++;
			if (counter_t3 >= 9999u)
			{
				counter_t3 = 0;
			}
			break;
		default:
			break;
	}
}
		
/// END: To be programmed
