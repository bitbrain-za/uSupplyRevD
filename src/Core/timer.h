/*
 * timer.h
 *
 * Created: 2016/12/27 5:05:40 PM
 *  Author: philb
 */ 


#ifndef TIMER_H_
#define TIMER_H_


typedef struct
{
  U32 Period;
  U32 Count;
  bool Running;
  bool Expired;
}TIMER;

typedef TIMER* TIMER_HANDLE;

extern void timer_module_init(void);
extern TIMER_HANDLE timer_new(U32 period);
extern void timer_delete(TIMER_HANDLE x_timer);

extern void timer_stop(TIMER_HANDLE x_timer);
extern void timer_start(TIMER_HANDLE x_timer);
static inline bool timer_expired(TIMER_HANDLE x_timer) { return x_timer->Expired; }

extern void timer_reset(TIMER_HANDLE x_timer);
extern void timer_restart(TIMER_HANDLE x_timer, U32 period);
extern U32 timer_time_remaining(TIMER_HANDLE x_timer);



#endif /* TIMER_H_ */