//  ********************  CONTROL.CPP  ********************
#include <Common.h>
#include "ADC_base.h"
#include "ADC_add.h"
#include "Control.h"
#include "TimerBase.h"
#include "TimerEvent.h"
#include "Communication.h"
#include "DoMeasure.h"
#include "Inner.h"
#include <Dangerous.h>
#include "Key.h"

unsigned char  Command = 0x00, CommandStep = 0x00;
unsigned char  SetInit;

////////////////////////////////////////////////////////////
CKEY KeyPortH, KeyPortK;
CUBS UBS;
CCOMPONENT COMPONENT;
CVVG VVG;
CDATCHIK DATCHIK;
// TIME
unsigned short TimeDelay = TIME_REQUEST;
unsigned short TimeCommand = TIME_COMMAND;
unsigned short TimeTranzistor = TIME_TRANZISTOR;
unsigned short TimeLeds = TIME_CHECK_LEDS;
unsigned short TimeCommon = TIME_REQUEST;

////////
unsigned char StepLed = 0, CurrentV = 0;
unsigned char RequestStep = 0x00;
unsigned char CommandCount = 0x00;   // для 3-го повторения команд 
unsigned char RequestCount = 0x00;   // для 3-го повторения запроса 
unsigned char Deblock = 0;
unsigned char Send_VENT = 0;         //  |
unsigned char Send_TIME = 0;         //  |
unsigned char Send_380 = 0;          //  |
unsigned char Send_SIRENA = 0;       //  | 
unsigned char Send_VVG_OTP = 0;      //  |
unsigned char Send_DEBLOCK = 0;      //  |
int Send_VVG = 0;                    //  - >  значения, которые отправляем на СКиН
unsigned char flag_380    = 0;
unsigned char flag_answer = 0;       // 1 - YES, 0 - NO
unsigned char flag_timeout_request = 0; // хоть какой-то ответ пришел на запрос

unsigned char fail_skin = 0;         // собирательный флаг аварии СКиН:
                                     //   0x01 - авария пришла в ответе на запрос - 71
                                     //   0x02 - авария по линии связи - запросы
                                     //   0x04 - авария по линии связи - команды

unsigned char fail_ozon = 0;         // собирательный флаг аварии ОЗОН:
                                     //   0x01 - авария озон ЗОИ
                                     //   0x02 - авария озон ТБК
                                     //   0x04 - авария датчиков из запроса - 71
 
unsigned char fail_vvg = 0;          // собирательный флаг аварии ВВГ:
                                     //   0x01 - неисправность ВВГ из запроса 73
                                     //   0x02 - авария ВВГ из запроса 71
                                     //   0x04 - нет связи ВВГ из запроса 71

unsigned char fail_tbk = 0;           // собирательный флаг аварии ТБК:
                                     //   0x01 - авария ТБК
                                     //   0x02 - нет связи ТБК
                                     //   0x04 - температура ТБК
                                     //   0x08 - давление ТБК
                                    
unsigned char zero_mass[4] = {0, 0, 0, 0}; // для обнуления семисегментников

                                          //   ----------------------
                                          //  |  D4    D5    D6    D7 |
unsigned char registersD[COUNT_REGISTER_LED] = {0xFF, 0xFF, 0xFF, 0xFF}; // по умолчанию все светодиоды погашены
unsigned char registersD_LE[COUNT_REGISTER_LED] = {0x02, 0x04, 0x08, 0x10};

unsigned char sevenSegmentD8[DIGS] = {ZERO|DP, ZERO|DP, ZERO|DP, ZERO|DP, 
                                                       ZERO|DP, ZERO|DP, ZERO|DP, ZERO|DP, 
                                                       ZERO|DP, ZERO|DP, ZERO|DP, ZERO|DP};
unsigned char sevenSegmentD9[DIGS-4] = {  ZERO|DP, ZERO|DP, ZERO|DP, ZERO|DP,
                                          ZERO|DP, ZERO|DP, ZERO|DP, ZERO|DP};
unsigned char tranzistorsPortQA[DIGS] = {0x01, 0x02, 0x04, 0x08, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };


unsigned char count_debug = 0;



void Control(void)
{
   DoTimer();
   DoCommand();
   if (!SetInit) 
   {
     DoRequest(); // прошла инициализация, можно делать запросы
     DoMeasure(); // и рассчеты 
   }
   DoCommunication();
   checkDiods();
   checkSevenDiods();
  

}


