//  ********************  ADC.CPP  ********************
#include <Common.h>

short int       ADC_Offset = 6;

unsigned char   ADC_PreScaler = 0,
                ADC_PSC_MIN = 0;

#ifdef PEAK
#endif

//InterruptServiceRoutine(ADCB_CH0_vect)  //  дл€ смещени€
//{
//}

//InterruptServiceRoutine(ADCB_CH3_vect)  //  дл€ измерений
//{
//}

void CADC::Init(  ADC_t *pField,
                  unsigned char Control,
                  unsigned char Event,
                  unsigned char Start,
                  unsigned char StopChannel,
                  unsigned char IntMode,
                  unsigned char IntLevel,
                  unsigned char Cal0,
                  unsigned char Cal1,
                  unsigned char Config,
                  unsigned char Reference,
                  unsigned char PreScaler)
{
  unsigned char _t, *Tmp;
  _t = __save_interrupt();
  __disable_interrupt();

  pADC = pField;

  Tmp = (unsigned char *) (PROD_SIGNATURES_START + Cal0);
  NVM_CMD = NVM_CMD_READ_CALIB_ROW;
//  Get ADC(A/B)CAL0 from production signature
  pADC->CALL = *Tmp;
  NVM_CMD = NVM_CMD_NO_OPERATION;
  Tmp = (unsigned char *) (PROD_SIGNATURES_START + Cal1);
  NVM_CMD = NVM_CMD_READ_CALIB_ROW;
  pADC->CALH = *Tmp;
  NVM_CMD = NVM_CMD_NO_OPERATION;

//  CTRLA
  ucControl = Control;

//  CTRLB
  //  7..5 Reserved
  //  4 CONVMODE (0 - unsigned; 1 - signed) надо устанавливать 1
  //  3 FREERUN  (?)
  //  2..1 RESOLUTION 00-12bit;  01-reserved;  10-8bit; 11-12bit left.
  //  0 Reserved
  pADC->CTRLB = Config;//ADC_CONVMODE | ADC_RESOLUTION_12R; // | (0 << 3)//ADC_FREERUN

//  Set reference voltage   bit - 10 AREFB
  pADC->REFCTRL = Reference;

//    ADC_PreScaler =  ADC_PSC_MIN + 1;   // 1000 к√ц
//    ADC_PreScaler =  ADC_PSC_MIN + 2;   // 500 к√ц
//    ADC_PreScaler =  ADC_PSC_MIN + 3;   // 250 к√ц
//    ADC_PreScaler =  ADC_PSC_MIN + 4;   // 125 к√ц
//    ADC_PreScaler =  ADC_PSC_MIN + 5;   // 62.5 к√ц
  if (PreScaler < ADC_PSC_MIN)
    ucPreScaler = ADC_PSC_MIN;
  else
    ucPreScaler = PreScaler;

//  ƒо измерени€ смещени€ устанавливаем минимальный допустимый
  pADC->PRESCALER = ADC_PSC_MIN;

//
  ucEvent = Event;

  pADC->EVCTRL = ucEvent;

  ucStart = Start;
  ucStopChannel = StopChannel;
  ucIntMode = IntMode;
  ucIntLevel = IntLevel;

  Status = 0x00;
  Get = Count = 0;  //CountControl = 0;
  Done = 0;
  //DimensionControl = 0;

  ucSkip = 0x00;

//  Set Channel as Single-ended input
//      ChannelConfig( 0, ADC_SINGLEENDED, 0x00, ADC_CH_MUX_0, 0x00);
//      ChannelConfig( 1, ADC_SINGLEENDED, 0x00, ADC_CH_MUX_1, 0x00);
//      ChannelConfig( 2, ADC_SINGLEENDED, 0x00, ADC_CH_MUX_2, 0x00);
//      ChannelConfig( 3, ADC_SINGLEENDED, 0x00, ADC_CH_MUX_3, 0x00);


  __restore_interrupt(_t);
}

