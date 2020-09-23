//   **************************   Inner.h     **************************
#define NBP 0 //напряжение на выходе источника
#define TBP 1 //ток на выходе источника
#define I_MAX 0x32 //максимальный ток 5А

#define VP1 0 // вторичный источник питания +5В
#define VP2 1 // вторичный источник питания +12В
#define VP3 2 // вторичный источник питания -12В
#define VP4 3 // вторичный источник питания +15В

extern unsigned short  vp1_par, vp3_par, vp4_par, tok_par,nbp_par;
/////////////////////////////////////////////////////
extern unsigned short U_VVG_circle_prev, U_VVG_circle;



//коды аварий
#define NO_ERROR 0x00 //БП исправен
#define END_MESS 0x03 //конец текста


unsigned char ones(unsigned char x);
unsigned char ControlSumMod2(BUFFER *buffer, unsigned char Tail, unsigned char *pCount);
unsigned char IsInnerEnd(BUFFER *);
//unsigned char IsOneByteInner(unsigned char );
void DoInnerMessage( unsigned char , BUFFER *);
//void ErrorMessageInner(unsigned char Code, BUFFER *buffer); // к ДУ
void IsCommandInner(BUFFER *);
unsigned char ReceiveInner(BUFFER *);

void GetPatameters(void);

//   ********************************************************************************