void DoTimer(void)
{
  unsigned char Flag = 1;
  unsigned char Count = 0x00, Data; //Cycle = 0x00,
  unsigned char _t;
  unsigned short Temp, Time;
  
  GetTimerBase();
  if (DoTimeBase)
  {
      while ( DoTimeBase >= Flag ) //
      {

        if (Flag);
        else
        {
          _t = __save_interrupt();
          __disable_interrupt();
          Flag = (1 << Count);
          continue;
          __restore_interrupt(_t);
        }

        switch (DoTimeBase & Flag)
        {
#ifdef TIMER_TAKT
        case TIMER_TAKT:    //  контроль временных промежутков с точностью 1 мс

          PORTK.OUT ^= 0x40;
       
         if (TimeLeds) TimeLeds--; 
         if (TimeTranzistor) TimeTranzistor --;
         if (TimeDelay) TimeDelay--;
         if (TimeCommand) TimeCommand--;
         if (TimeCommon) TimeCommon--;
         
          ReStartTimerBase(TIME_TAKT);
          break;
#endif
#ifdef TIMER_GET
        case TIMER_GET:    //  контроль временных промежутков с точностью 0.5 мс

#endif
#ifdef TIMER_KEY  // ****************** работа с клавиатурой *****************//
      case TIMER_KEY:
        //  опрос клавиатуры
 
          if (KeyPortK.ScanPortK())
        {
          unsigned char key = KeyPortK.GetCode();
          switch(key)
          {
            case B_PUSK: 
              Send_TIME = 1;
              Command = 0x50;
              CommandStep = 0x00; 
            break;
            case B_SBROS:
              Send_TIME = 2;
              Command = 0x50;
              CommandStep = 0x00;
            break;
            case B_SOUND: 
            
              Send_DEBLOCK = 1;
              Command = 0x56;
              CommandStep = 0x00;
            break;
            case B_FAIL: 
              Deblock = 1;
              Send_DEBLOCK = 2;
              Command = 0x56;
              CommandStep = 0x00;
            break;
          } 
        }
          if (KeyPortH.ScanPortH())
        {
          //KeyTest.GetCode() - код кнопки
          unsigned char key = KeyPortH.GetCode();
          switch(key)
          {
            case B_VENT_ON : 
              Send_VENT = 1;
              Command = 0x51;
              CommandStep = 0x00;
            break; 
            case B_VENT_OFF : 
              Send_VENT = 0;
              Command = 0x51;
              CommandStep = 0x00;
            break;
            case B_380_ON: 
              Send_380 = 1;
              Command = 0x52;
              CommandStep = 0x00;
            break;
            case B_380_OFF: 
              Send_380 = 0;
              Command = 0x52;
              CommandStep = 0x00;
            break;
            case B_SIRENA: // нажатие
              Send_SIRENA = 1;
              Command = 0x53;
              CommandStep = 0x00;          
            break;
            case B_VVG_OTP: 
              Send_VVG_OTP = 1;
              Command = 0x54;
              CommandStep = 0x00;
            break;
            case B_VVG_ZAP: 
             Send_VVG_OTP = 0;
             Command = 0x54;
             CommandStep = 0x00;
            break;
          }
        }
        
        if(KeyPortH.KeyRelease())
        {
          Send_SIRENA = 0;
          TimeCommand = 2000;
          Command = 0x53;
          CommandStep = 0x00; 
        }
        
        ReStartTimerBase(TIME_KEY);
        break;
#endif
#ifdef TIMER_ADC
        case TIMER_ADC:    //  контроль временных промежутков с точностью 0.5 мс
          //  обновление экрана
          GetPatameters();
          ReStartTimerBase(TIME_ADC);
          break;
#endif
/*#ifdef TIMER_DAC
        case TIMER_DAC:    //  контроль временных промежутков с точностью 0.5 мс
          ReStartTimerBase(TIME_DAC);
          break;
#endif

#ifdef TIMER_LCD  // ****************** работа с дисплеем ***************** //
        case TIMER_LCD:     // *************   ???   ***************
          ReStartTimerBase(TIME_LCD);
          break;
#endif
#ifdef TIMER_CANCEL_CHANGE
        case TIMER_CANCEL_CHANGE:    //  восстанавливаем экран
          break;
#endif
#ifdef TIMER_CONTROL
        case TIMER_CONTROL:    //  опрос отказов
          ReStartTimerBase(TIME_CONTROL);
          break;
#endif
        default:
          break;
        }
*/
        

      }
      DoTimeBase &= ~Flag;
        Flag = Flag << 1;
        Count++;
        if (DoTimeBase);
        else break;
  } 
  }
}

