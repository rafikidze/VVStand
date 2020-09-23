//   **************************   Inner.cpp      **************************
#include <Common.h>
#include "Control.h"
#include "Communication.h"
#include "DoMeasure.h"

#include "Inner.h"

#include "Transformation.h"
#include "TransformDouble.h"
#include "Translation.h"

#include "Other.h"


unsigned char messageLength = 0; //����� ����� ��������� ������ ������
unsigned char contrSum = 0x00; //����������� �����
unsigned char error = 0;
unsigned short vp1_par, vp3_par, vp4_par, tok_par,nbp_par;
//////////////////////////////////////////////
unsigned short U_VVG_circle = 0, U_VVG_circle_prev = 0;

unsigned char ones(unsigned char x)
{
  unsigned char result = 0;
  x = x & 0x0F;
  while(x > 0)
  {
    result += x % 2;
    x /= 2;
  }
  return result;
}

unsigned char ControlSumMod2(BUFFER *buffer, unsigned char Tail, unsigned char *pCount)
{
  unsigned char Count = 0x00, Save, Limit;
  unsigned char Data = 0x00, Code = 0x00;

  Save = GetTail(buffer);
  *pCount = 1;
  Limit = BufferLength(buffer);
  SetTail(buffer, Tail + 1);
  
  for (Count = 1; Count < Limit; Count++)
  {
    Code = PopBuffer(buffer);
    (*pCount)++;
    Data = Data ^ Code;
    if (Code == END_MESS)
    {
        *pCount |= END_MESS;
        break;
    }
  }
  SetTail(buffer, Save);
  return (Data);
}


unsigned char IsInnerEnd(BUFFER *buffer)
{
  if (buffer->Status & BUFFER_FULL)
    return 1;
  else
    return 0;
}

void DoInnerMessage( unsigned char Code, BUFFER *buffer)
{
  unsigned char Count;   //, Cycle, Index
  unsigned char *pData;
  int i = 0;

  IsBufferOver(buffer);
  ClearBuffer(buffer);
  unsigned char Tail = buffer->Head;
  PushBuffer(buffer, 0x02); // ��
  switch (Code)
  {
    // �� ��� ��� - ��� ������ ��������� (�� ������ ������)
  case YES:
    PushBuffer(buffer, YES);
    break;
  case NO:
    PushBuffer(buffer, NO);
    break;
  // �������    
  case 0x61: // ������ ��������� �����������
    PushBuffer(buffer, 0x61);
    break;   
  case 0x62: // ������ ���������� ���
    PushBuffer(buffer, 0x62);
    break;
  case 0x63: // ������ ���������� ���
    PushBuffer(buffer, 0x63);
    break;
  case 0x64: // ������ ��������, ��������
    PushBuffer(buffer, 0x64);
    break;
  // �������
   case 0x50: // P - ������� �� ������/����� ������� 
    PushBuffer(buffer, 0x50);
    PushBuffer(buffer, (Send_TIME | 0x30));
    break;
   case 0x51: // Q - ������� �� ���������/���������� ���������� 
    PushBuffer(buffer, 0x51);
    PushBuffer(buffer, (Send_VENT | 0x30));
    break;
   case 0x52: // R - ������� �� ���������/���������� 380 � 
    PushBuffer(buffer, 0x52);
    PushBuffer(buffer, (Send_380 | 0x30));
    break;
   case 0x53: // S - ������� �� ���������/���������� ������
    PushBuffer(buffer, 0x53);
    PushBuffer(buffer, (Send_SIRENA | 0x30));
    break;
   case 0x54: // T - ������� �� ���������/��������� ���
    PushBuffer(buffer, 0x54);
    PushBuffer(buffer, (Send_VVG_OTP | 0x30));
    break;
   case 0x55: // U - ������� �� ��������� ���������� ���
    PushBuffer(buffer, 0x55);
    PushBuffer(buffer, ( (U_VVG_circle / 100) | 0x30) );          // �����
    PushBuffer(buffer, ( ( (U_VVG_circle % 100) / 10 ) | 0x30) ); // �������
    PushBuffer(buffer, ( (U_VVG_circle % 10) | 0x30) );           // �������
    break;
   case 0x56: // V - ������� ������������
    PushBuffer(buffer, 0x56);
    PushBuffer(buffer, (Send_DEBLOCK | 0x30)); 
    break;    
  default:
    break;
  }
  PushBuffer(buffer, END_MESSAGE);  // ����� ���������
  unsigned char *pCount, controlSum = 0x00;
  controlSum = ControlSumMod2(buffer, Tail, pCount);
  PushBuffer(buffer, controlSum); // ���
skip_make_message:
  ;
}

