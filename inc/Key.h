
//  ********************  KEY.H  ********************

#define _KEY_PRESS_ENABLE_          0x01
#define _KEY_REPEAT_NEED_           0x02

class CKEY
{
  unsigned long Last;
  unsigned char Repeat;             //  количество совпадений с предыдущим
  unsigned char KeyBePressed;       //  количество нажатых клавиш
  bool Enable;                      //  разрешение работы с клавиатурой
  
 
public:
  unsigned char Head;           //  текущий
  
  unsigned char prev_Get;
  bool strob_sirena;
  
  void SetEnable(void);
  bool ScanPortH(void);
  bool ScanPortK(void);
  unsigned long GetCode(void);
  unsigned long GetHead(void);
  void SetHead(void); 
  
  //////////////////////
  bool KeyRelease(void);
  void init(void);
};

//  ********************  KEY.H  ********************