void DoCommand(void)  {
//  *********** команды управления - изменение состояния РПДУ ************
  //unsigned char Data = 0x00, Index;
  //unsigned char Temp;

  //  проверка команд управления от АДУ по линиям провод-команда

  //  контроль исполнения команд

    switch (Command)
    {
    case 0x00:
      if (SetInit)
      {
        Command = 0x24;
        CommandStep = 0x00;
      }
      else
      {
      }
      break;
    case 0x24:  //  инициализация
       switch (CommandStep)
      {
      case 0x00:
//    интерфейс АДУ      
           Uart_ADU.Init(   &USARTC0,
                         &PORTC, PORT3,                           // 232 Tx
                         0, 0,                                    // 232      //  En
                         19200,                                      //  Baud в соотвествии с конфигурацией
                         0x00,                                       //  BaudSel
                         (ASYNC | PARITY_EVEN | STOP_1 | INFO_8BIT), //  Frame 00 10 1 011 (асинхронный, контроль четности, 2 стоп бита, 8 инф бит)
                         (USART_RXCINTLVL | USART_TXCINTLVL),        //  LevelInterrupt
                         (USART_RXEN | USART_TXEN));                 //  Control

        //  при инициализации - установить Status и Code вх/вых буферов
        InitBuffer(&Uart_ADU.In, BUFFER_IN);
        InitBuffer(&Uart_ADU.Out, BUFFER_HF);
        Uart_ADU.Clear();
        Uart_ADU.EnableRxC();  
        
        CommandStep++;

        break;
      case 0x01:  
        //  
      Uart_TECHNO.Init(   &USARTC1,
                         &PORTC, PORT7,                           // 232 Tx
                         0, 0,                                    // 232      //  En
                         19200,                                      //  Baud в соотвествии с конфигурацией
                         0x00,                                       //  BaudSel
                         (ASYNC | PARITY_EVEN | STOP_1 | INFO_8BIT), //  Frame 00 10 1 011 (асинхронный, контроль четности, 2 стоп бита, 8 инф бит)
                         (USART_RXCINTLVL | USART_TXCINTLVL),        //  LevelInterrupt
                         (USART_RXEN | USART_TXEN));                 //  Control

        //  при инициализации - установить Status и Code вх/вых буферов
        InitBuffer(&Uart_TECHNO.In, BUFFER_IN);
        InitBuffer(&Uart_TECHNO.Out, BUFFER_HF);
        Uart_TECHNO.Clear();
        Uart_TECHNO.EnableRxC();
        
        
        
        CommandStep++;
        break;
      case 0x02:
        EVSYS.CH7MUX = 0xD0;  //TimerD0 OVF
        
        ADC_add.Init(  &ADCB,       // ADC_t *pField
                       CHN_0_START, // unsigned char Control
                       (EVENT_SWEEP_0 | EVENT_LINE_7 | EVENT_SYNC_SWEEP), //unsigned char Event
                       0x00,        // unsigned char Start
                       0x00,        // unsigned char StopChannel
                       0x00,        // unsigned char IntMode
                       0x02,        // unsigned char IntLevel
                       ADCBCAL0,    // unsigned char Cal0
                       ADCBCAL1,    // unsigned char Cal1
                       ADC_CONVMODE | ADC_RESOLUTION_12R, //unsigned char Config
                       REF_AREFB,   //unsigned char Reference
                       ADC_PreScaler); //unsigned char PreScaler
       
        ADC_add.ChannelConfig( 0, ADC_SINGLEENDED, 0x00, ADC_CH_MUX_1, 0x00);
        SetCalibrate(&MeasureAdd[0].Calibrate, 5900, 14, -10);
       

        CommandStep++;
        break;
      case 0x03:
        //  старт измерений
        ADC_add.Start();
        StartTimerEvent(TIME_EVENT);
        StartTimerBase(TIME_ADC);

        CommandStep++;
        break;
      case 0x04:
        
        KeyPortH.init();
        KeyPortK.init();
        TimeCommand = 2500;
        CommandStep++;
        break;
       case 0x05:
        if(TimeCommand) break;
        else
        {
        registersD[0] = 0x00; registersD[1] = 0x00; 
        registersD[2] = 0x00; registersD[3] = 0x00;
        SetInit = 0;
        Command = 0x00;
        CommandStep = 0x00;
        TimeDelay = 2500;
        
        }  
        break;  
        
      }
     break;
     // ------------------------------------------------
     case 0x50:  // P - команда на запуск/сброс таймера 
      switch(CommandStep)
      {
      case 0x00:
        flag_answer = 0;
        DoInnerMessage(0x50, &Uart_ADU.Out);
        TimeCommand = TIME_COMMAND;
        CommandStep++;
        break;
      case 0x01:
        if(TimeCommand) break;
        else // сделать проверку, что время вышло, а ответа нет
        {
          if(flag_answer) // все хорошо
          {
            CommandStep = CommandCount = Command =  0x00;
            fail_skin &= ~0x02;
          }
          else 
          {
            if(CommandCount > 3)
              // не удается установить -> авария СКиН
              fail_skin |= 0x02;
            else {
              CommandCount++;
              CommandStep = 0x00;
            }
          }
        }
        break;
      }
     break;
    // -----------------------------------------------
    case 0x51:  // Q - включение/выключение вентиляции
      
      switch(CommandStep)
      {
      case 0x00:
        flag_answer = 0;
        DoInnerMessage(0x51, &Uart_ADU.Out);
        TimeCommand = TIME_COMMAND;
        CommandStep++;
        break;
      case 0x01:
        if(TimeCommand) break;
        else // сделать проверку, что время вышло, а ответа нет
        {
          if(flag_answer) // все хорошо
          {
            CommandStep = CommandCount = Command =  0x00;
            fail_skin &= ~0x02;
          }
          else 
          {
            if(CommandCount > 3)
              // не удается установить -> авария СКиН
              fail_skin |= 0x02;
            else {
              CommandCount++;
              CommandStep = 0x00;
            }
          }
        }
        break;
      }
     break;
    // -----------------------------------------------
    case 0x52:  // R - команда на включение/отключение 380 В 
      switch(CommandStep)
      {
      case 0x00:
        DoInnerMessage(0x52, &Uart_ADU.Out);
        TimeCommand = TIME_COMMAND;
        CommandStep++;
        break;
      case 0x01:
        if(TimeCommand) break;
        else // сделать проверку, что время вышло, а ответа нет
        {
          if(flag_answer) // все хорошо
          {
            CommandStep = CommandCount = Command =  0x00;
            fail_skin &= ~0x02;
          }
          else 
          {
            if(CommandCount > 3)
              // не удается установить -> авария СКиН
              fail_skin |= 0x02;
            else {
              CommandCount++;
              CommandStep = 0x00;
            }
          }
        }
        break;
      }
     break;
    // -----------------------------------------------
    case 0x53:  // S - команда на включение/отключение сирены
      switch(CommandStep)
      {
      case 0x00:
        if(TimeCommand) break;
        else
        {
          DoInnerMessage(0x53, &Uart_ADU.Out);
          TimeCommand = TIME_COMMAND;
          CommandStep++;
        }
        break;
      case 0x01:
        if(TimeCommand) break;
        else // сделать проверку, что время вышло, а ответа нет
        {
          if(flag_answer) // все хорошо
          {
            CommandStep = CommandCount = Command =  0x00;
            fail_skin &= ~0x02;
          }
          else 
          {
            if(CommandCount > 3)
              // не удается установить -> авария СКиН
              fail_skin |= 0x02;
            else {
              CommandCount++;
              CommandStep = 0x00;
            }
          }
        }
        break;
      }
     break; 
     // -----------------------------------------------
    case 0x54:  // T - команда на запирание/отпирание ВВГ
      switch(CommandStep)
      {
      case 0x00:
        DoInnerMessage(0x54, &Uart_ADU.Out);
        TimeCommand = TIME_COMMAND;
        CommandStep++;
        break;
      case 0x01:
        if(TimeCommand) break;
        else // сделать проверку, что время вышло, а ответа нет
        {
          if(flag_answer) // все хорошо
          {
            CommandStep = CommandCount = Command =  0x00;
            fail_skin &= ~0x02;
          }
          else 
          {
            if(CommandCount > 3)
              // не удается установить -> авария СКиН
              fail_skin |= 0x02;
            else {
              CommandCount++;
              CommandStep = 0x00;
            }
          }
        }
        break;
      }
     break; 
     // -----------------------------------------------
    case 0x55: // U - команда на установку напряжения ВВГ     
      switch(CommandStep)
      {
      case 0x00:
        DoInnerMessage(0x55, &Uart_ADU.Out);
        TimeCommand = TIME_COMMAND;
        CommandStep++;
        break;
      case 0x01:
        if(TimeCommand) break;
        else // сделать проверку, что время вышло, а ответа нет
        {
          if(flag_answer) // все хорошо
          {
            CommandStep = CommandCount = Command =  0x00;
            fail_skin &= ~0x02;
          }
          else 
          {
            if(CommandCount > 3)
              // не удается установить -> авария СКиН
              fail_skin |= 0x02;
            else {
              CommandCount++;
              CommandStep = 0x00;
            }
          }
        }
        break;
      }
     break; 
      // -----------------------------------------------
    case 0x56: // V - команда деблокировки
      switch(CommandStep)
      {
      case 0x00:
        DoInnerMessage(0x56, &Uart_ADU.Out);
        TimeCommand = TIME_COMMAND;
        CommandStep++;
        break;
      case 0x01:
        if(TimeCommand) break;
        else // сделать проверку, что время вышло, а ответа нет
        {
          if(flag_answer) // все хорошо
          {
            CommandStep = CommandCount = Command =  0x00;
            fail_skin &= ~0x02;
          }
          else 
          {
            if(CommandCount > 3)
              // не удается установить -> авария СКиН
              fail_skin |= 0x02;
            else {
              CommandCount++;
              CommandStep = 0x00;
            }
          }
        }
        break;
      }
     break;
    
    }
  skip_all:
  ;
}


