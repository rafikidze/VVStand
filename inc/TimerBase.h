
//  ********************  TIMER_BASE.H  ********************

//  отладка - такты контроля времени исполнения
#define TIME_TAKT               TIME0_0
#define TIMER_TAKT              TIMER0_0
//  отладка - такты контроля времени исполнения

//  ********* такты опроса портов *********
#define TIME_GET                TIME0_1
#define TIMER_GET               TIMER0_1
//  ********* такты опроса портов *********

//  ********* такты ADC *********
#define TIME_ADC                TIME0_2
#define TIMER_ADC               TIMER0_2
//  ********* такты ADC *********

//  ********* такты DAC *********
//#define TIME_DAC                TIME0_3
//#define TIMER_DAC               TIMER0_3
//  ********* такты DAC *********

// ****************** работа с клавиатурой *****************//
#define TIME_KEY               TIME0_4
#define TIMER_KEY              TIMER0_4
//  отладка - ложный отказ при сбросе питания

// ****************** работа с дисплеем *****************//
//#define TIME_LCD                TIME0_5
//#define TIMER_LCD               TIMER0_5
// ****************** работа с дисплеем *****************//

// ****************** работа с дисплеем отмена изменений *****************//
//#define TIME_CANCEL_CHANGE      TIME0_6
//#define TIMER_CANCEL_CHANGE     TIMER0_6
// ****************** работа с дисплеем отмена изменений  *****************//

// ****************** опрос отказов PLM *****************//
//#define TIME_CONTROL            TIME0_7
//#define TIMER_CONTROL           TIMER0_7
// ******************  опрос отказов PLM   *****************//

//  изменение управляющего напряжения

extern CTIMER0 Timer_C0;

extern unsigned long  TimeBase,
                      TimeBaseOn,
                      TimeBaseOff;

extern unsigned short TimerBase[8],
                      EndTimerBase[8];

extern unsigned char  FlagTimerBase,
                      IsTimeBase,
                      DoTimeBase;

//  прототипы функций
void InitTimerBase(unsigned long );
unsigned char IsTimerBase(void);
__monitor void ResetTimerBase(void);
__monitor void CloseTimerBase(void);
__monitor void StartTimerBase(unsigned char Data);
__monitor void ReStartTimerBase(unsigned char Data);
__monitor void StopTimerBase(unsigned char Data);
__monitor unsigned short GetTimerBase(void);
__monitor void SetTimerBase(unsigned char Data, unsigned short Time);

//  ********************  TIMER_BASE.H  ********************

