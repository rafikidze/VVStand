//  ********************  ADC_BASE.CPP  ********************
#include <Common.h>
#include "ADC_base.h"
#include "Control.h"
#include "TimerBase.h"
#include "Communication.h"

//CADC ADC_base; //
//short int   ADC_SAMPLE_BASE[ADC_FIFO_SIZE][MAX_CHANNEL_BASE];



/*InterruptServiceRoutine(ADCA_CH1_vect)
{
  unsigned char _t;
  _t = __save_interrupt();
  __disable_interrupt();

  ADC_base.psData = &ADC_SAMPLE_BASE[ADC_base.Count & ADC_FIFO_LIMIT][0];
  ADC_base.Do();

  __restore_interrupt(_t);
}*/

//  ********************  ADC_BASE.CPP  ********************
