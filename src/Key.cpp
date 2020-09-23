//  ********************  KEY.CPP  ********************
#include "Key.h"
#include "Control.h"
#include "Common.h"

//CKEY Key;
//void Key.Init( &PORTD, &PORTE, 0x0F, 0xF8, 0x01, 0x08)


//CKEY::strob_sirena = false;
void CKEY::init(void)
{
  prev_Get = 0x00;
  strob_sirena = false;
}
unsigned long CKEY::GetCode(void)   {
    return Last;
}

unsigned long CKEY::GetHead(void)   {
    return Head;
}

void CKEY::SetHead(void)   {
    Head = Last;
}

//  в обработчике предусмотреть 
//  для клавиш в определенных состояниях возможность длительного нажатия
//  принудительно устанавливать Key.SetEnable() - сохранять состояние НАЖАТА
void CKEY::SetEnable(void)  {
    Enable = TRUE;
}


bool CKEY::ScanPortK(void)
{
  bool IsSet = FALSE;
  unsigned char SetIn, Get, Count = 0x00;

  SetIn = 1;
  //  считываем значение
  Get = 0x0F - (PORTK.IN&0x0F);
  
  while (SetIn < 0x10)
  {
    if (Get & SetIn)
    {
      Count++;
    }
    SetIn = SetIn << 1;
  }
  
  if (Count)
  {
    if ( Enable && ( Count <= _KEY_PRESS_ENABLE_) && ( Get == Last) )
    {
      if (Repeat >= _KEY_REPEAT_NEED_)
      {
        Enable = FALSE;
        IsSet = TRUE;
      }
      else Repeat++;
    }
    else
    {
      Repeat = 1;
    }
  }
  else
  {
    if (Last)
    {
      Repeat = 1;
    }
    else
    {
      if (Repeat >= _KEY_REPEAT_NEED_)
      {
        Enable = TRUE;
        if (Head) IsSet = TRUE;     //  отжатие - контроль
      }
      else Repeat++;
    }
  }
  Last = Get;
  KeyBePressed = Count;
  
  return IsSet;
}
bool CKEY::ScanPortH(void)
{
  bool IsSet = FALSE;
  unsigned char SetIn, Get, Count = 0x00;

  SetIn = 1;
  //  считываем значение
  Get = 0xFF - PORTH.IN;
  
  if(((Get&0x10) == 0x00) && ((prev_Get&0x10) /*== 0x10*/))
    strob_sirena = true;
  else strob_sirena = false;
  
  prev_Get = Get;
  
  while (SetIn < 0x80)
  {
    //unsigned char test = (Get & SetIn);
    if (Get & SetIn)
    {
      Count++;
    }
    SetIn = SetIn << 1;
  }
  
  if (Count)
  {
    if ( Enable && ( Count <= _KEY_PRESS_ENABLE_) && ( Get == Last) )
    {
      if (Repeat >= _KEY_REPEAT_NEED_)
      {
        Enable = FALSE;
        IsSet = TRUE;
      }
      else Repeat++;
    }
    else
    {
      Repeat = 1;
    }
  }
  else
  {
    if (Last)
    {
      Repeat = 1;
    }
    else
    {
      if (Repeat >= _KEY_REPEAT_NEED_)
      {
        Enable = TRUE;
        if (Head) IsSet = TRUE;     //  отжатие - контроль
      }
      else Repeat++;
    }
  }
  Last = Get;
  KeyBePressed = Count;
  
  return IsSet;
}

bool CKEY::KeyRelease(void)
{

  return strob_sirena;
}
//  ********************  KEY.CPP  ********************
