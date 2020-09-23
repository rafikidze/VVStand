//  ********************  BUFFER.CPP  ********************
#include <Common.h>

__monitor unsigned char GetBuffer(BUFFER *buffer)
{
  //Mutex m;
  return buffer->Data[buffer->Tail & LIMIT_SIZE];
}

__monitor unsigned char GetBufferOffset(BUFFER *buffer, unsigned char offset)
{
  //Mutex m;
  return buffer->Data[(buffer->Tail + offset) & LIMIT_SIZE];
}

__monitor unsigned char FixBuffer(BUFFER *buffer)
{
  //Mutex m;
  return buffer->Data[buffer->Fix & LIMIT_SIZE];
}

__monitor unsigned char PopBuffer(BUFFER *buffer)
{
  //Mutex m;
  return buffer->Data[buffer->Tail++ & LIMIT_SIZE];
}

__monitor void SetBuffer(BUFFER *buffer, unsigned char Data)
{
  //Mutex m;
  buffer->Data[buffer->Head & LIMIT_SIZE] = Data;
}


__monitor void PushBuffer(BUFFER *buffer, unsigned char Data)
{
  //Mutex m;
  buffer->Data[buffer->Head++ & LIMIT_SIZE] = Data;
}

__monitor unsigned char GetTail(BUFFER *buffer)
{
    return buffer->Tail;
}

__monitor void SetTail(BUFFER *buffer, unsigned char Val)
{
    buffer->Tail = Val;
}

void InitBuffer(BUFFER *buffer, unsigned char  Code)
{
  EraseBuffer(buffer);
  buffer->Code = Code;
}

void EraseBuffer(BUFFER *buffer)
{
//  buffer->Code - не изменяется, это фиксированная характеристика буфера
  unsigned char _t;
  _t = __save_interrupt();
  __disable_interrupt();
    buffer->Status = buffer->Title = buffer->Lenght = buffer->Error = buffer->Head = buffer->Tail = buffer->Fix = 0x00;
  __restore_interrupt(_t);
}

void IsBufferOver(BUFFER *buffer)
{
    if ( (buffer->Head - buffer->Tail) > LIMIT_SIZE )                                         // контроль переполнения буфера
    {
      buffer->Status |= BUFFER_OVER;                                                          // буфер переполнен
    }
}

unsigned char IsBufferNotEmpty(BUFFER *buffer)
{
  if (buffer->Head > buffer->Tail)  return 1;
  else                              return 0;
}

unsigned char BufferLength(BUFFER *buffer)
{
  return (buffer->Head - buffer->Tail);
}

unsigned char GetBufferSpace(BUFFER *buffer, unsigned char Space)
{
  unsigned short Temp;
  Temp = Space + buffer->Head;
  if (Temp > 0xFF) //индексация за пределами unsigned char
    return 0;
  else
    return (buffer->Tail + LIMIT_SIZE - buffer->Head);
}

void RelocateBufferEnd(BUFFER *buffer)
{
    unsigned char Code = 0x00, Tail;
    Tail = buffer->Fix;

    if (buffer->Code & BUFFER_END)
    {
      while ((buffer->Head > Tail) && (Code != END_MESSAGE))
      {
        Code = buffer->Data[Tail++ & LIMIT_SIZE];
        if (Code == END_MESSAGE)
        {
          buffer->Status |= BUFFER_FULL;
        }
      }
    }
    else
    {
      Tail += buffer->Lenght; //+ 0x01;
    }

    buffer->Tail = Tail;
}

void CopyBuffer(BUFFER *In, BUFFER *Out)
{
  unsigned char Code = 0x00, Tail;
  Tail = In->Tail;
  while (In->Head > Tail)
  {
    Code = In->Data[Tail++ & LIMIT_SIZE];
    Out->Data[Out->Head++ & LIMIT_SIZE] = Code;
  }
}


void CopyBufferFix(BUFFER *In, BUFFER *Out, unsigned char Start, unsigned char Count)
{
  unsigned char Code = 0x00, Tail;
  Tail = Start;
  for (unsigned char Data = 0x00; Data < Count; Data++)
  {
    Code = In->Data[Tail++ & LIMIT_SIZE];
    Out->Data[Out->Head++ & LIMIT_SIZE] = Code;
  }
}