void IsCommandInner(BUFFER *buffer)
{
  unsigned char Tail, Data;
  Tail =  buffer->Tail;
  buffer->Status |= NULL_CORRECT;
  buffer->Title = GetBufferOffset(buffer, 1);//((GetBufferOffset(buffer, 1) & 0x0F) << 4) + (GetBufferOffset(buffer, 2) & 0x0F);
  Data = GetBufferOffset(buffer, 3);
 if (Data == END_MESSAGE)
  {
  // 2-� ������� ���������
    switch(buffer->Title)
    {
      buffer->Status |= QUEST;
   /* case YES: // ��������� ��
      buffer->Lenght = 4;
    case NO: // ��������� ���
      buffer->Lenght = 4;
      break;*/  
    default:
      buffer->Status &= ~NULL_CORRECT;
      buffer->Title = 0;
      break;
    }
  }
  switch(buffer->Title)
  {
    case YES:
      buffer->Title = YES;
      buffer->Lenght = 4;
    break;
    case NO:
      buffer->Title = NO;
      buffer->Lenght = 4;
    break;
  //  ������ ���������

    case 0x71: // ������ ��������� �����������
      buffer->Lenght = 11;
      break;
    case 0x72: // ������ ���������� ���
      buffer->Lenght = 33;
      break;
    case 0x73: // ������ ���������� ���
      buffer->Lenght = 28;
      break;
    case 0x74: // ������ ��������, ��������
      buffer->Lenght = 48;
      break;
    default:
      Tail++;
      buffer->Status &= ~NULL_CORRECT;
      buffer->Title = 0x00;
       break;
    }
  

  buffer->Tail =  Tail;
}