void ClearCommand(void)
{
  Command = CommandStep = 0x00;
}

unsigned char checkDiods ()
{
   if(TimeLeds) return 0;
   else 
   {
     for (int i = 0; i < COUNT_REGISTER_LED; i++)
      TurnOnLeds(registersD[i], registersD_LE[i]); 
     TimeLeds = TIME_CHECK_LEDS;
     
     // для отладки
    /* if (count_debug >=12)
       count_debug = 0;
     else 
     {
       switch (count_debug)
       {
         case 0:  
         case 1:
         case 2:
         case 3:
         case 4:  
         case 5:
         case 6:
         case 7:
         case 8:  
         case 9:
         case 10:
         case 11:
           sevenSegmentD8[count_debug] = MINUS; break;
       }
         
     }
     count_debug++;*/
     
   }
   return 1;
   
} 
void TurnOnLeds (unsigned char regD, unsigned char LE)
{ 
    PORTJ.OUT = regD;
    PORTD.OUT |= LE; // LE в 1
    PORTD.OUT ^= LE; // LE в 0 
      
}
unsigned char checkSevenDiods ()
{

     if (TimeTranzistor) return 0;
     else 
     {
       if(StepLed >=12)
       {
         PORTA.OUT ^=CurrentV;
         StepLed = CurrentV =  0;
       }
       else if (StepLed >=8)
       {
         PORTA.OUT ^=CurrentV; // снимаем прошлый транзистор
         TurnOnSevenSegments(StepLed, LE_D8);  
         PORTA.OUT |= CurrentV;
       }
       else if (StepLed >4)
       {
         PORTA.OUT ^=CurrentV; // снимаем прошлый транзистор
         TurnOnSevenSegments(StepLed, (LE_D8 | LE_D9));  
         PORTA.OUT |= CurrentV;
       }
       else if (StepLed == 4)
       {
         PORTQ.OUT ^=CurrentV; // снимаем прошлый транзистор
         TurnOnSevenSegments(StepLed, (LE_D8 | LE_D9));  
         PORTA.OUT |= CurrentV;
       }
       else 
       {
         PORTQ.OUT ^=CurrentV; // снимаем прошлый транзистор
         TurnOnSevenSegments(StepLed, (LE_D8 | LE_D9));  
         PORTQ.OUT |= CurrentV;
       }
     }
   return 1;
  
} 

