//  ********************  TIMERBASE.CPP  ********************
#include <Common.h>
#include "TimerBase.h"

CTIMER0 Timer_C0;

unsigned long   TimeBase = 500,                   //  время в мкс
                TimeBaseOn = 200000,
                TimeBaseOff = 5;
unsigned char   FlagTimerBase = 0x80,
                IsTimeBase = 0,
                DoTimeBase = 0;
unsigned short  TimerBase[8] = {   0,   0,   0,   0,
                                   0,   0,   0,   0},
//                                    0,   0,   0,   0,
//                                    0,   0,   0,   0,
                EndTimerBase[8] =  {   2,   1, 2000,  10,
                                      20,  20, 14000, 20};    //,
//                                        0,   0,   0,   0,
//                                        0,   0,   0,   0};
//  устанавливаем время в мс
//  0 TAKT            1
//  1 GET             0.5
//  2 ADC             150
//  3 DAC             5
//  4 KEY             0.5
//  5 LCD             7000
//  6 CANCEL_CHANGE   7000    36B0
//  7 CONTROL         10    36B0

//    UART_ADU
//    UART_BY
//    UART_LCD
//    TWI_LCD
//    UART_ADAM
//    SPI_PLM
//    SPI_ENET
//    SPI_PLM


InterruptServiceRoutine(TCC0_OVF_vect)
{
  unsigned char _t;
  _t = __save_interrupt();
  __disable_interrupt();
  Timer_C0.CounterIncrement();
  __restore_interrupt(_t);
}

//  строим на базе Timer_C0 работу с текущими счетчиками

void InitTimerBase( unsigned long pause)
{
  Timer_C0.Init( &TCC0, pause, 1, TIMER_OVF);
}

unsigned char IsTimerBase(void)
{
  return Timer_C0.Is(TIMER_OVF);
}

__monitor void ResetTimerBase(void)
{
  Timer_C0.Count = 0;
}

__monitor void CloseTimerBase(void)
{
  Timer_C0.Close(TIMER_OVF);
  DoTimeBase = 0;
  IsTimeBase = 0;
}

__monitor void StartTimerBase(unsigned char Data)
{
  TimerBase[Data] = 0;
  IsTimeBase |= 1 << Data;
}

__monitor void ReStartTimerBase(unsigned char Data)
{
  TimerBase[Data] -= EndTimerBase[Data];
  IsTimeBase |= 1 << Data;
}

__monitor void StopTimerBase(unsigned char Data)
{
  unsigned char Flag = 1 << Data;
  IsTimeBase &= ~Flag;
  DoTimeBase &= ~Flag;
  //TimerBase[Data] = 0;
}

__monitor unsigned short GetTimerBase(void)
{
  unsigned char Flag;
  unsigned char Data, Count = 0x00;

  Data = Timer_C0.Count;
  Timer_C0.Count = 0;
  //PORTB.OUT ^= 0x08;

  if (Data)
  {
    Flag = 1;

    while ( IsTimeBase >= Flag ) // Flag <= FlagTimerBase
    {
      if (IsTimeBase & Flag)
      {
        TimerBase[Count] += Data;
        if (TimerBase[Count] >= EndTimerBase[Count])
        {
          IsTimeBase &= ~Flag;
          DoTimeBase |= Flag;
        }
      }
      Flag = Flag << 1;
      Count++;
      if (Flag);
      else break;
    }
  }

}

__monitor void SetTimerBase(unsigned char Data, unsigned short Time)
{
  TimerBase[Data] = 0;
  EndTimerBase[Data] = Time;
}


//  ********************  TIMERBASE.CPP  ********************
