
//  ********************  TRANSFORMATION.H  ********************

#define WHITE_SPACE   0x20
#define NULL_DIGIT    0x30
#define DIM_TEMP_STR  0x08
extern unsigned char sTemp[DIM_TEMP_STR];
unsigned long myabs(unsigned long x);

void longToA( BUFFER *, unsigned short , unsigned char , unsigned char );
void BinDecToA( BUFFER *, unsigned long int , unsigned char , unsigned char );

void ChangeHalfByteOrder(unsigned long int *, unsigned long int *, unsigned char);
void BinDecToLong( unsigned long int *, unsigned long int *);
void LongToBinDec( unsigned long int *, unsigned long int *, unsigned char);

unsigned char ToHexDec(unsigned char );
unsigned char ToHexDecHi(unsigned char );
unsigned char ToHexDecLo(unsigned char );

unsigned char InverseOrderBit(unsigned char );

void CopyBufferToStr(unsigned char *In, BUFFER *Out, unsigned char Shift, unsigned char Count);
unsigned short GetWord(unsigned char *str, unsigned char Digit);
unsigned char GetChar(unsigned char *str, unsigned char Digit);
unsigned char GetCharControl(BUFFER *In, unsigned char offset, unsigned char Count, unsigned char *pData);
unsigned char GetCharControlPower(BUFFER *In, unsigned char offset, unsigned char *pData);
unsigned char GetCharControlTemperature(BUFFER *In, unsigned char offset, unsigned char *pData);
unsigned char GetCharArray(BUFFER *In, unsigned char offset, unsigned char *pData);


//unsigned short GetWord(BUFFER *buffer, unsigned char Offset, unsigned char Digit);
//unsigned char GetChar(BUFFER *buffer, unsigned char Offset, unsigned char Digit);

unsigned long Div10(unsigned long A, unsigned char *Rem);
void SetStr(unsigned long Value, unsigned char Count, unsigned char *pStr);
void SetStrWithPointPosition(unsigned long Value, unsigned char PtPos, unsigned char Count, unsigned char *pStr);

//  ********************  TRANSFORMATION.H  ********************

