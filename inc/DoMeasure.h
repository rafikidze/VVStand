//  ********************  DOMEASURE.H  ********************
#define MASK_AVERAGE_INDEX  0x03
#define MAX_AVERAGE_INDEX   0x04

#ifdef CORRECT_ZERO
#define OP_SHIFT            0x18
#else
#define OP_SHIFT            0
#endif

//BASE
//  с измер трансформаторов
#define I_OUT_HF            0x00
#define U_OUT_HF            0x01
//  управляющие сигналы (ОС)
#define I_OUT_CTRL         0x02
#define U_OUT_CTRL         0x03

//  ADD
//  c датчиков
#define I_OUT_LEM          0x00
#define U_OUT_LEM          0x01

extern unsigned long TaktLimitLEM;   //  10kHz

typedef struct _parameter_summator
{
  double Error;
  unsigned long nSample;
  CALIBRATE_CONSTANT ReCount;
} PARAMETER_SUMMATOR;

typedef struct _summator_out
{
  unsigned short Max;   //  максимальное
  unsigned short Min;   //  минимальное
  unsigned short Ave;   //  среднее
} SUMMATOR_OUT;

typedef struct _parameter_measure
{
  unsigned long   ValA;   //  накопление для усреднения
  unsigned short  Max, Min;
  unsigned char   IndexA; //  куда писать
  unsigned char   ReadyA; //  флаг готовности данных
  SUMMATOR_OUT    DataA[MAX_AVERAGE_INDEX];
  CALIBRATE_CONSTANT Calibrate;
  /*
  unsigned long  CountA;
  unsigned char  ShiftA;
  unsigned long  ValF;  //  фильтрация
  unsigned short Out;   //  НЧ фильтр
  unsigned short FactorF;
  unsigned char  ShiftF;
  */
} PARAMETER_MEASURE;

//extern PARAMETER_MEASURE MeasureBase[MAX_CHANNEL_BASE];
extern PARAMETER_MEASURE MeasureAdd[MAX_CHANNEL_ADD];
extern PARAMETER_SUMMATOR Summator;

void ClearMeasure(PARAMETER_MEASURE *pMeasure);
void InitMesure(void);
void AddData(unsigned long *pMeasure, unsigned short Data);
void ChangeIndex(unsigned long *pMeasure);

unsigned char CountCalibrate(CALIBRATE_CONSTANT *pCalibrate, double *pData, double *pError);
void SetCalibrate(CALIBRATE_CONSTANT *pCalibrate, unsigned short Factor, unsigned char Shift, short Add); // исправила 11.05.16

__monitor unsigned short GetAve(PARAMETER_MEASURE *pMeasure);
unsigned short GetMin(PARAMETER_MEASURE *pMeasure);
unsigned short GetMax(PARAMETER_MEASURE *pMeasure);

void DoMeasure(void);
void SetMeasureResult(void);

//  ********************  DOMEASURE.H  ********************


