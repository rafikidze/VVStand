//  ********************  UART_ADU.CPP  ********************
#include <Common.h>
#include "Control.h"
#include "TimerBase.h"
#include "Communication.h"

#include "Inner.h"

//#ifdef         UART_ADU

CUSART Uart_ADU;    //USARTC1
CUSART Uart_TECHNO; //USARTC0

//pBasePORT           PORTС
//pinTx               0x08
//pEnablePORT         PORTС
//pinEn               0x00
//Baud                в соответствии с конфигурацией
//BaudSel             0x00
//Frame               00 10 1 111 (асинхронный, контроль четности, 2 стоп бита, 9 байт)
//SetLevel            0x3C
//Control             USART_RXEN | USART_TXEN

//  при инициализации - установить Status и Code вх/вых буферов
// вектора прерываний - в зависимости от порта
InterruptServiceRoutine(USARTC0_RXC_vect)     //232
{
  unsigned char _t;
  _t = __save_interrupt();
  __disable_interrupt();

  
  Uart_ADU.DoRxC();
  //  запуск таймера контроля по приему
  if (Uart_ADU.In.Code & BUFFER_TIME)
  {
#ifdef TIMER_ADU
    StartTimerBase(TIME_ADU);
#endif
  }
  __restore_interrupt(_t);
}

InterruptServiceRoutine(USARTC0_TXC_vect)     //232
{
  unsigned char _t;
  _t = __save_interrupt();
  __disable_interrupt();
  Uart_ADU.DoTxC();
  __restore_interrupt(_t);
}

void UART_ADU_Receive(void)
{
  unsigned char Data = 0x00;

  if (IsBufferNotEmpty(&Uart_ADU.In))
  {
    Data = ReceiveInner(&Uart_ADU.In);

    if (Data) //  пришло полное сообщение
    {
  #ifdef TIMER_ADU
      if (Uart_ADU.In.Code & BUFFER_TIME)
      {
        if (IsBufferNotEmpty(&Uart_ADU.In));  //  идет следующее сообщение
        else StopTimerBase(TIME_ADU);         //  останавливаем таймер
      }
  #endif
      ClearBuffer(&Uart_ADU.In);
      
     // DoInnerMessage(YES, &Uart_ADU.Out);

    }
  }
}

void UART_ADU_Transmite(void)
{
  Uart_ADU.DoTx();
}

//  при инициализации - установить Status и Code вх/вых буферов
// вектора прерываний - в зависимости от порта
InterruptServiceRoutine(USARTC1_RXC_vect)     //232
{
  unsigned char _t;
  _t = __save_interrupt();
  __disable_interrupt();
 
  Uart_TECHNO.DoRxC();
  //  запуск таймера контроля по приему
  if (Uart_TECHNO.In.Code & BUFFER_TIME)
  {
#ifdef TIMER_ADU
    StartTimerBase(TIME_ADU);
#endif
  }
  __restore_interrupt(_t);
}

InterruptServiceRoutine(USARTC1_TXC_vect)     //232
{
  unsigned char _t;
  _t = __save_interrupt();
  __disable_interrupt();
  Uart_TECHNO.DoTxC();
  __restore_interrupt(_t);
}

void UART_TECHNO_Receive(void)
{
  unsigned char Data = 0x00;

  if (IsBufferNotEmpty(&Uart_TECHNO.In))
  {
    Data = ReceiveInner(&Uart_TECHNO.In);

    if (Data) //  пришло полное сообщение
    {
  #ifdef TIMER_ADU
      if (Uart_TECHNO.In.Code & BUFFER_TIME)
      {
        if (IsBufferNotEmpty(&Uart_TECHNO.In));  //  идет следующее сообщение
        else StopTimerBase(TIME_ADU);         //  останавливаем таймер
      }
  #endif
      ClearBuffer(&Uart_TECHNO.In);
      
    }
  }
}

void UART_TECHNO_Transmite(void)
{
  Uart_TECHNO.DoTx();
}



//#endif
//  ********************  UART_ADU.CPP  ********************