void TurnOnSevenSegments (unsigned char CountStep, unsigned char LE_)
{
  CurrentV = tranzistorsPortQA[CountStep];
  PORTE.OUT = sevenSegmentD8[CountStep];
  PORTF.OUT = sevenSegmentD9[CountStep];
  PORTD.OUT = LE_;
  PORTD.OUT ^= LE_;
       
  StepLed++;
  TimeTranzistor = TIME_TRANZISTOR;
}

void DoRequest(void)
{ 
  if(TimeDelay) return;
 
  switch (RequestStep)
  {
  case 0x00: // запрос состояния компонентов
      flag_timeout_request = 0;
      if (Deblock) Deblock++;
      RequestStep ++; 
      TimeDelay = TIME_REQUEST;
      DoInnerMessage(0x61, &Uart_ADU.Out); 
    break;
  case 0x01: // запрос параметров УБС
      flag_timeout_request = 0;
      if (Deblock) Deblock++;
      RequestStep ++; 
      TimeDelay = TIME_REQUEST;
      DoInnerMessage(0x62, &Uart_ADU.Out);
    break;
  case 0x02: // запрос параметров ВВГ
      flag_timeout_request = 0;
      if (Deblock) Deblock++;
      RequestStep ++; 
      TimeDelay = TIME_REQUEST;
      DoInnerMessage(0x63, &Uart_ADU.Out);
    break;
  case 0x03: // запрос датчиков, таймера
      flag_timeout_request = 0;
      if (Deblock) Deblock++;
      RequestStep = 0x00; 
      TimeDelay = TIME_REQUEST;
      DoInnerMessage(0x64, &Uart_ADU.Out);
    break;
  default: RequestStep = 0x00; break;
  
  }
  
  if (Deblock > 5) Deblock = 0;
  
 
  if(!flag_timeout_request)
  {
    RequestCount++;
  }
  
  if (RequestCount > MAX_ERROR)
  {
    fail_skin |=0x04;
    RequestCount = MAX_ERROR;
  }
  else 
  {
    fail_skin &= ~0x04;
    fail_skin &= ~0x02; // уберем еще аварию комманд, если она стояла, но связь возобновилась
 
  }
  
  
   // проверку на "нет связи" у СКиН делать здесь, тк
   // запросы не прекратятся (в отличие от ответов на них)
   // собирательная авария СКиН
  if(fail_skin) registersD[D5] |= REG_D5;
  else { if (Deblock) registersD[D5] &= ~REG_D5;}
  
  
}
void SetDataFromRequestUBS ()
{
  RequestCount = 0;
  UBS.zoi = UBS.c_byte_h & 0x02;
  UBS.tbk = UBS.c_byte_h & 0x04;
  
  if(UBS.door_zoi & 0x04)  fail_ozon |= 0x01;
  else fail_ozon &= ~0x01;
  
  if(UBS.door_tbk & 0x04)  fail_ozon |= 0x02;
  else fail_ozon &= ~0x02;
  

  // Авария пожар
  if(UBS.fire & 0x01) registersD[D5] |= REG_D0;
  else { if (Deblock) registersD[D5] &= ~REG_D0;}
  
  // вентиляция
  if(UBS.vent_on & 0x01) registersD[D7] |= REG_D4;
  else registersD[D7] &= ~REG_D4;
  
  // F1 ВВП
  if(UBS.c_byte_l & 0x01) registersD[D6] |= REG_D1;
  else registersD[D6] &= ~REG_D1;
  // F2 ВВП
  if(UBS.c_byte_l & 0x02) registersD[D6] |= REG_D2;
  else registersD[D6] &= ~REG_D2;
  // F3 ВВП
  if(UBS.c_byte_l & 0x04) registersD[D6] |= REG_D3;
  else registersD[D6] &= ~REG_D3;
  // GND 
  if(UBS.c_byte_l & 0x08) registersD[D6] |= REG_D0;
  else registersD[D6] &= ~REG_D0;
  // ЗОИ 
  if(UBS.zoi) registersD[D7] |= REG_D0;
  else registersD[D7] &= ~REG_D0;
  // ТБК
  if(UBS.tbk) registersD[D7] |= REG_D1;
  else registersD[D7] &= ~REG_D1;
  // ТС СЕТЬ 1 = 380
  if(UBS.set1_380 & 0x01) registersD[D7] |= REG_D5;
  else registersD[D7] &= ~REG_D5;
  
  if (flag_380 != (UBS.set1_380 & 0x01)) 
  {
    flag_380 = UBS.set1_380 & 0x01;
    if (flag_380) // если включили сеть, ставим таймер
      TimeCommon = 30000;
  }
  
  
  // если хотя бы одной ТС СЕТЬ 1/ТС СЕТЬ 2/ТС СЕТЬ 3/ТС СЕТЬ 4/ТС СЕТЬ 6 нет,
  // то зажигать 
  // АВТОМАТ
  if(((UBS.set1_380 & 0x0E) == 0x0E) && (UBS.seth & 0x02)) registersD[D4] &= ~REG_D3;
  else registersD[D4] |= REG_D3;
  
  // РМБ
  if((UBS.rmb & 0x0C) == 0x04)  registersD[D4] &= ~REG_D0; 
  else registersD[D4] |= REG_D0;
  
  // Дверь ЗОИ
  if(UBS.door_zoi & 0x01)  registersD[D4] |= REG_D1;
  else registersD[D4] &= ~REG_D1;
  
  // Дверь ТБК
  if(UBS.door_tbk & 0x01)  registersD[D4] |= REG_D2;
  else registersD[D4] &= ~REG_D2;
   

}

