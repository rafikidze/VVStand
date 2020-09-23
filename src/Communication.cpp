//  ********************  COMMUNICATION.CPP  ********************
#include <Common.h>
#include "Communication.h"

unsigned short  DataCommR = 0,
                DataCommT = 0;
unsigned char SystemError = 0x00;

void DoCommunication(void)
{
  Receive();
  Transmite();
}

void Receive(void)
{

//#ifdef         UART_ADU
  UART_ADU_Receive();
  
 // UART_TECHNO_Receive();
  
//#endif

}

void Transmite(void)
{
//#ifdef         UART_ADU
  Uart_ADU.DoTx();  //UART_ADU_Transmite();
 // Uart_TECHNO.DoTx(); 
//#endif

}

void ClearIn(unsigned short Data)
{
  unsigned short Flag;
  Flag = 1;
  while (Flag < Data)
  {
    switch (Flag & Data)
    {
//#ifdef         UART_ADU
    case UART_ADU:
      ClearBuffer(&Uart_ADU.In);
      break;
//#endif

    }
    Flag = Flag << 1;
  }
}

void ClearOut(unsigned short Data)
{
  unsigned short Flag;
  Flag = 1;
  while (Flag < Data)
  {
    switch (Flag & Data)
    {
//#ifdef         UART_ADU
    case UART_ADU:
      ClearBuffer(&Uart_ADU.Out);
      break;
//#endif

    }
    Flag = Flag << 1;
  }
}

//  ********************  COMMUNICATION.CPP  ********************
