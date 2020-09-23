//  ********************  ADC_ADD.CPP  ********************
#include <Common.h>
#include "ADC_add.h"
#include "Control.h"
#include "TimerBase.h"
#include "Communication.h"

CADC ADC_add; //
short int   ADC_SAMPLE_ADD[ADC_FIFO_SIZE][MAX_CHANNEL_ADD];

//ADC_add.Init(  &ADCB, CHN_01_START, 0x04, 0x01, 0x00, 0x02, ADCBCAL0, ADCBCAL1, ADC_CONVMODE | ADC_RESOLUTION_12R, REF_AREFA, ADC_PreScaler);
//      ADC_add.ChannelConfig( 0, ADC_SINGLEENDED, 0x00, ADC_CH_MUX_0, 0x00);
//      ADC_add.ChannelConfig( 1, ADC_SINGLEENDED, 0x00, ADC_CH_MUX_1, 0x00);

InterruptServiceRoutine(ADCB_CH0_vect)
{
  unsigned char _t;
  _t = __save_interrupt();
  __disable_interrupt();

  ADC_add.psData = &ADC_SAMPLE_ADD[ADC_add.Count & ADC_FIFO_LIMIT][0];
  ADC_add.Do();

  __restore_interrupt(_t);
}

//  ********************  ADC_ADD.CPP  ********************
