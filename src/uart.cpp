//  ********************  UART.CPP  ********************
#include <Common.h>
unsigned char DelayRx;
unsigned char isEven( unsigned char Data )
{
    unsigned char Even = 0x00;
    while ( Data )
    {
        Even += Data;
        Data = Data >> 1;
    }
    return ( Even & 0x01 );
}

void CUSART::Init(USART_t *pField, PORT_t *BasePORT, unsigned char TxPIN,
                             PORT_t *EnablePORT, unsigned char EnPIN,
                             unsigned long  Baud,                 //BAUDCTRLA-B
                             signed char    BaudScale,              //BAUDCTRLB
                             unsigned char  Frame,                //CTRLC
                             unsigned char  SetLevel,             //уровни прерываний для CTRLA
                             unsigned char  Control)              //CTRLB
{
  unsigned char _t, Temp;
  unsigned short BaudRate;

  _t = __save_interrupt();
  __disable_interrupt();

  pUSART = pField;

  //  установка линии Tx
  if (TxPIN)
  {
    pBasePORT = BasePORT;
    pinTx = TxPIN;
    pBasePORT->OUTSET = pinTx;
  __no_operation();
    pBasePORT->DIRSET = pinTx;
  }
  else
  {
    pinTx = 0;
  }

  //  установка линии En (для RS-485)
  if (EnPIN)
  {
    pEnablePORT = EnablePORT;
    pinEn = EnPIN;
    pEnablePORT->OUTCLR = pinEn;
  __no_operation();
    pEnablePORT->DIRSET = pinEn;
  }
  else
  {
    pinEn = 0;
  }

  //  установка BAUDCTRLA/BAUDCTRLB     F_Quartz
  if (Control & USART_CLK2X)
  {
    if (BaudScale >= 0)
    {
      BaudRate = (F_Quartz >> (BaudScale + 3)) / Baud;
    }
    else
    {
      BaudRate = ((F_Quartz >> 3) / Baud  - 1) >> BaudScale;
    }
  }
  else
  {
    if (BaudScale >= 0)
    {
      BaudRate = (F_Quartz >> (BaudScale + 4)) / Baud - 1;
    }
    else
    {
      Temp = -BaudScale;
      BaudRate = ((F_Quartz << Temp) >> 4) / Baud  - (1 << Temp);
    }
  }

  //pUSART->BAUDCTRLA = *((unsigned char *) BaudRate);
  Temp = (unsigned char) BaudRate;
  pUSART->BAUDCTRLA = Temp;

  //pUSART->BAUDCTRLB = (BaudRate >> 8) & 0x0F;
  //pUSART->BAUDCTRLB |= ((unsigned char) (BaudScale & 0x0F)) << 4;
  //pUSART->BAUDCTRLB |= BaudScale << 4;
  Temp = (BaudScale & 0x0F) << 4;
  Temp |= (BaudRate >> 8) & 0x0F;

  pUSART->BAUDCTRLB = Temp;

  pUSART->CTRLC = Frame;                                      //(PARITY_MODE_ON | STOP_BIT_2 | CHAR_SIZE_1 | CHAR_SIZE_0)
  pUSART->CTRLA = 0;
  Level = SetLevel;
  pUSART->CTRLB = Control;                                    // RX_ENABLE | TX_ENABLE

  __restore_interrupt(_t);
}

void CUSART::Clear(void)                          //  очистка буфера приема
{
  unsigned char Data, RegA; //
  //Temp = pUSART->CTRLB;
  //pUSART->CTRLB &= ~USART_RXEN;
  RegA = pUSART->STATUS;
  while (RegA & USART_RXCIF)
  {
    Data = pUSART->DATA;
    RegA = pUSART->STATUS;
  }
  //if (Temp & USART_RXEN) pUSART->CTRLB |= USART_RXEN;
}

void CUSART::DoRxC(void)                          // (прерывание Rx)
{
  unsigned char Data, RegA, RegB, Error;
  
  /* if (DelayRx)
   {
     DelayRx--;
     return;
   } else DelayRx = 2;*/
  
  RegA = pUSART->STATUS;
  RegB = pUSART->CTRLC;
  Data = pUSART->DATA;

  
  Error = RegA & (USART_FERR | USART_BUFOVF | USART_PERR);

  //if (RegB & NINE_BIT)                          // проверка четности
  //{
  //  if (isEven(Data) != (RegA & RxBitParity)) ReceiveError_0 |= EVEN_ERR;
  //}
 
  if (Error)                           // ошибка приема - байт не опознан - пока не понятно, что с этим делать - игнорируем
  {
    In.Error |= ERR_DO;
  }
  //else
  //{
    In.Status |= BUFFER_DO;
    In.Data[In.Head++ & LIMIT_SIZE] = Data;     // считывание в буфер приема
    
  //  Out.Data[In.Head++ & LIMIT_SIZE] = Data;

    if (In.Code & BUFFER_PRO)                 // для буфера, работающего сообщениями определенной длины - внести изменения
    {
    }
    else
    {
      if ((Data == END_MESSAGE) && ((In.Code & BUFFER_END) == BUFFER_END))
      {
        In.Status |= BUFFER_FULL;
      }
    }
  //}
}

