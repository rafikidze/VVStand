

//  ********************  COMMON.H  ********************

#include <ina90.h>
#include <ioxm128a1.h>

#include "DefineAll.h"
#include "Mutex.h"
#include "Buffer.h"
#include "Timer.h"
#include "ADC.h"
#include "uart.h"

#define PRAGMA(x) _Pragma( #x )
#define InterruptServiceRoutine(vec) PRAGMA( vector=vec ) __interrupt void handler_##vec(void)

#define CCP_IOREG                         0xD8      // IO Register Protection

#define NULL (void *)0

#define   TRUE      0x01
#define   FALSE     0x00


//  ********************  COMMON.H  ********************

