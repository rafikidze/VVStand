//  ********************  TRANSLATION.CPP  ********************
#include <Common.h>
#include "Translation.h"
#include "Control.h"

void PackByte( unsigned char Code, unsigned char Data, BUFFER *buffer)
{
  if (Code) PushBuffer(buffer, Code);
  PushBuffer(buffer, 0x30 | ((Data & 0xF0) >> 4));
  PushBuffer(buffer, 0x30 | (Data & 0x0F));
}

void packData( unsigned char Code, unsigned char Data, BUFFER *buffer)
{
  if (Code) PushBuffer(buffer, Code);
  PushBuffer(buffer, Data);
}

void packDataEnd( unsigned char Code, unsigned char Data, BUFFER *buffer)
{
  if (Code) PushBuffer(buffer, Code);
  PushBuffer(buffer, Data);
  PushBuffer(buffer, END_MESSAGE);
}

//  функции для передачи версии ПО
void CopyStringFlash( BUFFER *buffer, unsigned char __flash *string)
{
    //unsigned char Count = 0x00;
    while (*string)
    {
      PushBuffer(buffer, *string++);
    }
    PushBuffer(buffer, *string);  // запись \0
}

void CopyString( BUFFER *buffer, unsigned char *string)
{
    //unsigned char Count = 0x00;
    while (*string)
    {
      PushBuffer(buffer, *string++);
    }
    PushBuffer(buffer, *string);  // запись \0
}

void verFormat( BUFFER *buffer, unsigned char *string)
{
    //unsigned char Count = 0x00;
    while (*string)
    {
      PushBuffer(buffer, *string++);
    }
}

//функция для преобразования даты в формат ХХ ХХ ХХ
void dateFormat(BUFFER *buffer, unsigned char __flash *stringDate)
{
  unsigned char monCount = 0;
  int i = 0;
  
  //вычислим месяц
  if(stringDate[0] == 0x4A) //J..
  {
    if(stringDate[1] == 0x61) //Ja.. - Jan
    {
      monCount = 1;
    }
    else if(stringDate[1] == 0x75) //Ju..
    {
      if(stringDate[2] == 0x6E) //Jun
      {
        monCount = 6;
      }
      else if(stringDate[2] == 0x6C) //Jul
      {
        monCount = 7;
      }
    }
  }  
  if(stringDate[0] == 0x46) //Feb
  {
    monCount = 2;
  }
  if(stringDate[0] == 0x4D) //M..
  {
    if(stringDate[2] == 0x72) //Mar
    {
      monCount = 3;
    }
    else if(stringDate[2] == 0x79) //May
    {
      monCount = 5;
    }
  }
  if(stringDate[0] == 0x41) //A..
  {
    if(stringDate[2] == 0x72) //Apr
    {
      monCount = 4;
    }
    else if(stringDate[2] == 0x67) //Aug
    {
      monCount = 8;
    }
  }
  if(stringDate[0] == 0x53) //Sep
  {
    monCount = 9;
  }
  if(stringDate[0] == 0x4F) //Oct
  {
    monCount = 10;
  }
  if(stringDate[0] == 0x4E) //Nov
  {
    monCount = 11;
  }
  if(stringDate[0] == 0x44) //Dec
  {
    monCount = 12;
  }

  while(((stringDate[i] > 0x40) && (stringDate[i] < 0x5B)) || ((stringDate[i] > 0x60) && (stringDate[i] < 0x7B)))
  {
    i++;
  }
  while(stringDate[i] == 0x20)
  {
    i++;
  }
  
  //записываем день
  int flag = 1;
  while((stringDate[i] >= 0x30) && (stringDate[i] <= 0x39))
  {
    if(flag)
    {
      if((stringDate[i+1] >= 0x30) && (stringDate[i+1] <= 0x39))
      {
        PushBuffer(buffer, stringDate[i]);
        flag = 0;
      }
      else
      {
        PushBuffer(buffer, 0x30);
        PushBuffer(buffer, stringDate[i]);
      }
    }
    else
    {
      PushBuffer(buffer, stringDate[i]);
    }
    i++;
  }
  
  //записываем месяц
  if(monCount < 10)
  {
    PushBuffer(buffer, 0x30);
    PushBuffer(buffer, 0x30+(monCount&0x0F));
  }
  else
  {
    PushBuffer(buffer, 0x30+((monCount&0xF0)>>4));
    PushBuffer(buffer, 0x30+(monCount&0x0F));
  }
    
  //записываем год
  PushBuffer(buffer, stringDate[dateSize-3]);
  PushBuffer(buffer, stringDate[dateSize-2]);
}

void CopyStrDim1(unsigned char *pIn, unsigned char *pOut, unsigned char Row)
{
  for (unsigned char i = 0; i < Row; i++)
  {
    *pIn++ = *pOut++;
  }
}

void CopyStrDim2(unsigned char *pIn, unsigned char *pOut, unsigned char Row, unsigned char Col)
{
  for (unsigned char i = 0; i < Row; i++)
  {
    for (unsigned char j = 0; j < Col; j++)
    {
      *pIn++ = *pOut++;
    }
  }
}


//  ********************  TRANSLATION.CPP  ********************
