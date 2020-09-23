
//  ********************  DEFINE_ALL.H  ********************

//#define F_Quartz        0x01E84800  //  32 ћ√ц
//#define F_Quartz        0x001E8480  //  2 ћ√ц

//#define       F_QuartzM                     0x20      // 32 ћ√ц
//#define       F_QuartzM                     0x02      // 2 ћ√ц
#define       FREQ_1M                       0x000F4240L
#define       FREQ_2M                       0x02
#define       FREQ_32M                      0x20
#define       FREQ_EXT                      0x10
//  флаги дл€ OSC
#define   SET_OSC_PLL_EN      0x10
#define   SET_OSC_EXT_EN      0x08
#define   SET_OSC_32k_EN      0x04
#define   SET_OSC_32M_EN      0x02
#define   SET_OSC_2M_EN       0x01

#define   SET_OSC_32M         0x01
#define   SET_OSC_EXT         0x03
#define   SET_OSC_PLL         0x04

#define   PLL_FACTOR_2        0x02    //  4
#define   PLL_FACTOR_4        0x04    //  4
#define   PLL_FACTOR_8        0x08    //  8
#define   PLL_FACTOR_16       0x10    //  64
#define   PLL_FACTOR_24       0x18
#define   PLL_FACTOR_32       0x20    //  64
#define   PLL_SOURCE_2M       0x40    //  2M
#define   PLL_SOURCE_32M      0x80    //  32M
#define   PLL_SOURCE_EXT      0xC0    //  32M
//  флаги дл€ OSC

#define PORT0     0x01
#define PORT1     0x02
#define PORT2     0x04
#define PORT3     0x08
#define PORT4     0x10
#define PORT5     0x20
#define PORT6     0x40
#define PORT7     0x80

#define BIT_0     0x01
#define BIT_1     0x02
#define BIT_2     0x04
#define BIT_3     0x08
#define BIT_4     0x10
#define BIT_5     0x20
#define BIT_6     0x40
#define BIT_7     0x80

#define TIME_IN_MKC                 0x01
#define PUSH_F_VIEW_ALL             0x01

typedef struct _calibrate_constant
{
  unsigned short Factor;
  unsigned char Shift;
  short Add;         // исправила 11.05.16
} CALIBRATE_CONSTANT;

extern unsigned long F_Quartz, F_QuartzM;

extern unsigned short DivA, DivSys;
extern unsigned char  DivB, DivC;

//  ********************  DEFINE_ALL.H  ********************

