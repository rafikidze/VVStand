
//  ********************  TIMER.H  ********************


#define       TIME0_0                       0x00    //
#define       TIME0_1                       0x01    //
#define       TIME0_2                       0x02    //
#define       TIME0_3                       0x03    //
#define       TIME0_4                       0x04    //
#define       TIME0_5                       0x05    //
#define       TIME0_6                       0x06    //
#define       TIME0_7                       0x07    //

#define       TIMER0_F                      0x8000    //
#define       TIMER0_E                      0x4000    //
#define       TIMER0_D                      0x2000    //
#define       TIMER0_C                      0x1000    //
#define       TIMER0_B                      0x0800    //
#define       TIMER0_A                      0x0400    //
#define       TIMER0_9                      0x0200    //
#define       TIMER0_8                      0x0100    //

#define       TIMER0_7                      0x0080    //
#define       TIMER0_6                      0x0040    //
#define       TIMER0_5                      0x0020    //
#define       TIMER0_4                      0x0010    //
#define       TIMER0_3                      0x0008    //
#define       TIMER0_2                      0x0004    //
#define       TIMER0_1                      0x0002    //
#define       TIMER0_0                      0x0001    //

#define       TIMER_DONE                    BIT_0

/*
// TCC0 interrupt vectors
#define TCC0_OVF_vect 0x0038 // Overflow Interrupt
#define TCC0_ERR_vect 0x003C // Error Interrupt
#define TCC0_CCA_vect 0x0040 // Compare or Capture A Interrupt
#define TCC0_CCB_vect 0x0044 // Compare or Capture B Interrupt
#define TCC0_CCC_vect 0x0048 // Compare or Capture C Interrupt
#define TCC0_CCD_vect 0x004C // Compare or Capture D Interrupt

// TCC1 interrupt vectors
#define TCC1_OVF_vect 0x0050 // Overflow Interrupt
#define TCC1_ERR_vect 0x0054 // Error Interrupt
#define TCC1_CCA_vect 0x0058 // Compare or Capture A Interrupt
#define TCC1_CCB_vect 0x005C // Compare or Capture B Interrupt

// TCD0 interrupt vectors
#define TCD0_OVF_vect 0x0134 // Overflow Interrupt
#define TCD0_ERR_vect 0x0138 // Error Interrupt
#define TCD0_CCA_vect 0x013C // Compare or Capture A Interrupt
#define TCD0_CCB_vect 0x0140 // Compare or Capture B Interrupt
#define TCD0_CCC_vect 0x0144 // Compare or Capture C Interrupt
#define TCD0_CCD_vect 0x0148 // Compare or Capture D Interrupt

// TCD1 interrupt vectors
#define TCD1_OVF_vect 0x014C // Overflow Interrupt
#define TCD1_ERR_vect 0x0150 // Error Interrupt
#define TCD1_CCA_vect 0x0154 // Compare or Capture A Interrupt
#define TCD1_CCB_vect 0x0158 // Compare or Capture B Interrupt

// TCE0 interrupt vectors
#define TCE0_OVF_vect 0x00BC // Overflow Interrupt
#define TCE0_ERR_vect 0x00C0 // Error Interrupt
#define TCE0_CCA_vect 0x00C4 // Compare or Capture A Interrupt
#define TCE0_CCB_vect 0x00C8 // Compare or Capture B Interrupt
#define TCE0_CCC_vect 0x00CC // Compare or Capture C Interrupt
#define TCE0_CCD_vect 0x00D0 // Compare or Capture D Interrupt

// TCE1 interrupt vectors
#define TCE1_OVF_vect 0x00D4 // Overflow Interrupt
#define TCE1_ERR_vect 0x00D8 // Error Interrupt
#define TCE1_CCA_vect 0x00DC // Compare or Capture A Interrupt
#define TCE1_CCB_vect 0x00E0 // Compare or Capture B Interrupt

// TCF0 interrupt vectors
#define TCF0_OVF_vect 0x01B0 // Overflow Interrupt
#define TCF0_ERR_vect 0x01B4 // Error Interrupt
#define TCF0_CCA_vect 0x01B8 // Compare or Capture A Interrupt
#define TCF0_CCB_vect 0x01BC // Compare or Capture B Interrupt
#define TCF0_CCC_vect 0x01C0 // Compare or Capture C Interrupt
#define TCF0_CCD_vect 0x01C4 // Compare or Capture D Interrupt

*/

unsigned char set_pause(unsigned long, unsigned char *, unsigned short int *);

#define         TIMER_OVF       0x01

class CTIMER0
{
  TC0_t *pTimer;
public:
  unsigned short Start;
  unsigned char Count;
  void Init(TC0_t *, unsigned long, unsigned char, unsigned char );  //  пауза в мкс, Level, Mask
  unsigned char Is(unsigned char);
  void Close(unsigned char );
  void CounterIncrement(void);
};

//  ********************  TIMER.H  ********************

