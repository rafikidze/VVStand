//  ********************  TRANSFORMATION.CPP  ********************
#include <Common.h>
#include "Transformation.h"

unsigned char sTemp[DIM_TEMP_STR];

unsigned long myabs(unsigned long x)
{
  return ((x > 0) ? x : (-x));
}

void BinDecToA( BUFFER *buffer, unsigned long int Value, unsigned char Lenght, unsigned char Code)
{
    unsigned long int Temp =  Value;
    signed char i;
    unsigned char Count = 0x00;
    if (Code) PushBuffer(buffer, Code);
    while (Temp)
    {
        sTemp[Count++] = (Temp & 0x0F) | 0x30;
        Temp = Temp >> 4;
    }

    if (Lenght > 0x00)
    {
        for (i = 0x00; i < (Lenght - Count); i++)
        {
            PushBuffer(buffer, 0x30);
        }
    }
    for (i = (Count - 0x01); i >= 0x00;)
    {
        PushBuffer(buffer, sTemp[i--]);
    }
}

void longToA( BUFFER *buffer, unsigned short Value, unsigned char Lenght, unsigned char Code)
{
    unsigned short Temp =  Value;
    signed char  i;
    unsigned char Count = 0x00;//, Sign = 0x00;
    if (Code) PushBuffer(buffer, Code);
    while (Temp)
    {
        sTemp[Count++] = (unsigned char) ((Temp % 10) & 0x0F) | 0x30; //ё
        Temp /= 10;
    }

    if (Lenght > 0x00)
    {
        for (i = 0x00; i < (Lenght - Count); i++)
        {
            PushBuffer(buffer, 0x30);
        }
    }
    for (i = (Count - 0x01); i >= 0x00;)
    {
        PushBuffer(buffer, sTemp[i--]);
    }
// проверка на переполнение буфера - не проводится
}

void ChangeHalfByteOrder(unsigned long int *In, unsigned long int *Out, unsigned char Digit)
{
    unsigned long int Temp = *In, Tmp;
    Tmp = 0;
    while (Temp | Digit)
    {
        Tmp = (Tmp) << 4;
        Tmp |= Temp & 0x0F;
        Temp = Temp >> 4;
        Digit--;
    }
    *Out = Tmp;
}

void BinDecToLong( unsigned long int *In, unsigned long int *Out)
{
    unsigned long int Temp = *In, Tmp;

    Tmp = 0;

    while (Temp)
    {
        Tmp = ((Tmp << 2) + Tmp) << 1;      // *10
        Tmp += Temp & 0x0F;                 //  + единицы
        Temp = Temp >> 4;                   //  следующий разряд
    }
    *Out = Tmp;
}

void LongToBinDec( unsigned long int *In, unsigned long int *Out, unsigned char Digit)
{
    unsigned long int Temp = *In, Tmp;

    Tmp = 0;
    while (Temp | Digit)
    {
        Tmp = Tmp << 4;                     // следующий разряд
        Tmp |= ((Temp % 10) & 0x0F);        //  единицы
        Temp /= 10;                         // следующий разряд
        Digit--;
    }
    *Out = Tmp;
}

unsigned char ToHexDec(unsigned char Data)
{
  return (((Data%10) & 0x0F) | (((Data%100)/10) << 4));
}

unsigned char ToHexDecHi(unsigned char Data)
{
  return (((Data%10) & 0x0F) << 4);
}

unsigned char ToHexDecLo(unsigned char Data)
{
  return ((Data%10) & 0x0F);
}

unsigned char InverseOrderBit(unsigned char Data)
{
  unsigned char FlagD = BIT_3, FlagI = BIT_0, Code = 0x00;
  while (FlagD)
  {
    if (Data & FlagD) Code |= FlagI;
    FlagD = FlagD >> 1;
    FlagI = FlagI << 1;
  }
  return Code;
}

