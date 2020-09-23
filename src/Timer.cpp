//  ********************  TIMER.CPP  ********************
#include <Common.h>

#define Code 0x0000FFFFL

unsigned char set_pause(unsigned long time, unsigned short *pTCNT, unsigned char *pFCK)
{
//  *pFCK   TC0_t.CTRLA
//  *pTCNT  TC0_t.CNT
  unsigned char Data = 0, Temp = 0x01;
  //unsigned short Count = 0;
  //*pTCNT = 0;
  //*pFCK = 1;
  //Temp = 1;
  time = time * F_QuartzM;
  while ((time > Code) || (Data > 0))
  {
    switch (Temp)
    {
    case 1: case 2: case 3:
      time = time >> 1;
      break;
    case 4:
      time = time >> 3;    //
      break;
    case 5: case 6:
      time = time >> 2;
      break;
    default:
      Temp = 6;
      Data++;
      break;
    }
    Temp++;
  }

/*
  if (Data) *pTCNT = 0;
  else
  {
    if (time < 0xA0) time = 0xA0;
    *pTCNT = Code - (time - 9);
  }
*/

  if (Data) *pTCNT = 0xFFFF;
  else
  {
    *pTCNT = time;
  }

  *pFCK = Temp;

  return Data;
}

void CTIMER0::Init(TC0_t *pField, unsigned long pause, unsigned char Level, unsigned char Mask)            //  пауза в мкс
{
  unsigned char Data;

  pTimer = pField;

//  TCCR0A = PreScalerTimer0;
  Data = set_pause( pause, &Start, (unsigned char *) &pTimer->CTRLA);
  Count = 0;

//  TCNT0 = StartTimer0;
  pTimer->CNT = 0;
  pTimer->PER = Start;

  pTimer->INTCTRLA = Level; //  High level interrupt

//  TIMSK0 |= TIMER_DONE;
  pTimer->INTFLAGS = Mask; //  Overflow/Underflow - WGMODE 000 - TOP - Overflow
}

unsigned char CTIMER0::Is(unsigned char Mask)
{
  return (pTimer->INTFLAGS & Mask);
}

void CTIMER0::Close(unsigned char Mask)
{
  pTimer->CTRLA &= 0xF0;
  pTimer->INTFLAGS &= ~Mask;
  Start = Count = 0;
}

void CTIMER0::CounterIncrement(void)
{
  //pTimer->CNT += Start;
  Count++;
}

//  ********************  TIMER.CPP  ********************
