
//  ********************  UART.H  ********************

class CUSART
{
  USART_t *pUSART;
  PORT_t *pBasePORT;
  PORT_t *pEnablePORT;
public:
  unsigned char pinTx;
  unsigned char pinEn;
  unsigned char Level;
  unsigned char Delay;
  BUFFER In, Out;
  void Init(USART_t *pField, PORT_t *BasePORT, unsigned char TxPIN,//  EN -PORT,PIN;
                             PORT_t *EnablePORT, unsigned char EnPIN,//  BUFFERR IN-OUT,
                             unsigned long  Baud,                 //  baud rate,//BAUDCTRLA-B
                             signed char    BaudSel,              //BAUDCTRLB
                             unsigned char  Frame,                //  frame format,//CTRLC
                             unsigned char  SetLevel,             //  level interrupt,//CTRLA
                             unsigned char  Control);       //  control//
  void Clear(void);               //  очистка буфера приема
  void DoRxC(void);               //  (прерывание Rx)
  void DoTx(void);                //  запуск передачи по UART
  void DoTxC(void);               //  (прерывание Tx)
  void Close(void);               //  завершение работы UART
  unsigned char IsRxC(void);
  void EnableRxC(void);    //  разрешение приема по UART
  void DisableRxC(void);   //  запрет приема по UART
};


// USART.STATUS bit masks and bit positions
#define USART_RXCIF       0x80 // Receive Interrupt Flag bit mask
#define USART_TXCIF       0x40 // Transmit Interrupt Flag bit mask
#define USART_DREIF       0x20 // Data Register Empty Flag bit mask
#define USART_FERR        0x10 // Frame Error bit mask
#define USART_BUFOVF      0x08 // Buffer Overflow bit mask
#define USART_PERR        0x04 // Parity Error bit mask
#define USART_RXB8        0x01 // Receive Bit 8 bit mask

// USART.CTRLA bit masks and bit positions
#define USART_RXCINTLVL   0x10 // Receive Interrupt Level group mask                //0x30
#define USART_TXCINTLVL   0x04 // Transmit Interrupt Level group mask               //0x0C
#define USART_DREINTLVL   0x01 // Data Register Empty Interrupt Level group mask    //0x03

// USART.CTRLB bit masks and bit positions
#define USART_RXEN        0x10 // Receiver Enable bit mask
#define USART_TXEN        0x08 // Transmitter Enable bit mask
#define USART_CLK2X       0x04 // Double transmission speed bit mask
#define USART_MPCM        0x02 // Multi-processor Communication Mode bit mask
#define USART_TXB8        0x01 // Transmit bit 8 bit mask

// USART.CTRLC bit masks and bit positions
#define USART_CMODE       0xC0 // Communication Mode group mask
#define USART_PMODE       0x30 // Parity Mode group mask
#define USART_SBMODE      0x08 // Stop Bit Mode bit mask
#define USART_CHSIZE      0x07 // Character Size group mask

// USART.BAUDCTRLA bit masks and bit positions
#define USART_BSEL        0xFF // Baud Rate Selection Bits [7:0] group mask

// USART.BAUDCTRLB bit masks and bit positions
#define USART_BSCALE      0xF0 // Baud Rate Scale group mask
// Masks for BSEL aready defined

#define   ASYNC             0x00
#define   PARITY_DISABLE    0x00
#define   PARITY_EVEN       0x20
#define   STOP_2            0x08
#define   STOP_1            0x00
#define   INFO_8BIT         0x03

/*
// USARTC0 interrupt vectors
#define USARTC0_RXC_vect 0x0064 // Reception Complete Interrupt
#define USARTC0_DRE_vect 0x0068 // Data Register Empty Interrupt
#define USARTC0_TXC_vect 0x006C // Transmission Complete Interrupt

// USARTC1 interrupt vectors
#define USARTC1_RXC_vect 0x0070 // Reception Complete Interrupt
#define USARTC1_DRE_vect 0x0074 // Data Register Empty Interrupt
#define USARTC1_TXC_vect 0x0078 // Transmission Complete Interrupt

// USARTE0 interrupt vectors
#define USARTE0_RXC_vect 0x00E8 // Reception Complete Interrupt
#define USARTE0_DRE_vect 0x00EC // Data Register Empty Interrupt
#define USARTE0_TXC_vect 0x00F0 // Transmission Complete Interrupt

// USARTE1 interrupt vectors
#define USARTE1_RXC_vect 0x00F4 // Reception Complete Interrupt
#define USARTE1_DRE_vect 0x00F8 // Data Register Empty Interrupt
#define USARTE1_TXC_vect 0x00FC // Transmission Complete Interrupt

// USARTD0 interrupt vectors
#define USARTD0_RXC_vect 0x0160 // Reception Complete Interrupt
#define USARTD0_DRE_vect 0x0164 // Data Register Empty Interrupt
#define USARTD0_TXC_vect 0x0168 // Transmission Complete Interrupt

// USARTD1 interrupt vectors
#define USARTD1_RXC_vect 0x016C // Reception Complete Interrupt
#define USARTD1_DRE_vect 0x0170 // Data Register Empty Interrupt
#define USARTD1_TXC_vect 0x0174 // Transmission Complete Interrupt

// USARTF0 interrupt vectors
#define USARTF0_RXC_vect 0x01DC // Reception Complete Interrupt
#define USARTF0_DRE_vect 0x01E0 // Data Register Empty Interrupt
#define USARTF0_TXC_vect 0x01E4 // Transmission Complete Interrupt
*/

//  ********************  UART.H  ********************