void CUSART::DoTxC(void)                        // (прерывание Tx)
{

  if (Out.Code & BUFFER_BYTE)
  {//  old - c хорошей паузой между байтами
    pUSART->CTRLA &= ~USART_TXCINTLVL;
    Out.Status &= ~BUFFER_DO;
    Delay = 2;
    if (pinEn)
    {
      if (Out.Head > Out.Tail);
      else
      {                                           //  разрешаем прием по UART
        pEnablePORT->OUTCLR = pinEn;
        if (Out.Code & BUFFER_HF)
        {
          pUSART->CTRLB |= USART_RXEN;
        }
      }
    }
  }
  else
  { //  без паузы
    if (IsBufferNotEmpty(&Out))
    {
      pUSART->DATA = Out.Data[Out.Tail++ & LIMIT_SIZE];
    }
    else
    {
      if (pinEn)
      {
        pEnablePORT->OUTCLR = pinEn;
        if (Out.Code & BUFFER_HF)//  разрешаем прием по UART
        {
          pUSART->CTRLB |= USART_RXEN;
        }
      }
      pUSART->CTRLA &= ~USART_TXCINTLVL;
      Out.Status &= ~BUFFER_DO;
    }
  }


}

void CUSART::DoTx(void)       //  запуск передачи по UART - Transmite
{
  unsigned char _t;
  unsigned char Data;
  if ((Out.Status & BUFFER_DO) == 0)
  {
    if (Out.Code & BUFFER_BYTE)
    {//  old - c хорошей паузой между байтами
      if (Delay)
      {
        Delay--;
        return;
      }
    }

    if (Out.Head > Out.Tail)
    {

      if (pinEn)
      {
        if (pEnablePORT->OUT & pinEn);
        else
        {
          pEnablePORT->OUTSET = pinEn;
          if (Out.Code & BUFFER_HF)                                                   //  RS485 полудуплекс
          {
            pUSART->CTRLB &= ~USART_RXEN;
          }
          return;
        }
      }

      Out.Status |= BUFFER_DO;
      Data = PopBuffer(&Out);
      if (Out.Code & BUFFER_END)
      {
        if (Data == END_MESSAGE) Out.Status |= BUFFER_FULL;
      }
//            if (pUSART->CTRLC & NINE_BIT)
//            {
//              if (isEven(Data))
//                  pUSART->CTRLB |= EVEN_ERR;
//              else
//                  pUSART->CTRLB &= ~EVEN_ERR;
//            }
//            else;

      pUSART->DATA = Data;
  _t = __save_interrupt();
  __disable_interrupt();
      pUSART->CTRLA |= Level & USART_TXCINTLVL;
  __restore_interrupt(_t);
    }
    else
    {
      if (Out.Code & BUFFER_END)
      {
        if (Out.Status & BUFFER_FULL) ;
        else;
      }
      else;

      //if (Out.Code & BUFFER_HF)                                                   //  RS485 полудуплекс
      //{
      //  pUSART->CTRLB |= USART_RXEN;
      //}

      //if (pinEn)
      //{
      //  pEnablePORT->OUTCLR = pinEn;
      //}
    }
  }
}

unsigned char CUSART::IsRxC(void)                       //  разрешение приема по UART
{
  return (pUSART->CTRLA & USART_RXCINTLVL);
}

void CUSART::EnableRxC(void)                       //  разрешение приема по UART
{
  unsigned char _t;
  _t = __save_interrupt();
  __disable_interrupt();
  pUSART->CTRLA |= (Level & USART_RXCINTLVL);
  __restore_interrupt(_t);
}

void CUSART::DisableRxC(void)                       //  запрет приема по UART
{
  unsigned char _t;
  _t = __save_interrupt();
  __disable_interrupt();
  pUSART->CTRLA &= ~(Level & USART_RXCINTLVL);
  __restore_interrupt(_t);
}

void CUSART::Close(void)                        // завершение работы UART
{
  unsigned char _t;

  _t = __save_interrupt();
  __disable_interrupt();

  pUSART->CTRLA = 0x00;
  pUSART->CTRLC = 0x00;
  pUSART->CTRLB = 0x00;

  //  установка линии En (для RS-485)
  if (pinEn)
  {
    pEnablePORT->OUTCLR = pinEn;
  __no_operation();
    pEnablePORT->DIRCLR = pinEn;
  }

  //  установка линии Tx
  if (pinTx)
  {
    pBasePORT->OUTCLR = pinTx;
  __no_operation();
    pBasePORT->DIRCLR = pinTx;
  }
  __restore_interrupt(_t);
}
//  ********************  UART.CPP  ********************
