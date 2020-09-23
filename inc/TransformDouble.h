
//  ********************  TRANSFORMDOUBLE.H  ********************

#define TRANSF_ERR_TWOPOINT           0x01  //  введены две точки
#define TRANSF_ERR_MANYDIGIT          0x02  //  нет точки
#define TRANSF_ERR_NOTDIGIT           0x04  //  не цифра - этого не должно быть

#define ADD_ROUND_ERROR               1.8e-08  //5.0E-12


unsigned short StrToDbl( unsigned char *str, double *Data, unsigned char Pos, unsigned char PPos);
unsigned short DblToStr( unsigned char *str, double Data, unsigned char Pos, unsigned char PPos);

unsigned char StrToUChar( unsigned char *str, unsigned char *Data, unsigned char Pos);
unsigned char UCharToStr( unsigned char *str, unsigned char Data, unsigned char Pos);

unsigned char StrToUShort( unsigned char *str, unsigned short *Data, unsigned char Pos);
unsigned char UShortToStr( unsigned char *str, unsigned short Data, unsigned char Pos);

void DoWhiteSpace( unsigned char *str, unsigned char Pos);
//  ********************  TRANSFORMDOUBLE.H  ********************

