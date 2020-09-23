//  ********************  TRANSFORMDOUBLE.CPP  ********************
#include <Common.h>
#include <math.h>
#include "Transformation.h"
#include "TransformDouble.h"

void DoWhiteSpace( unsigned char *str, unsigned char Pos)
{
  unsigned char Index;
  for (Index = 0; Index < Pos; Index++)
  {
    *str++ = WHITE_SPACE;
  }
}

unsigned short StrToDbl( unsigned char *str, double *pData, unsigned char Pos, unsigned char PPos)
{
  //  преобразование строки в число типа double
  unsigned char *pStr = str;
  unsigned char Index,
                Error = 0,
                Data,
                Point = 0,
                Digit = 0;    //  кол-во преобразованных символов
  unsigned short usA = 0;
  unsigned long ulA = 0;
  double  A = 0,  //A += 1.0E-11;
          B = 1.0;

  for (Index = 0; Index < Pos; Index++)
  {
    Data = *pStr++;
    if ((Data == 0x2E) || (Data == 0x2C))
    {
      if (Point)
      {// обработка ошибки - 2 разделител€ разр€да
        Error |= TRANSF_ERR_TWOPOINT;
        goto skip_end;
      }
      else
      {
        if (Digit > PPos)
        {
          Error |= TRANSF_ERR_MANYDIGIT;
          goto skip_end;
        }
        else
        {
          Point++;
          Digit++;
        }
      }
    }
    else
    {
      if (Data == 0x20)
      {
        if (Digit)
        {
          goto skip_end;
        }
        else;
      }
      else
      {
        if (Data > 0x2F && Data < 0x3A)
        {
          if (Point)
          {
            B /= 10;  //  ? можно ¬ задать в виде массива {0.1; 0.01 ...}
            A += B * (Data & 0x0F);
          }
          else
          {
            ulA = ((ulA << 2) + ulA) << 1;
            ulA += Data & 0x0F;
          }
          Digit++;
        }
        else
        {
          // обработка ошибки - не цифра
          Error |= TRANSF_ERR_NOTDIGIT;
          goto skip_end;
        }
      }
    }
  }


skip_end:
  if ((Error > 0) || (Digit == 0x00))
  {
      usA = ((unsigned short) Error) << 8;
      usA |= Digit;
  }
  else
  {
    if (A) A += ADD_ROUND_ERROR;
    *pData = ((double) ulA) + A;
    usA = Digit;
  }

  return usA;
}

unsigned short DblToStr( unsigned char *str, double Data, unsigned char Pos, unsigned char PPos)
{
  //  преобразование числа типа double в строку
  unsigned char temp[8];
  unsigned char Index,
                Error = 0,
                Count = 0,
                Space = 0x00,
                ucData,
                Digit = 0;    //  кол-во преобразованных символов

  unsigned short usA;
  unsigned long ulA;
  double  B;

  B = Data;
  ulA = (unsigned long) floor(Data);
  B -= ulA;

  while (ulA)
  {
    temp[Count++] = ((unsigned char) (ulA % 10)) | 0x30;
    ulA /= 10;
    Digit++;
  }

  if (Digit);
  else
  {
    temp[Count++] = 0x30; //  ведущий ноль перед .
    Digit++;
  }

  if (Digit <= Pos);
  else
  {
    Error |= TRANSF_ERR_MANYDIGIT;
    goto skip_end;
  }

  for (Index = Digit; Index < (PPos - 3); Index++)
  {
    //temp[Count++] = 0x20; //  ведущие пробелы
    Space++;
  }

  for (Index = 1; Index <= Count; Index++)
  {
    *(str + Index - 1) = temp[Count - Index];
  }

  if (Count < Pos)
    *(str + Count++) = 0x2E; //0x2C(,) 0x2E(.)

  B += 5E-7;  //  округление последнего знака
  for (Index = Count; Index < (Pos - Space); Index++)
  {
    B *= 10;
    //if (Index == (Pos - Space - 1))
    //{
    //  ucData = (unsigned char) floor(B + 0.5);  //  округление последнего знака
    //  if (ucData > 9) ucData = 9;               //  откидываем, чтобы не переносить разр€ды
    //}
    //else

    ucData = (unsigned char) floor(B);

    B -= (double) ucData;
    *(str + Index) = ucData | 0x30;
    Digit++;
    Count++;
  }

  for (Index = Count; Index < Pos; Index++)
  {
    *(str + Index) = 0x20;
  }


skip_end:
  if ((Error > 0) || (Digit == 0x00))
  {
      usA = ((unsigned short) Error) << 8;
      usA |= Digit;
  }
  else
  {
    usA = Digit;
  }

  return usA;
}

