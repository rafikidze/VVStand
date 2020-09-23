//  ********************  DANGEROUS.H  ********************
#define DANGEROUS_CLOSE_PUSH     0x01

#define DANGEROUS_ON             0x01
#define DANGEROUS_OFF            0x02

extern unsigned char DangerousSet, DangerousGet;

extern unsigned char DangerousState;
void InitDangerousButton(void);
//  ********************  DANGEROUS.H  ********************