void CopyBufferToStr(unsigned char *In, BUFFER *Out, unsigned char Shift, unsigned char Count)
{
  unsigned char Code = 0x00, Tail;
  Tail = Out->Fix + Shift;
  for (int i = 0; i < Count; i++)
  {
    Code = Out->Data[Tail++ & LIMIT_SIZE];
    *In++ = Code;
  }
}
unsigned char GetCharArray(BUFFER *In, unsigned char offset, unsigned char *pData)
{
  unsigned char Error = 0, data;
  //pData = 0;
  
  //*pData = *pData << 4;
  for (int t = 0; t < 4; t++)
  {
    data = GetBufferOffset(In, offset + t);
    if((data > 0x2F) &&(data < 0x40))
    {
      pData[t] = data - 0x30;
    }
    else Error = 1;
  }

  return Error;
}
unsigned char GetCharControlTemperature(BUFFER *In, unsigned char offset, unsigned char *pData)
{
  unsigned char Error = 0, data;
  *pData = 0;
  
  *pData = *pData << 4;
  data = GetBufferOffset(In, offset);
  
  if ((data == 0x2B) || (data == 0x2D) )
    *pData = data;
  else if((data > 0x2F) &&(data < 0x40))
  {
    *pData = data - 0x30;
  }
    
  else Error = 1;
 
  return Error;
}
unsigned char GetChar(unsigned char *str, unsigned char Digit)
{
  unsigned char ucData = 0;
  for (int i = 0; i < Digit; i++)
  {
    ucData = ucData << 4;
    ucData |= *str++ & 0x0F;
  }
  return ucData;
}

unsigned char GetCharControl(BUFFER *In, unsigned char offset, unsigned char Count, unsigned char *pData)
{
  unsigned char Error = 0, data;
  *pData = 0;
  
  for (int i = 0; i < Count; i++)
  {
    *pData = *pData << 4;
    data = GetBufferOffset(In, offset + i);
    if((data > 0x2F) &&(data < 0x40))
      *pData |= data & 0x0F;
    else Error = 1;
  }
  return Error;
}

unsigned char GetCharControlPower(BUFFER *In, unsigned char offset, unsigned char *pData)
{
  unsigned char Error = 0, data;
  *pData = 0;
  
  *pData = *pData << 4;
  data = GetBufferOffset(In, offset);
  if((data > 0x2F) &&(data < 0x40))
  {
    *pData = data - 0x30;
  }
    
  else Error = 1;
 
  return Error;
}

unsigned short GetWord(unsigned char *str, unsigned char Digit)
{
  unsigned short usData = 0;
  for (int i = 0; i < Digit; i++)
  {
    usData = usData << 4;
    usData |= (unsigned char) *str++ & 0x0F;  //ё
  }
  return usData;
}

unsigned long Div10(unsigned long A, unsigned char *Rem)
{
  unsigned long B, C = 0;
  unsigned char index = 0;
  B = A >> 3;
  while (B){
    if (index)  C -= B;
    else        C += B;
    index ^=1;
    B = B >> 2;
  }
  B = ((C << 2) + C) << 1;
  while (B > A) {
    B -= 10;
    C--;
  }
  *Rem = (unsigned char) A - B; //ё
  while (*Rem > 9) {
    *Rem -= 0x0A;
    C++;
  }
  return C;
}


void SetStr(unsigned long Value, unsigned char Count, unsigned char *pStr)
{
  unsigned char Data = 0x00;
  while (Value)
  {
    Value = Div10( Value, pStr--);
    Count--;
    Data++;
  }

//  0 в младшем разряде перед "."
  if (Data);
  else  {
    Count--;
    *pStr-- = NULL_DIGIT;	//
  }

//  ведущие пробелы
  while (Count--) {
    *pStr-- = 0x20;	
  }	
}

void SetStrWithPointPosition(unsigned long Value, unsigned char PtPos, unsigned char Count, unsigned char *pStr)
{
  unsigned char Data = 0x00;
//  знаки после   "."
  while (Data < PtPos)
  {
    if (Value) Value = Div10( Value, pStr--);
    else *pStr-- = NULL_DIGIT;                      //  0 после .
    Data++;
    Count--;
  }

//  "."
  *pStr-- = 0x2E;
  Count--;
  Data = 0x00;


//  SetStr( Value, Count, pStr)
  while (Value)
  {
    Value = Div10( Value, pStr--);
    Count--;
    Data++;
  }

//  0 в младшем разряде перед "."
  if (Data);
  else  {
    Count--;
    *pStr-- = NULL_DIGIT;	//
  }

//  ведущие пробелы
  while (Count--) {
    *pStr-- = 0x20;	
  }	
}


//  ********************  TRANSFORMATION.CPP  ********************