unsigned char StrToUChar( unsigned char *str, unsigned char *pData, unsigned char Pos)
{
  //  преобразование строки в число типа  unsigned char
  unsigned char *pStr = str;
  unsigned char Index,
                Value = 0,
                Error = 0,
                Data,
                Digit = 0;    //  кол-во преобразованных символов

  for (Index = 0; Index < Pos; Index++)
  {
    Data = *pStr++;
    if (Data == 0x20)
    {
      if (Digit)
      {
        goto skip_end;
      }
      else;
    }
    else
    {
      if (Data > 0x2F && Data < 0x3A)
      {
        Value = ((Value << 2) + Value) << 1;
        Value += Data & 0x0F;
        Digit++;
      }
      else
      {
        // обработка ошибки - не цифра
        Error |= TRANSF_ERR_NOTDIGIT;
        goto skip_end;
      }
    }
  }


skip_end:
  if ((Error > 0) || (Digit == 0x00))
  {
      Value = Error << 4;
      Value |= Digit;
  }
  else
  {
    *pData = Value;
    Value = Digit;
  }

  return Value;
}

unsigned char UCharToStr( unsigned char *str, unsigned char Data, unsigned char Pos)
{
  //  преобразование числа типа <unsigned char> в строку
  unsigned char temp[4];
  unsigned char Index,
                Error = 0,
                Count = 0,
                Digit = 0;    //  кол-во преобразованных символов

  while (Data)
  {
    temp[Count++] = (Data % 10) | 0x30;
    Data /= 10;
    Digit++;
  }

  if (Digit <= Pos);
  else
  {
    Error |= TRANSF_ERR_MANYDIGIT;
    goto skip_end;
  }

  for (Index = 0; Index < (Pos - Count); Index++)
  {
    *str++ = 0x30;  //  ведущие пробелы
  }

  for (Index = 1; Index <= Count; Index++)
  {
    *str++ = temp[Count - Index];
  }

skip_end:

  return ((Error << 4) | Digit);
}


unsigned char StrToUShort( unsigned char *str, unsigned short *pData, unsigned char Pos)
{
  //  преобразование строки в число типа  unsigned char
  unsigned char *pStr = str;
  unsigned short Value = 0;
  unsigned char Index,
                Error = 0,
                Data,
                Digit = 0;    //  кол-во преобразованных символов

  for (Index = 0; Index < Pos; Index++)
  {
    Data = *pStr++;
    if (Data == 0x20)
    {
      if (Digit)
      {
        goto skip_end;
      }
      else;
    }
    else
    {
      if (Data > 0x2F && Data < 0x3A)
      {
        Value = ((Value << 2) + Value) << 1;
        Value += Data & 0x0F;
        Digit++;
      }
      else
      {
        // обработка ошибки - не цифра
        Error |= TRANSF_ERR_NOTDIGIT;
        goto skip_end;
      }
    }
  }

skip_end:
  if ((Error > 0) || (Digit == 0x00));
  else
  {
    *pData = Value;
  }

  return ((Error << 4) | Digit);
}

unsigned char UShortToStr( unsigned char *str, unsigned short Data, unsigned char Pos)
{
  //  преобразование числа типа <unsigned char> в строку
  unsigned char temp[4];
  unsigned char Index,
                Error = 0,
                Count = 0,
                Digit = 0;    //  кол-во преобразованных символов

  while (Data)
  {
    temp[Count++] = (Data % 10) | 0x30;
    Data /= 10;
    Digit++;
  }

  //if (Digit);
  //else
  //{
  //  temp[Count++] = 0x30; //  ведущий ноль
  //  Digit++;
  //}

  if (Digit <= Pos);
  else
  {
    Error |= TRANSF_ERR_MANYDIGIT;
    goto skip_end;
  }

  for (Index = 0; Index < (Pos - Count); Index++)
  {
    *str++ = 0x30;  //  ведущие пробелы
  }

  for (Index = 1; Index <= Count; Index++)
  {
    *str++ = temp[Count - Index];
  }

skip_end:

  return ((Error << 4) | Digit);
}


//  ********************  TRANSFORMDOUBLE.CPP  ********************