void SetDataFromRequestCOMPONENT()
{
  RequestCount = 0;
  // Авария УБС
  if(COMPONENT.ubs & 0x08) registersD[D5] |= REG_D1;
  else { if (Deblock) registersD[D5] &= ~REG_D1; }
  
  // Нет связи ВВГ
  // если не включено 380, то "нет связи" с ВВГ не считается за аварию!
  // ВВГ не включается мгновенно, поэтому после включения 380 следует подождать 30 секунд 
  if (flag_380)  // есть сеть, таймер кончился
  {
    if (!TimeCommon)
    {
      if(COMPONENT.vvg & 0x08) fail_vvg |= 0x04;
      else fail_vvg &= ~0x04;
    }
  }
  // Авария ВВГ
  if(COMPONENT.vvg & 0x01) fail_vvg |= 0x02;
  else fail_vvg &= ~0x02;
  // Авария ОЗОН
  if((COMPONENT.datch_h & 0x03) /*|| (COMPONENT.datch_l & 0x07)*/) fail_ozon |= 0x04;
  else fail_ozon &= ~0x04;
  // Авария СКиН
  if(COMPONENT.skin & 0x01)  fail_skin|= 0x01;
  else fail_skin &= ~0x01;
  // Авария ТБК
  if(COMPONENT.tbk & 0x01) fail_tbk |= 0x01;
  else fail_tbk &= ~0x01;
  // Нет связи ТБК
  if(COMPONENT.tbk & 0x08) fail_tbk |= 0x02;
  else fail_tbk &= ~0x02;
  // Температура ТБК
  if(COMPONENT.tbk & 0x02) fail_tbk |= 0x04;
  else fail_tbk &= ~0x04;
  // Давление ТБК
  if(COMPONENT.tbk & 0x04) fail_tbk |= 0x08;
  else fail_tbk &= ~0x08;
  
   
  // собирательная авария ОЗОН
  if(fail_ozon) registersD[D5] |= REG_D2;
  else { if (Deblock) registersD[D5] &= ~REG_D2; }
  
  // собирательная авария ВВГ
  if(fail_vvg) registersD[D5] |= REG_D3;
  else { if (Deblock)  registersD[D5] &= ~REG_D3; }
  
  // сбирательная авария ТБК 
  if(fail_tbk) registersD[D5] |= REG_D4;
  else { if (Deblock)  registersD[D5] &= ~REG_D4; }

   
}



