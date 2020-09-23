
//   **************************   BUFFER.h     **************************

//  STATUS
#define BUFFER_OVER   0x80                                                          //  ����� ����������
#define BUFFER_FULL   0x10                                                          //  ������ ������ �����
#define NULL_CORRECT  0x08
#define QUEST         0x04
#define BUFFER_LENGHT 0x02                                                          //  ����� ��������� ���������� (��� ������������� ���� BUFFER_PRO)
#define BUFFER_DO     0x01                                                          //  ����� - �������� ���������

//  CODE
#define BUFFER_IN     0x80                                                          //  ������� �����
//#define BUFFER_OUT    0x40
#define BUFFER_BYTE   0x40    //  � ��������� ����� �������                                                      //  �������� �����
#define BUFFER_HF     0x20                                                          //  ������ � ������ ���������� ������ - ��������
#define BUFFER_TIME   0x10      //  �������� ��������� �����������                                                     //  ������ � ������ ���������� ������ - ��������
//  ��������� ��������� � ������
#define BUFFER_END    0x08                                                          //  ������� ����� � ������ ������ ��������� - � ����������� �� ������� �����
#define BUFFER_X      0x04                                                          //  ������� ����� � ������ ������ ��������� - � ����������� �� ������� �����
#define BUFFER_PRO    0x02                                                          //  ������ � ����������� ������������ �����
#define BUFFER_BEGIN  0x01                                                          //  ��������� � ��������� ������ ��������� - ����� 2(?) ����� - ������� �������� E

#define MAX_SIZE 0x40                                                               //  ����� ������
#define LIMIT_SIZE 0x3F

#define END_MESSAGE             0x03                                                //  ������� ����� ���������

// ���� ���������
#define ABORT_MESSAGE           0x83
#define BEGIN_TEXT              0x02
#define DONE_MESSAGE            0x09
#define DO_MESSAGE              0x10
#define HOME_MESSAGE            0x48

#define BEGIN_MESSAGE           0xE5
#define ERROR_MESSAGE           0xE4

//  ������ ������
#define ERR_BEGIN               BIT_0
#define ERR_WAIT                BIT_1
#define ERR_TITLE               BIT_2
#define ERR_LENGHT              BIT_3

#define ERR_CODE                BIT_4

#define ERR_SUM                 BIT_5
#define ERR_TIME                BIT_6
#define ERR_DO                  BIT_7

typedef struct _buffer
{
unsigned char  Code,
               Status,
               Title,
               Lenght,
               Error,
               Head, Tail, Fix;
unsigned char  Data[MAX_SIZE];
} BUFFER;

unsigned char BufferLength(BUFFER *buffer);
void ClearBuffer(BUFFER *buffer);
unsigned char ClearBufferFix(BUFFER *buffer);                                                              // ������� Status ������, ������������� ���������
void CopyBuffer(BUFFER *In, BUFFER *Out);
void CopyBufferFix(BUFFER *In, BUFFER *Out, unsigned char Start, unsigned char Count);
unsigned char GetBufferSpace(BUFFER *buffer, unsigned char Space);
void EraseBuffer(BUFFER *buffer);
void InitBuffer(BUFFER *buffer, unsigned char Code);
unsigned char IsBufferNotEmpty(BUFFER *buffer);
void IsBufferOver(BUFFER *buffer);
void RelocateBufferEnd(BUFFER *buffer);

__monitor unsigned char GetBuffer(BUFFER *buffer);
__monitor unsigned char GetBufferOffset(BUFFER *buffer, unsigned char offset);
__monitor unsigned char PopBuffer(BUFFER *buffer);
__monitor unsigned char FixBuffer(BUFFER *buffer);
__monitor unsigned char GetTail(BUFFER *buffer);
__monitor void SetTail(BUFFER *buffer, unsigned char Val);
__monitor void PushBuffer(BUFFER *buffer, unsigned char Data);
__monitor void SetBuffer(BUFFER *buffer, unsigned char Data);


//   ********************************************************************************

