
//  ********************  CONTROL.H  ********************

//DACA
//DACB

#define DACA_FACTOR     28  //56
#define DACA_SHIFT      1
#define DACA_STEP       0x80  //64+6

#define DACB_FACTOR     109
#define DACB_SHIFT      1
#define DACB_STEP       0x80

/////////////////////////////////////////////////////////
#define YES               0x06
#define NO                0x15

#define COUNT_REGISTER_LED 0x04
#define TIME_CHECK_LEDS    200
#define TIME_TRANZISTOR    1
#define TIME_COMMAND       100
#define TIME_REQUEST       200
#define ZERO               0x3F
#define ONE                0x06
#define TWO                0x5B
#define THREE              0x4F
#define FOUR               0x66
#define FIVE               0x6D
#define SIX                0x7D
#define SEVEN              0x07
#define EIGHT              0x7F
#define NINE               0x6F
#define DP                 0x80
#define MINUS              0x40
#define LE_D8              0x20
#define LE_D9              0x40
#define DIGS               12
// коды кнопок
#define B_VENT_ON          0x01
#define B_VENT_OFF         0x02
#define B_380_ON           0x04
#define B_380_OFF          0x08
#define B_SIRENA           0x10
#define B_VVG_OTP          0x20
#define B_VVG_ZAP          0x40
#define B_PUSK             0x01
#define B_SBROS            0x02
#define B_SOUND            0x04
#define B_FAIL             0x08
// РЕГИСТРЫ
#define D4                  0x00
#define D5                  0x01
#define D6                  0x02
#define D7                  0x03

#define REG_D0              0x01
#define REG_D1              0x02
#define REG_D2              0x04
#define REG_D3              0x08
#define REG_D4              0x10
#define REG_D5              0x20
#define REG_D6              0x40
#define REG_D7              0x80
#define MAX_ERROR           5



extern unsigned char Command, CommandStep;
extern unsigned char  SetInit;

//  описание
extern unsigned char __flash Date[];
extern unsigned char __flash Time[];
extern unsigned char dateSize;

//////////////////////////////////////////////////////////////
extern unsigned char registersD[COUNT_REGISTER_LED];
extern unsigned char registersD_LE[COUNT_REGISTER_LED];
extern unsigned char sevenSegmentD8[DIGS];
extern unsigned char sevenSegmentD9[DIGS-4];
extern unsigned char tranzistorsPortQA[DIGS];
extern unsigned char StepLed, CurrentV;
extern unsigned char RequestStep;
extern unsigned char CommandCount, RequestCount;
extern unsigned char Send_VENT, Send_TIME, Send_380, Send_SIRENA, Send_VVG_OTP, Send_DEBLOCK;
extern int Send_VVG;
extern unsigned char flag_answer, flag_timeout_request; // 1 - YES, 0 - NO
extern unsigned char flag_chande_leds; 
extern unsigned short TimeLeds, TimeDelay, TimeTranzistor, TimeCommand, TimeCommon;
extern unsigned char fail_skin, fail_ozon, fail_vvg;
extern unsigned char Deblock;


typedef struct _CUBS {
  unsigned char fire,
                vent_on,
                c_byte_h,
                c_byte_l,
                zoi,
                tbk,
                set1_380,
                seth,
                rmb,
                door_zoi,
                door_tbk;
} CUBS;

typedef struct _CCOMPONENT {
  unsigned char ubs,
                vvg,
                datch_h,
                datch_l,
                skin,
                tbk;
} CCOMPONENT;

typedef struct _CVVG {
  unsigned char fail_h,
                fail_l,
                state;
  unsigned char U1[4],
                I1[4],
                U2[4],
                I2[4];
} CVVG;

typedef struct _CDATCHIK {
  unsigned char t_zoi[4],
                t_tbk[4],
                vl_zoi[4],
                p_tbk[4],
                time[4];
} CDATCHIK;

extern CUBS UBS;
extern CCOMPONENT COMPONENT;
extern CVVG VVG;
extern CDATCHIK DATCHIK;

void Control(void);
void DoCommand(void);
void DoRequest(void);
void DoTimer(void);

void ClearCommand(void);

unsigned char checkDiods ();
unsigned char checkSevenDiods ();
void TurnOnLeds (unsigned char, unsigned char);
void TurnOnSevenSegments (unsigned char, unsigned char);
void SetNumbersOnSevenSegmentsD9(unsigned char *array, unsigned char index, unsigned char DP_); 
void SetNumbersOnSevenSegmentsD8(unsigned char *array, unsigned char index, unsigned char DP_); 
void SetDataFromRequestUBS ();
void SetDataFromRequestCOMPONENT();
void SetDataFromRequestVVG();
void SetDataFromRequestDATCHIK();

//  ********************  CONTROL.H  ********************