void CADC::ChannelConfig( unsigned char Index,
                          unsigned char Mode,
                          unsigned char Gain,
                          unsigned char MuxPos,
                          unsigned char MuxNeg)
{
  ADC_CH_t *pChannel = &pADC->CH0;
  pChannel += Index;
  pChannel->CTRL = Mode | (Gain << 2);
  pChannel->MUXCTRL = (MuxPos << 3) | (MuxNeg & 0x03);
}

void CADC::SetChannelInterrupt( unsigned char Index,
                              unsigned char Mode,
                              unsigned char Level)
{
  ADC_CH_t *pChannel = &pADC->CH0;
  pChannel += Index;
  pChannel->INTCTRL = (Mode << 2) | (Level);
}

void CADC::StartChannel( unsigned char Index)
{
  ADC_CH_t *pChannel = &pADC->CH0;
  pChannel += Index;
  if (pChannel->INTFLAGS) pChannel->INTFLAGS |= ADC_ENABLE;
  pChannel->CTRL |= ADC_CH_START;
}

unsigned char CADC::ChannelReady( unsigned char Index)
{
  ADC_CH_t *pChannel = &pADC->CH0;
  pChannel += Index;
  return pChannel->INTFLAGS;
}

short int CADC::GetChannel( unsigned char Index)
{
  ADC_CH_t *pChannel = &pADC->CH0;
  pChannel += Index;
  if (pChannel->INTFLAGS) pChannel->INTFLAGS |= ADC_ENABLE;
  return pChannel->RES;
}

void   CADC::Start(    void)
{
  ADC_CH_t *pChannel = &pADC->CH0;
  pChannel += ucStopChannel;

//  закрываем предыдущие измерени€
  //pADC->CTRLA = 0x00;
  //pADC->EVCTRL = 0x00;
  ucSkip = ADC_SKIP;  //  ложное измерение - сброс в прерывании

  Status = ADC_DO;
  Get = Count = 0x00;
  Done = 0;
  //Measure = (1 << ADC_MeasureNumber) - 1; //

  pADC->PRESCALER = ucPreScaler;
  SetChannelInterrupt(  ucStopChannel, ucIntMode, ucIntLevel);
  pADC->CTRLA = (ucControl | ADC_ENABLE);

}

void   CADC::ReStart(    void)//  повторный пуск - успеть обработать
{
  Status &= ~(ADC_FIFO | ADC_COMPLETE);
  Get = Count = 0;  //CountControl = 0;
  Done = 0;
  pADC->CTRLA |= ucControl;
}

void   CADC::SetSkip(  unsigned char Data)
{
  ucSkip = Data;
}

unsigned char CADC::GetSkip( void )
{
  return ucSkip;
}

void   CADC::Stop(    void)//
{

  pADC->CTRLA = 0x00;
  //pADC->EVCTRL = 0x00;
  SetChannelInterrupt(  ucStopChannel, 0x00, 0x00);
  pADC->CTRLA = 0x00;
  Status = 0x00;
  Get = Count = 0;  //CountControl = 0;
  Done = 0;
  //DimensionControl = 0;

}

void CADC::Do(void)
{
  unsigned char _t;
  short int *pIn, *pOut;

  _t = __save_interrupt();
  __disable_interrupt();

  if (ucSkip & ADC_SKIP) //  пропускаем ложное измерение
  {
    ucSkip = ucSkip << 1;
    //pADC->CTRLA |= ucControl;
  }
  else
  {
    // *********** выполн€етс€ 135 тактов *********
    pIn = psData;
    pOut = (short int*) &pADC->CH0RES;
    for (unsigned char i = 0; i <= ucStopChannel; i++)
    {
      *pIn++ = *pOut++;
    }

    //if ((Count & Measure) == Measure)   //  4 измерени€ на канал
    //{
    Status |= ADC_COMPLETE;
    //  CountControl++;
    //}

    Count++;

    if ((Count - Get) >= ADC_HALF_FIFO_LIMIT)
    {// не успеваем обрабатывать
      Status |= ADC_FIFO;
      //Stop();
    }
    else
    {
      //pADC->CTRLA |= ucControl;
    }
  }


__restore_interrupt(_t);
}

//  ********************  ADC.CPP  ********************
