
//  ********************  COMMUNICATION.H  ********************

//  Communications mask
//   ***** PORTD *****
//   ***** PORTE *****

//  ***** PORTC *****
#define        UART_ADU        0x0010
#ifdef         UART_ADU
#include "uart_adu.h"
#endif

//   ***** PORTF *****
//#define         UART_GPS      0x0020
#ifdef         UART_GPS
#include "uart_gps.h"
#endif

//#define         SPI_PLM       0x0040
#ifdef         SPI_PLM
#include "spi_plm.h"
#endif

//  Прототипы функций
void DoCommunication(void);
void Receive(void);
void Transmite(void);
void ClearIn(unsigned short Data);
void ClearOut(unsigned short Data);

extern unsigned short DataCommR, DataCommT;

extern unsigned char SystemError;


//  ********************  COMMUNICATION.H  ********************