unsigned char ReceiveInner(BUFFER *buffer)
{
    unsigned char Data = 0x00; //, Error = 0x00, Index = 0x00
 
        if (((buffer->Head - buffer->Fix) == 1) && (GetBuffer(buffer) == END_MESSAGE))
        {
            // ������� �� �������� - ������� ������ ������� ����� ��������� - ����������
            buffer->Tail++;
            goto end_loop;
        }
    
        //  ��������� �� ����� �������� �� ������ �����, ����� ������� ����� ���������
        if ((buffer->Head - buffer->Fix) < 2) goto skip_end_loop;
    
        //��������, ���� �� ��
        if(GetBuffer(buffer) == 0x02);
        else
        {
          buffer->Tail++;
          goto end_loop;
        }
        
        //�������� ������������ ���� ���������
        if (buffer->Status & NULL_CORRECT);
        else IsCommandInner(buffer);
    
        // ����� c�������� �� ��
        if (buffer->Status & NULL_CORRECT);
        else
        {
          buffer->Tail++;
          goto end_loop;// ���� �� ������� - ���� ������� ����� ������
        }
    

      //������ ������ ���������
      messageLength = BufferLength(buffer);
      if(messageLength < buffer->Lenght)
        goto skip_end_loop;
      
      //��������, ���� �� ��
      if(GetBufferOffset(buffer, (buffer->Lenght - 2)) != 0x03)
      {
        buffer->Tail++;
        goto end_loop;
      }
      
      //�������� ����������� �����
      unsigned char *pCount;
      contrSum = ControlSumMod2(buffer, buffer->Tail, pCount);
      if(contrSum != GetBufferOffset(buffer, buffer->Lenght - 1))
      {
        //DoInnerMessage(0x15, &Uart_ADU.Out);
        buffer->Tail += buffer->Lenght;
        goto end_loop;
      }
      else
      {
       
        //������/ ��-���
       switch (buffer->Title)
        {
        case YES: // ����� ��
          if(Command) flag_answer = 1;
          //else RequestCount = 0;
          //DoInnerMessage(YES, &Uart_ADU.Out);
          break;
        case NO: //����� ���
         if(Command) flag_answer = 0;
         else 
         {
           RequestCount ++; 
           flag_timeout_request = 1; // ���� �����-�� ����� ������
     
         }
        // DoInnerMessage(NO, &Uart_ADU.Out);
          break;
        case 0x71:
        
            
            error = GetCharControlPower(buffer, 2, &COMPONENT.ubs);
            error += GetCharControlPower(buffer, 4, &COMPONENT.vvg);
            error += GetCharControlPower(buffer, 5, &COMPONENT.datch_h);
            error += GetCharControlPower(buffer, 6, &COMPONENT.datch_l);
            error += GetCharControlPower(buffer, 7, &COMPONENT.skin);
            error += GetCharControlPower(buffer, 8, &COMPONENT.tbk);

            flag_timeout_request = 1; // ���� �����-�� ����� ������
            if (error) 
            {
              RequestCount ++;
              
            }
            else SetDataFromRequestCOMPONENT();
        
           break;
         case 0x72:
            error = GetCharControlPower(buffer, 4, &UBS.fire);
            error += GetCharControlPower(buffer, 7, &UBS.vent_on);
            error += GetCharControlPower(buffer, 9, &UBS.c_byte_h);
            error += GetCharControlPower(buffer, 10, &UBS.c_byte_l);
            error += GetCharControlPower(buffer, 12, &UBS.seth);
            error += GetCharControlPower(buffer, 13, &UBS.set1_380);
            error += GetCharControlPower(buffer, 15, &UBS.rmb);
            error += GetCharControlPower(buffer, 22, &UBS.door_zoi);
            error += GetCharControlPower(buffer, 25, &UBS.door_tbk);

            flag_timeout_request = 1; // ���� �����-�� ����� ������
            if (error) {
              RequestCount ++;
              
              
            }
            else SetDataFromRequestUBS();
        
           break;
          case 0x73:
            error = GetCharControlPower(buffer, 2, &VVG.fail_h);
            error += GetCharControlPower(buffer, 3, &VVG.fail_l);
            
            // ������� - ��������� �� ���� ������� 09.06.20
            error += GetCharControlPower(buffer, 4, &VVG.U1[0]);
            error += GetCharControlPower(buffer, 5, &VVG.U1[1]);
            error += GetCharControlPower(buffer, 6, &VVG.U1[2]);
            error += GetCharControlPower(buffer, 7, &VVG.U1[3]);
            
            error += GetCharControlPower(buffer, 12, &VVG.U2[0]);
            error += GetCharControlPower(buffer, 13, &VVG.U2[1]);
            error += GetCharControlPower(buffer, 14, &VVG.U2[2]);
            error += GetCharControlPower(buffer, 15, &VVG.U2[3]);
            
            error += GetCharControlPower(buffer, 8, &VVG.I1[0]);
            error += GetCharControlPower(buffer, 9, &VVG.I1[1]);
            error += GetCharControlPower(buffer, 10, &VVG.I1[2]);
            error += GetCharControlPower(buffer, 11, &VVG.I1[3]);
            
            error += GetCharControlPower(buffer, 16, &VVG.I2[0]);
            error += GetCharControlPower(buffer, 17, &VVG.I2[1]);
            error += GetCharControlPower(buffer, 18, &VVG.I2[2]);
            error += GetCharControlPower(buffer, 19, &VVG.I2[3]);
            
            error += GetCharControlPower(buffer, 25, &VVG.state);

            flag_timeout_request = 1; // ���� �����-�� ����� ������
            if (error) {
              RequestCount ++;
              
              
            }
            else SetDataFromRequestVVG();
        
           break;
        case 0x74:
            error =  GetCharControlTemperature(buffer, 2, &DATCHIK.t_zoi[0]);
            error += GetCharControlTemperature(buffer, 3, &DATCHIK.t_zoi[1]);
            error += GetCharControlTemperature(buffer, 4, &DATCHIK.t_zoi[2]);
            error += GetCharControlTemperature(buffer, 5, &DATCHIK.t_zoi[3]);
            
            error += GetCharControlPower(buffer, 6, &DATCHIK.vl_zoi[0]);
            error += GetCharControlPower(buffer, 7, &DATCHIK.vl_zoi[1]);
            error += GetCharControlPower(buffer, 8, &DATCHIK.vl_zoi[2]);
            error += GetCharControlPower(buffer, 9, &DATCHIK.vl_zoi[3]);
            
            error += GetCharControlTemperature(buffer, 10, &DATCHIK.t_tbk[0]);
            error += GetCharControlTemperature(buffer, 11, &DATCHIK.t_tbk[1]);
            error += GetCharControlTemperature(buffer, 12, &DATCHIK.t_tbk[2]);
            error += GetCharControlTemperature(buffer, 13, &DATCHIK.t_tbk[3]);
            
            error += GetCharControlPower(buffer, 14, &DATCHIK.p_tbk[0]);
            error += GetCharControlPower(buffer, 15, &DATCHIK.p_tbk[1]);
            error += GetCharControlPower(buffer, 16, &DATCHIK.p_tbk[2]);
            error += GetCharControlPower(buffer, 17, &DATCHIK.p_tbk[3]);
            
            error += GetCharControlPower(buffer, 18, &DATCHIK.time[0]);
            error += GetCharControlPower(buffer, 19, &DATCHIK.time[1]);
            error += GetCharControlPower(buffer, 20, &DATCHIK.time[2]);
            error += GetCharControlPower(buffer, 21, &DATCHIK.time[3]);
            
            flag_timeout_request = 1; // ���� �����-�� ����� ������
            if (error) 
            {
              RequestCount ++;
              
            }
            else SetDataFromRequestDATCHIK();
            
            
            
          break;
        default:
          break;
        }
       }
        
    
    
    RelocateBufferEnd(buffer);

end_loop:

    buffer->Title = 0x00;
    Data = 1;
    buffer->Fix = buffer->Tail;
skip_end_loop:
           
    return Data;
}

void GetPatameters(void)
{
   short res; 
   vp1_par = GetAve(&MeasureAdd[VP1])*2; // ��� �������
   
   U_VVG_circle = GetAve(&MeasureAdd[VP1]) * 2; 
   if (U_VVG_circle > 500) U_VVG_circle = 500;
   
   // ������� ������
   res = U_VVG_circle_prev - U_VVG_circle;
   if (res < 0) res = -res;
   
   if(res >= 2) // ������ �������, ������ �������
   {
     Command = 0x55;
     CommandStep = 0x00;
     
     U_VVG_circle_prev = U_VVG_circle;
   }
   
}

//   ********************************************************************************