void SetDataFromRequestVVG()
{

  
  RequestCount = 0;
  // если нет сети, то обнулять все, что приходит в сообщении
  // выводить параметры от ВВГ только при 380
  // если не включено 380, то "нет связи" с ВВГ не считается за аварию!
  // если есть сеть, но нет связи с ВВГ, то оставлять старые значения
  
  if (flag_380) // СЕТЬ 1 / 380
  {
  ////////////////////////////////////////
  // НАПРЯЖЕНИЕ И ТОК
  // если включен ЗОИ
  if(UBS.zoi)
  {
    SetNumbersOnSevenSegmentsD9(VVG.U1, 0, 1);
    SetNumbersOnSevenSegmentsD9(VVG.I1, 4, 2);
  }
    
  // если включен ТБК
  else if(UBS.tbk) 
  {
    SetNumbersOnSevenSegmentsD9(VVG.U2, 0, 1);
    SetNumbersOnSevenSegmentsD9(VVG.I2, 4, 2);
  }   
  // если ничего не включено
  else 
  {
    SetNumbersOnSevenSegmentsD9(zero_mass, 0, 1);
    SetNumbersOnSevenSegmentsD9(zero_mass, 4, 2);
  }
 }
 else 
 {
   SetNumbersOnSevenSegmentsD9(zero_mass, 0, 1);
   SetNumbersOnSevenSegmentsD9(zero_mass, 4, 2);
 }
  
  
  // Авария I max
  if(VVG.fail_l & 0x02)  registersD[D4] |= REG_D4;
  else { if (Deblock) registersD[D4] &= ~REG_D4; }
  // Авария ВВГ
  if(VVG.fail_h & 0x01)  fail_vvg |= 0x01;
  else fail_vvg &= ~0x01;
  // Авария U 10%
  if(VVG.fail_h & 0x02)  registersD[D4] |= REG_D6;
  else { if (Deblock)  registersD[D4] &= ~REG_D6; }
  // Авария I кз
  if(VVG.fail_h & 0x04)  registersD[D4] |= REG_D5;
  else { if (Deblock) registersD[D4] &= ~REG_D5; }
  
  
  
  // Отпирание ВВГ
  if(VVG.state & 0x01)  registersD[D7] |= REG_D6;
  else registersD[D7] &= ~REG_D6;
  // ВВГ F1
  if(VVG.state & 0x02)  registersD[D6] |= REG_D4;
  else registersD[D6] &= ~REG_D4;
  // ВВГ F2
  if(VVG.state & 0x04)  registersD[D6] |= REG_D5;
  else registersD[D6] &= ~REG_D5;
  // ВВГ F3
  if(VVG.state & 0x08)  registersD[D6] |= REG_D6;
  else registersD[D6] &= ~REG_D6;
  
  
   
}

