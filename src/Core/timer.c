/*
 * timer.c
 *
 * Created: 2016/12/27 5:05:29 PM
 *  Author: philb
 */ 
 #include "../system.h"

#define MAX_TIMERS 32

#define RUNNING_BM 0x01
#define EXPIRED_BM 0x02

struct tc_module tc_instance_system;

#define TIMER_INT_ENABLE()      tc_enable_callback(&tc_instance_system, TC_CALLBACK_OVERFLOW);
#define TIMER_INT_DISABLE()     tc_disable_callback(&tc_instance_system, TC_CALLBACK_OVERFLOW);

void tc_overflow_callback(struct tc_module *const module_inst);
void timer_tick(U32 ul_ms);

static TIMER_HANDLE gx_timer_list[MAX_TIMERS] = { 0 };

void tc_overflow_callback(struct tc_module *const module_inst)
{
  timer_tick(1);
}

void timer_module_init(void)
{
  struct tc_config config_tc;
  tc_get_config_defaults(&config_tc);

  config_tc.counter_size    = TC_COUNTER_SIZE_8BIT;
  /* 8MHz */
  config_tc.clock_source    = GCLK_GENERATOR_3;
  /* 31 250 Hz */
  config_tc.clock_prescaler = TC_CLOCK_PRESCALER_DIV256;
  /* 1008 Hz */
  config_tc.counter_8_bit.period = 30;
    
  tc_init(&tc_instance_system, SYSTEM_TC, &config_tc);

  tc_enable(&tc_instance_system);

	tc_register_callback(&tc_instance_system, tc_overflow_callback, TC_CALLBACK_OVERFLOW);
	tc_enable_callback(&tc_instance_system, TC_CALLBACK_OVERFLOW);
}

TIMER_HANDLE timer_new(U32 period)
{
  TIMER_HANDLE x_new_timer = NULL;
  U32 i;

  x_new_timer = (TIMER_HANDLE) malloc(sizeof(TIMER));

  if(NULL == x_new_timer)
  {
    return NULL;
  }

  x_new_timer->Count = 0;
  x_new_timer->Period = period;
  x_new_timer->Flags = 0x00;

  for( i = 0 ; i < MAX_TIMERS ; i++ )
  {
    if(gx_timer_list[i] == NULL)
    {
      TIMER_INT_DISABLE();
      gx_timer_list[i] = x_new_timer;
      TIMER_INT_ENABLE();
      break;
    }
  }

  return x_new_timer;
}

void timer_delete(TIMER_HANDLE x_timer)
{
  U32 i;

  if(x_timer == NULL)
    return;

  for( i = 0 ; i < MAX_TIMERS ; i++)
  {
    if(gx_timer_list[i] == x_timer)
    {
      TIMER_INT_DISABLE();
      gx_timer_list[i] = NULL;
      TIMER_INT_ENABLE();
      free(x_timer);
      return;
    }
  }
}

U32 timer_time_remaining(TIMER_HANDLE x_timer)
{
  return (x_timer->Period - x_timer->Count);
}

void timer_stop(TIMER_HANDLE x_timer)
{
  x_timer->Flags &= ~RUNNING_BM;
}

void timer_start(TIMER_HANDLE x_timer)
{
  x_timer->Flags |= RUNNING_BM;
}

void timer_reset(TIMER_HANDLE x_timer)
{
  x_timer->Count = 0;
  x_timer->Flags = RUNNING_BM;
}

void timer_restart(TIMER_HANDLE x_timer, U32 period)
{
  x_timer->Period = period;
  timer_reset(x_timer);
}


bool timer_expired(TIMER_HANDLE x_timer) { return (EXPIRED_BM == (x_timer->Flags & EXPIRED_BM)); }
bool timer_running(TIMER_HANDLE x_timer) { return (RUNNING_BM == (x_timer->Flags & RUNNING_BM)); }

void timer_tick(U32 ul_ms)
{
  U32 i;

  for( i = 0 ; i < MAX_TIMERS ; i++ )
  {
    if( NULL != gx_timer_list[i] )
    {
      if( timer_running(gx_timer_list[i]) && !timer_expired(gx_timer_list[i]) )
      {
        if(timer_time_remaining(gx_timer_list[i]) > ul_ms)
        {
          gx_timer_list[i]->Count += ul_ms;
        }
        else
        {
          gx_timer_list[i]->Flags |= EXPIRED_BM;
          gx_timer_list[i]->Count = gx_timer_list[i]->Period;
        }
      }
    }
  }
}