unsigned char ClearBufferFix(BUFFER *buffer)                                                              // очистка Status буфера, корректировка счетчиков
{
  unsigned char Data = 0x00, Temp;
  buffer->Status &= BUFFER_DO;

  while (buffer->Fix >= MAX_SIZE)                                                           //  сдвигаем счетчики
  {
    buffer->Fix -= MAX_SIZE;
    buffer->Tail -= MAX_SIZE;
    buffer->Head -= MAX_SIZE;
    Data += MAX_SIZE;
  }

  if (buffer->Status & BUFFER_DO);
  else
  {
    Temp = buffer->Fix;
    if (Temp >= buffer->Head) //  данные обработаны
    {
      buffer->Fix = 0x00;
      buffer->Tail -= Temp;
      buffer->Head -= Temp;
      Data += Temp;
    }
  }

  return Data;
}

void ClearBuffer(BUFFER *buffer)                                                              // очистка Status буфера, корректировка счетчиков
{
  unsigned char _t;
  _t = __save_interrupt();
  __disable_interrupt();

  buffer->Title = buffer->Error = 0x00;

  if ((buffer->Tail >= buffer->Head) || ((buffer->Status & BUFFER_OVER) != 0x00))             //  буфер пустой (ошибка индексации) или переполнен
  {
        buffer->Status = buffer->Lenght = buffer->Error = buffer->Head = buffer->Tail = buffer->Fix = 0x00;
  }
  else
  {
    while (buffer->Fix >= MAX_SIZE)                                                           //  сдвигаем счетчики
    {
        buffer->Fix -= MAX_SIZE;
        buffer->Tail -= MAX_SIZE;
        buffer->Head -= MAX_SIZE;
    }

    if (buffer->Code & BUFFER_IN)                                                             //  ПРИЕМ
    {
      if (buffer->Code & (BUFFER_PRO | BUFFER_BEGIN | BUFFER_X))  //  резерв
      {
        //buffer->Status = buffer->Lenght = buffer->Error = buffer->Head = buffer->Tail = buffer->Fix = 0x00;
        buffer->Status = buffer->Lenght = buffer->Error = 0x00;
        buffer->Fix = buffer->Tail;
      }
      else    //(buffer->Code & BUFFER_PRO) == 0x00
      {
        buffer->Status = 0x00;
        if (buffer->Code & BUFFER_END)
        {
          if (buffer->Data[(buffer->Tail - 1) & LIMIT_SIZE] == END_MESSAGE)                   //  считали буфер до конца сообщения                   //  сообщение принято полностью
          {
            buffer->Fix = buffer->Tail;                                                       //  смещаем счетчик
          }
          else
          {
            while (buffer->Fix < buffer->Tail)
            {
              if (buffer->Data[buffer->Fix++ & LIMIT_SIZE] == END_MESSAGE)                    //  обнаружен конец сообщения
              {
                break;
              }
            }
            buffer->Tail = buffer->Fix;
          }
          // проверка заполнения буфера - контролируется в CopyToOut(buffer) и здесь при приеме неопознанного символа;

          while (buffer->Tail < buffer->Head)
          {
            buffer->Status |= BUFFER_DO;
            if (buffer->Data[buffer->Tail++ & LIMIT_SIZE] == END_MESSAGE)
            {
              buffer->Status |= BUFFER_FULL;
              break;
            }
          }
          buffer->Tail = buffer->Fix;
        }
        else
        {
            while (buffer->Fix < buffer->Tail)
            {
              buffer->Fix++;
            }

            if (buffer->Fix == buffer->Head)
            {
              buffer->Status = buffer->Lenght =
              buffer->Head = buffer->Tail = buffer->Fix = 0x00;
            }
        }

      } //elseif (buffer->Status & BUFFER_PRO)
    }
    else                                                                                      //  ПЕРЕДАЧА
    {
      buffer->Status &= BUFFER_DO;
      if (buffer->Status & BUFFER_DO);
      else
        buffer->Fix = buffer->Tail;
      if (buffer->Code & (BUFFER_PRO | BUFFER_BEGIN | BUFFER_X))
      {
        //  резерв
      }
      else ;
    }
  }
  __restore_interrupt(_t);
}

//  ********************  BUFFER.CPP  ********************
