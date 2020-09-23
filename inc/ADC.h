
//  ********************  ADC.H  ********************

/*
// ACB interrupt vectors
#define ACB_ACW_vect 0x0098 // ACW Window Mode Interrupt
#define ACB_AC1_vect 0x0094 // AC1 Interrupt
#define ACB_AC0_vect 0x0090 // AC0 Interrupt

// ADCB interrupt vectors
#define ADCB_CH0_vect 0x009C // Interrupt 0
#define ADCB_CH1_vect 0x00A0 // Interrupt 1
#define ADCB_CH2_vect 0x00A4 // Interrupt 2
#define ADCB_CH3_vect 0x00A8 // Interrupt 3

#define DMA_CH_TRIGSRC_ADCB_CH0_gc (0x20<<0)	// ADCB Channel 0
#define DMA_CH_TRIGSRC_ADCB_CH1_gc (0x21<<0)	// ADCB Channel 1
#define DMA_CH_TRIGSRC_ADCB_CH2_gc (0x22<<0)	// ADCB Channel 2
#define DMA_CH_TRIGSRC_ADCB_CH3_gc (0x23<<0)	// ADCB Channel 3
#define DMA_CH_TRIGSRC_ADCB_CH4_gc (0x24<<0)	// ADCB Channel 0,1,2,3 combined
#define DMA_CH_TRIGSRC_DACB_CH0_gc (0x25<<0)	// DACB Channel 0
#define DMA_CH_TRIGSRC_DACB_CH1_gc (0x26<<0)	// DACB Channel 1

*/

#define ADC_FIFO_SIZE         0x40
#define ADC_FIFO_LIMIT        0x3F
#define ADC_HALF_FIFO_LIMIT   0x20
//#define ADC_FIFO_MEMORY       (ADC_FIFO_SIZE << 3)

#define 	ADCACAL0            0x20        // ADCB Calibration Byte 0
#define 	ADCACAL1            0x21        // ADCB Calibration Byte 1

#define 	ADCBCAL0            0x24        // ADCB Calibration Byte 0
#define 	ADCBCAL1            0x25        // ADCB Calibration Byte 1

#define NVM_CMD_READ_CALIB_ROW      0x02        // Read calibration row
#define NVM_CMD_NO_OPERATION        0x00        // Noop/Ordinary LPM
#define PROD_SIGNATURES_START 0x0000

//  MUXCTRL
#define ADC_CH_TEMP                 0x00        // Positive Input Select
#define ADC_CH_BANDGAP              0x01        // Positive Input Select
#define ADC_CH_SCALEDVCC            0x02        // Positive Input Select
#define ADC_CH_DAC                  0x03        // Positive Input Select

#define ADC_CH_MUX_0                0x00        // Positive Input Select bit 0
#define ADC_CH_MUX_1                0x01        // Positive Input Select bit 1
#define ADC_CH_MUX_2                0x02        // Positive Input Select bit 2
#define ADC_CH_MUX_3                0x03        // Positive Input Select bit 3
#define ADC_CH_MUX_4                0x04        // Positive Input Select bit 4
#define ADC_CH_MUX_5                0x05        // Positive Input Select bit 5
#define ADC_CH_MUX_6                0x06        // Positive Input Select bit 6
#define ADC_CH_MUX_7                0x07        // Positive Input Select bit 7

#define ADC_ENABLE                  0x01        //
#define ADC_CH_START                0x80        //

//  REFCTRL
#define REF_INT1V   0x00
#define REF_INTVCC  0x10
#define REF_AREFA   0x20
#define REF_AREFB   0x30

//  CTRLA
#define CHN_0_START     0x04
#define CHN_01_START    0x0C
#define CHN_0123_START  0x3C
//  CTRLB
#define ADC_CONVMODE                0x10
#define ADC_FREERUN                 0x08        //
#define ADC_RESOLUTION_12R          0x00        // 12 right
#define ADC_RESOLUTION_8R           0x04        // 12 right
#define ADC_RESOLUTION_12L          0x06        // 12 right

//  CTRL
#define ADC_INTERNAL                0x00
#define ADC_SINGLEENDED             0x01
#define ADC_DIFF                    0x02
#define ADC_DIFFWGAIN               0x03

// ADC_ucSkip
#define ADC_SKIP                    0x01        //
#define ADC_AVERAGE                 0x02        //
#define ADC_FILTER                  0x04        //

//  ADC_status
#define ADC_COMPLETE                0x08        //
#define ADC_DO                      0x10        //
#define ADC_FIFO                    0x80        //

//  ADC_Command
#define ADC_INIT                    0x01
#define ADC_START                   0x02
#define ADC_RESTART                 0x20
#define ADC_STOP                    0x04
#define ADC_CLOSE                   0x08

//  для измерений
#define MAX_CHANNEL_BASE            0x04 //2
#define MAX_CHANNEL_ADD             0x04
//#define LIMIT_CHANNEL               0x03

#define ADC_FALSE_COUNT             0x04      //  количество ложных измерений

#define PSC_8000k                   0x00
#define PSC_4000k                   0x01
#define PSC_2000k                   0x02
#define PSC_1000k                   0x03
#define PSC_500k                    0x04
#define PSC_250k                    0x05
#define PSC_125k                    0x06
#define PSC_62500                   0x07
//#define PSC_31250                   0x06
//#define PSC_15625                   0x07

//  EVENT
#define EVENT_SWEEP_0              0x00
#define EVENT_SWEEP_01              0x40
#define EVENT_SWEEP_0123            0xC0

#define EVENT_LINE_7                0x38
#define EVENT_SYNC_SWEEP            0x06

class CADC
{
  ADC_t *pADC;
  unsigned char ucPreScaler;
  unsigned char ucControl;
  unsigned char ucEvent;
public:
  unsigned char ucStopChannel, ucIntMode, ucIntLevel;
  unsigned char ucSkip;
  unsigned char ucStart;
  //long int DimensionControl;

  unsigned char Status;           //, Measure;
  unsigned short Get, Count;      //, CountControl;
  unsigned short Done;            //  количество обработанных
  short int *psData;
  //  регистры
  void            Init(     ADC_t *pField,
                            unsigned char Control,
                            unsigned char Event,
                            unsigned char Start,
                            unsigned char StopChannel,
                            unsigned char IntMode,
                            unsigned char IntLevel,
                            unsigned char Cal0,       //  CALL
                            unsigned char Cal1,       //  CALH
                            unsigned char Config,     //  CTRLB
                            unsigned char Reference,  //  REFCTRL
                            unsigned char PreScaler);
  void            Do(void);
  void            ChannelConfig(  unsigned char Index,
                                  unsigned char Mode,
                                  unsigned char Gain,
                                  unsigned char MuxPos,
                                  unsigned char MuxNeg);
  unsigned char   ChannelReady(   unsigned char Index);
  void            SetChannelInterrupt(  unsigned char Index,
                                        unsigned char Mode,
                                        unsigned char Level);
  void            StartChannel(   unsigned char Index);
  short int       GetChannel(    unsigned char Index);

  void            Start(    void);
  void            ReStart(    void);//  повторный пуск - успеть обработать
  void            Stop(    void);//

  void            SetSkip(  unsigned char Data);//
  unsigned char   GetSkip( void );


};

//  ADC_var.h
extern short            ADC_Offset;
extern unsigned char    ADC_PreScaler,
                        ADC_PSC_MIN;

#ifdef PEAK
#endif


//  ********************  ADC.H  ********************
