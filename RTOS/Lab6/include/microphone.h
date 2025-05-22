#define SAMPLE_RATE             48000
#define MIC_PIN                 26
#define AUDIO_BUFFER_SIZE       12000
#define ADC_CHANNEL             0

#define THRESHOLD_HIGH          3000
#define THRESHOLD_LOW           1000
#define MAX_CLAP_LENGTH         100

#include <stdint.h>

int init_microphone(uint16_t* adc_buf);