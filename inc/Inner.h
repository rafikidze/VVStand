//   **************************   Inner.h     **************************
#define NBP 0 //���������� �� ������ ���������
#define TBP 1 //��� �� ������ ���������
#define I_MAX 0x32 //������������ ��� 5�

#define VP1 0 // ��������� �������� ������� +5�
#define VP2 1 // ��������� �������� ������� +12�
#define VP3 2 // ��������� �������� ������� -12�
#define VP4 3 // ��������� �������� ������� +15�

extern unsigned short  vp1_par, vp3_par, vp4_par, tok_par,nbp_par;
/////////////////////////////////////////////////////
extern unsigned short U_VVG_circle_prev, U_VVG_circle;



//���� ������
#define NO_ERROR 0x00 //�� ��������
#define END_MESS 0x03 //����� ������


unsigned char ones(unsigned char x);
unsigned char ControlSumMod2(BUFFER *buffer, unsigned char Tail, unsigned char *pCount);
unsigned char IsInnerEnd(BUFFER *);
//unsigned char IsOneByteInner(unsigned char );
void DoInnerMessage( unsigned char , BUFFER *);
//void ErrorMessageInner(unsigned char Code, BUFFER *buffer); // � ��
void IsCommandInner(BUFFER *);
unsigned char ReceiveInner(BUFFER *);

void GetPatameters(void);

//   ********************************************************************************


