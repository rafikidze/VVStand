//  ********************  DANGEROUS.CPP  ********************
#include <Common.h>
#include <Dangerous.h>
//__interrupt void PORTA_INT0_interrupt(void)
unsigned char DangerousSet = 0x00, DangerousGet = 0x00;
unsigned char DangerousState = 0x00;

InterruptServiceRoutine(PORTC_INT0_vect)
{
  unsigned char _t;
  _t = __save_interrupt();
  __disable_interrupt();

  DangerousGet = PORTC.IN & DANGEROUS_CLOSE_PUSH;
  if (DangerousGet ^ DangerousSet)
  {
    DangerousSet = DangerousGet;
    if (DangerousSet & DANGEROUS_CLOSE_PUSH)
    {// АВАРИЯ
      DangerousState = DANGEROUS_ON;
    }
    else
    {// НОРМА
      DangerousState = DANGEROUS_OFF;
    }
  }
  else;// иголка - ничего не изменяем

  __restore_interrupt(_t);
}

void InitDangerousButton(void)
{
  /*PORTC.OUTSET = DANGEROUS_CLOSE_PUSH;
  PORTC.INTCTRL = 0x02;         // High Level Interrupt
  PORTC.INT0MASK = DANGEROUS_CLOSE_PUSH;
  PORTC.PIN2CTRL = 0x00;        //  Конфигурация PIN "POWER_ON"*/
}
//  ********************  DANGEROUS.CPP  ********************
