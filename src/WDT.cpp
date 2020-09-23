//  ********************  WDT.CPP  ********************
#include <Common.h>
#include "WDT.h"

void WDT_Enable( void )
{
  unsigned char Temp = WDT.CTRL | WDT_ENABLE | WDT_CEN;
  CCP = CCP_IOREG;
  WDT.CTRL = Temp;
  while(WDT.STATUS & WDT_BUSY)
  { }
}

void WDT_Disable( void )
{
  unsigned char Temp = WDT.CTRL & ~(WDT_ENABLE | WDT_CEN);
  CCP = CCP_IOREG;
  WDT.CTRL = Temp;
}

void WDT_SetTime( unsigned char TimeOut)
{
  unsigned char Temp;
  WDT_Disable();
  Temp = (TimeOut << 2)  | WDT_ENABLE | WDT_CEN;
  CCP = CCP_IOREG;
  WDT.CTRL = Temp;
  while(WDT.STATUS & WDT_BUSY)
  { }
}

//  ********************  WDT.CPP  ********************