void SetDataFromRequestDATCHIK()
{
  // отладка АЦП
  /*DATCHIK.t_zoi[0] = vp1_par/100;
  DATCHIK.t_zoi[1] = (vp1_par%100)/10;
  DATCHIK.t_zoi[2] = vp1_par % 10;
  DATCHIK.t_zoi[3] = 0;*/
  
  ///////////////////////////////////////// конец отладки
  RequestCount = 0;
  ////////////////////////////////////////
  // ТЕМПЕРАТУРА
  // если включен ЗОИ
  if(UBS.zoi)
  {
    // Ставим температуру ЗОИ
    SetNumbersOnSevenSegmentsD8(DATCHIK.t_zoi, 4, 2);
    // Ставим влажность
    SetNumbersOnSevenSegmentsD8(DATCHIK.vl_zoi, 8, 2);
    
  } 
  // если включен ТБК
  else if(UBS.tbk) 
  {
    // Ставим температуру ТБК
    SetNumbersOnSevenSegmentsD8(DATCHIK.t_tbk, 4, 2);
    // Ставим давление
    SetNumbersOnSevenSegmentsD8(DATCHIK.p_tbk, 8, 5);
  }   
  // если ничего не включено/ошибка/итд
  else 
  {
    SetNumbersOnSevenSegmentsD8(zero_mass, 4, 2);
    SetNumbersOnSevenSegmentsD8(zero_mass, 8, 5);
  }
  // ТАЙМЕР
  SetNumbersOnSevenSegmentsD8(DATCHIK.time, 0, 5);
  
}


void SetNumbersOnSevenSegmentsD9(unsigned char *array, unsigned char index, unsigned char DP_)
{
  for (int i = 0; i< 4; i++)
  {
    switch (array[i])
    {
    case 0: sevenSegmentD9[index+i] = ZERO; break;
    case 1: sevenSegmentD9[index+i] = ONE; break;
    case 2: sevenSegmentD9[index+i] = TWO; break;
    case 3: sevenSegmentD9[index+i] = THREE; break;
    case 4: sevenSegmentD9[index+i] = FOUR; break;
    case 5: sevenSegmentD9[index+i] = FIVE; break;
    case 6: sevenSegmentD9[index+i] = SIX; break;
    case 7: sevenSegmentD9[index+i] = SEVEN; break;
    case 8: sevenSegmentD9[index+i] = EIGHT; break;
    case 9: sevenSegmentD9[index+i] = NINE; break;
    }
    
    if(DP_ == i) sevenSegmentD9[index+i] |= DP;
  }
}
void SetNumbersOnSevenSegmentsD8(unsigned char *array, unsigned char index, unsigned char DP_)
{
  for (int i = 0; i< 4; i++)
  {
    switch (array[i])
    {
    case 0: sevenSegmentD8[index+i] = ZERO; break;
    case 1: sevenSegmentD8[index+i] = ONE; break;
    case 2: sevenSegmentD8[index+i] = TWO; break;
    case 3: sevenSegmentD8[index+i] = THREE; break;
    case 4: sevenSegmentD8[index+i] = FOUR; break;
    case 5: sevenSegmentD8[index+i] = FIVE; break;
    case 6: sevenSegmentD8[index+i] = SIX; break;
    case 7: sevenSegmentD8[index+i] = SEVEN; break;
    case 8: sevenSegmentD8[index+i] = EIGHT; break;
    case 9: sevenSegmentD8[index+i] = NINE; break;
    case 0x2D: sevenSegmentD8[index+i] = MINUS; break;
    case 0x2B: sevenSegmentD8[index+i] = ZERO; break; ////////////////////////// отключать 
    }
    if(DP_ == i) sevenSegmentD8[index+i] |= DP;
  }
}
//  ********************  CONTROL.CPP  ********************
