#define TIME_EVENT    0x000000C8UL  //0xC8 - 200ìêñ

extern CTIMER0 Timer_D0;

extern unsigned long   TimeEvent;

void StartTimerEvent( unsigned long pause);
void CloseTimerEvent(void);
