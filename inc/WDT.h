
//  ********************  WDT.H  ********************

#define       WDT_ENABLE        0x02
#define       WDT_CEN           0x01
#define       WDT_BUSY          0x01

#define       WDT_SET_8mc       0x00
#define       WDT_SET_16mc      0x01
#define       WDT_SET_32mc      0x02
#define       WDT_SET_64mc      0x03
#define       WDT_SET_125mc     0x04
#define       WDT_SET_250mc     0x05
#define       WDT_SET_500mc     0x06
#define       WDT_SET_1c        0x07
#define       WDT_SET_2c        0x08
#define       WDT_SET_4c        0x09
#define       WDT_SET_8c        0x0A

void WDT_Enable( void );
void WDT_Disable( void );
void WDT_SetTime( unsigned char TimeOut);

//  ********************  WDT.H  ********************



