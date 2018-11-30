/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/** 
 *  \brief  Implementation of module stop_watch.
 * 
 *  $Id: stop_watch.c 1064 2016-11-18 06:12:26Z feur $
 * ------------------------------------------------------------------------- */
 

/* User includes */
#include "stop_watch.h"
#include "scheduler.h"

/// STUDENTS: To be programmed

typedef struct{
	uint16_t seconds;
	uint8_t milliseconds;
} stop_time_t;

// local variables
static scheduler_task_t task_increment;
static stop_time_t time = {0u, 0u};

// local funtion declarations
static void stop_watch_increment(void);
static void (*finished_call_back)(void);

void stop_watch_start(void (*func_ptr)(void))
{
	time.seconds = 0;
	time.milliseconds = 0;
	seg7_output_update();
	finished_call_back = func_ptr;
	scheduler_register_task(&task_increment, stop_watch_increment, TEN_MILLISECONDS);
}

void stop_watch_stop(void)
{
	scheduler_unregister_task(&task_increment);
}

void stop_watch_reset(void)
{
	time.seconds = 0;
	time.milliseconds = 0;
	seg7_output_update();
}

void stop_watch_get_output(seg7_output_t *output)
{
	uint8_t temp;
  uint16_t bcd;
        
  /* show (1/10)*seconds on first digit */
  temp = time.milliseconds;
  bcd = (temp / 10u) % 10u;

  /* Convert seconds */
  temp = time.seconds;
  bcd |= (temp % 10u) << 4u;
  temp /= 10u;
  bcd |= (temp % 10u) << 8u;
	temp /= 10u;
	bcd |= (temp % 10u) << 12u;
	
	output->value = bcd;
	output->dots = (1 << 1u);
}

static void stop_watch_increment(void)
{
	time.milliseconds++;
	if (time.milliseconds >= 100)
	{
		time.milliseconds = 0;
		time.seconds++;
		if (time.seconds >= 999u)
		{
			finished_call_back();
		}
	}
	seg7_output_update();
}	

/// END: To be programmed
