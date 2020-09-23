//  ********************  TIMEREVENT.CPP  ********************
#include <Common.h>
#include "Control.h"
#include "TimerEvent.h"

CTIMER0 Timer_D0;

InterruptServiceRoutine(TCD0_OVF_vect)
{
  unsigned char _t;
  _t = __save_interrupt();
  __disable_interrupt();
  __restore_interrupt(_t);
}

void StartTimerEvent( unsigned long pause)
{
  Timer_D0.Init( &TCD0, pause, 0, 0);
  //Timer_D0.Init( &TCD0, pause, 1, TIMER_OVF);
}

void CloseTimerEvent(void)
{
  Timer_D0.Close(TIMER_OVF);
}

//  ********************  TIMEREVENT.CPP  ********************
