//  ********************  DOMEASURE.H  ********************
#define MASK_AVERAGE_INDEX  0x03
#define MAX_AVERAGE_INDEX   0x04

#ifdef CORRECT_ZERO
#define OP_SHIFT            0x18
#else
#define OP_SHIFT            0
#endif

//BASE
//  � ����� ���������������
#define I_OUT_HF            0x00
#define U_OUT_HF            0x01
//  ����������� ������� (��)
#define I_OUT_CTRL         0x02
#define U_OUT_CTRL         0x03

//  ADD
//  c ��������
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
  unsigned short Max;   //  ������������
  unsigned short Min;   //  �����������
  unsigned short Ave;   //  �������
} SUMMATOR_OUT;

typedef struct _parameter_measure
{
  unsigned long   ValA;   //  ���������� ��� ����������
  unsigned short  Max, Min;
  unsigned char   IndexA; //  ���� ������
  unsigned char   ReadyA; //  ���� ���������� ������
  SUMMATOR_OUT    DataA[MAX_AVERAGE_INDEX];
  CALIBRATE_CONSTANT Calibrate;
  /*
  unsigned long  CountA;
  unsigned char  ShiftA;
  unsigned long  ValF;  //  ����������
  unsigned short Out;   //  �� ������
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
void SetCalibrate(CALIBRATE_CONSTANT *pCalibrate, unsigned short Factor, unsigned char Shift, short Add); // ��������� 11.05.16

__monitor unsigned short GetAve(PARAMETER_MEASURE *pMeasure);
unsigned short GetMin(PARAMETER_MEASURE *pMeasure);
unsigned short GetMax(PARAMETER_MEASURE *pMeasure);

void DoMeasure(void);
void SetMeasureResult(void);

//  ********************  DOMEASURE.H  ********************


