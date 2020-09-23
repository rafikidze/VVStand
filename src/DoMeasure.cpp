//  ********************  DOMEASURE.CPP  ********************
#include <Common.h>
#include <math.h>
#include "ADC_base.h"
#include "ADC_add.h"
#include "DoMeasure.h"


//PARAMETER_MEASURE MeasureBase[MAX_CHANNEL_BASE];
PARAMETER_MEASURE MeasureAdd[MAX_CHANNEL_ADD];
PARAMETER_SUMMATOR Summator = {0.0, 128, {1, 7, 0}};//{0.0, 2048, {1, 11, 0}};   //  102.4 мс  ё
//PARAMETER_SUMMATOR Summator = {0.0, 256, {1, 8, 0}}; //  12.8 мс

unsigned char CountCalibrate(CALIBRATE_CONSTANT *pCalibrate, double *pData, double *pError)
{
  unsigned char Data;
  return Data;
}

void SetCalibrate(CALIBRATE_CONSTANT *pCalibrate, unsigned short Factor, unsigned char Shift, short Add) // исправила 11.05.16 
{
  pCalibrate->Factor = Factor;
  pCalibrate->Shift = Shift;
  pCalibrate->Add = Add;
}

void SetMeasureResult(void)
{
  unsigned short Temp;
  
 
  
  
  
}

void ClearMeasure(unsigned long *pMeasure)
{
  unsigned short *pS;
  unsigned char *pC;
  *pMeasure++ = 0;
  pS = (unsigned short *) pMeasure;
  //Max
  *pS++ = 0;
  //Min
  *pS++ = 0x1000;
  pC = (unsigned char *) pS;

  *pC++ = 0;
  *pC++ = 0;
}

void InitMesure(void)
{
  unsigned char _t;
 /* for (unsigned char i = 0; i <= ADC_base.ucStopChannel; i++)
  {
    ClearMeasure(&MeasureBase[i].ValA);
  }*/

  for (unsigned char i = 0; i <= ADC_add.ucStopChannel; i++)
  {
    ClearMeasure(&MeasureAdd[i].ValA);
  }
  _t = __save_interrupt();
  __disable_interrupt();
  //ADC_base.ReStart();
  ADC_add.ReStart();
__restore_interrupt(_t);

}

void AddData(unsigned long *pMeasure, unsigned short Data)
{
  unsigned long *pL = pMeasure;
  unsigned short *pS;
  *pL++ += Data;    //pMeasure->ValA += Data;
  pS = (unsigned short *) pL;
  //Max
  if (Data > *pS) *pS = Data;   //(Data > pMeasure->DataA[pMeasure->IndexA].Max)
  pS++;
  //Min
  if (Data < *pS) *pS = Data;   //(Data < pMeasure->DataA[pMeasure->IndexA].Min)
}

void ChangeIndex(unsigned long *pMeasure)
{
  unsigned long *pL = pMeasure; //  ValA
  unsigned long Data;
  unsigned short *pS, *pSIn;
  unsigned char *pC;
  Data = *pL++;

  pS = (unsigned short *) pL;   //  Max

  if (Summator.ReCount.Factor > 1)
  {
    Data *= Summator.ReCount.Factor;
  }

  Data = (unsigned short) (Data >> Summator.ReCount.Shift); 
  //Data += Summator.ReCount.Add;

  pC = ((unsigned char *) pS) + 4;    //Index

  pSIn = pS + 3 + (*pC + (*pC << 1));

  *pMeasure = 0;
  //Max
  *pSIn++ = *pS;
  *pS++ = 0;
  //Min
  *pSIn++ = *pS;
  *pS++ = 0x1000;
  //Ave
  *pSIn++ = Data;

  *pC += 1;
  if (*(pC + 1));
  else
  {
    if (*pC > MASK_AVERAGE_INDEX)
    {
      *(pC + 1) += 1;
    }
  }
  *pC &= MASK_AVERAGE_INDEX;
}

__monitor unsigned short GetAve(PARAMETER_MEASURE *pMeasure)
{
  long Data;
  unsigned char Index = (pMeasure->IndexA - 1) & MASK_AVERAGE_INDEX;
  Data = (long) pMeasure->DataA[Index].Ave;
  
  if (Data +(long)  pMeasure->Calibrate.Add > 0) // --------- ё
    Data += (long) pMeasure->Calibrate.Add;       //  ё
  else Data = 0;                          // ---------
  //Data += pMeasure->Calibrate.Add;    11.05.16

  if (pMeasure->Calibrate.Factor > 1)
    Data *= (long) pMeasure->Calibrate.Factor;
  Data = Data >> pMeasure->Calibrate.Shift;
  
  return (unsigned short) Data;
}

unsigned short GetMin(PARAMETER_MEASURE *pMeasure)
{
  unsigned long Data;
  unsigned char Index = (pMeasure->IndexA - 1) & MASK_AVERAGE_INDEX;
  Data = (unsigned long) pMeasure->DataA[Index].Min;
  Data +=  (unsigned long) pMeasure->Calibrate.Add; //ё
  /*
  if (CurrentParameter.ucOnOff)   Data += pMeasure->Calibrate.Add;
  else  {   //  поправка 0
    if (Data > OP_SHIFT) Data -= OP_SHIFT;
    else Data = 0;
  }
  */
  if (pMeasure->Calibrate.Factor > 1)
    Data *= (unsigned long) pMeasure->Calibrate.Factor;
  Data = Data >> pMeasure->Calibrate.Shift;
  return (unsigned short) Data;
}

unsigned short GetMax(PARAMETER_MEASURE *pMeasure)
{
  unsigned long Data;
  unsigned char Index = (pMeasure->IndexA - 1) & MASK_AVERAGE_INDEX;
  Data = (unsigned long) pMeasure->DataA[Index].Max;
  Data += (unsigned long) pMeasure->Calibrate.Add; //ё
  /*
  if (CurrentParameter.ucOnOff)   Data += pMeasure->Calibrate.Add;
  else  {   //  поправка 0
    if (Data > OP_SHIFT) Data -= OP_SHIFT;
    else Data = 0;
  }
  */
  if (pMeasure->Calibrate.Factor > 1)
    Data *= (unsigned long) pMeasure->Calibrate.Factor;
  Data = Data >> pMeasure->Calibrate.Shift;
  return (unsigned short) Data;
}

void DoMeasure(void)
{
  short int *pOut;
  short int Tmp;
  unsigned char Count;
  unsigned short Data;

 /* if (ADC_base.Status & ADC_DO)
  {
    if (ADC_base.Status & ADC_COMPLETE) //  есть новые данные
    {
      unsigned char _t = __save_interrupt();
      __disable_interrupt();
      if (ADC_base.Get >= ADC_FIFO_SIZE)
      {
        ADC_base.Get -= ADC_FIFO_SIZE;
        ADC_base.Count -= ADC_FIFO_SIZE;
      }
      ADC_base.Status &= ~ADC_COMPLETE;
      Count =(unsigned char)( ADC_base.Count - ADC_base.Get); //ё
      __restore_interrupt(_t);

      for (unsigned char i = 0; i < Count; i++)
      {
        pOut = &ADC_SAMPLE_BASE[ADC_base.Get & ADC_FIFO_LIMIT][0];
        ADC_base.Get++;
        for (unsigned char i = 0; i <= ADC_base.ucStopChannel; i++)
        {
          Tmp = *pOut++;
          if (Tmp < 0) Tmp = -Tmp;
          Data = (unsigned short) Tmp;
          AddData(&MeasureBase[i].ValA, Data);
        } //  по каналам

        ADC_base.Done++;

        if (ADC_base.Done == (unsigned short) Summator.nSample) //ё
        {
          ADC_base.Done -=(unsigned short) Summator.nSample; //ё
          for (unsigned char i = 0; i <= ADC_base.ucStopChannel; i++)
          {
            ChangeIndex(&MeasureBase[i].ValA);
          }
        }
      }
    }
  }*/

  if (ADC_add.Status & ADC_DO)
  {
    if (ADC_add.Status & ADC_COMPLETE) //  есть новые данные
    {
      unsigned char _t = __save_interrupt();
      __disable_interrupt();
      if (ADC_add.Get >= ADC_FIFO_SIZE)
      {
        ADC_add.Get -= ADC_FIFO_SIZE;
        ADC_add.Count -= ADC_FIFO_SIZE;
      }
      ADC_add.Status &= ~ADC_COMPLETE;
      Count = (unsigned short) ADC_add.Count - ADC_add.Get; //ё
      __restore_interrupt(_t);

      for (unsigned char i = 0; i < Count; i++)
      {
        pOut = &ADC_SAMPLE_ADD[ADC_add.Get & ADC_FIFO_LIMIT][0];
        ADC_add.Get++;
        for (unsigned char i = 0; i <= ADC_add.ucStopChannel; i++)
        {
          Tmp = *pOut++;
          if (Tmp < 0) Tmp = -Tmp;
          Data = (unsigned short) Tmp;
          AddData(&MeasureAdd[i].ValA, Data);
        } //  по каналам

        ADC_add.Done++;

        if (ADC_add.Done ==(unsigned short) Summator.nSample)
        {
          ADC_add.Done -=(unsigned short) Summator.nSample;
          for (unsigned char i = 0; i <= ADC_add.ucStopChannel; i++)
          {
            ChangeIndex(&MeasureAdd[i].ValA);
          }
        }
      }
    }
  }
}
//  ********************  DOMEASURE.CPP  ********************

