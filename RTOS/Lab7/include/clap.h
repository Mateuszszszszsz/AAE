#pragma once

// clang-format off
#define SAMPLE_RATE         48000
#define MIC_PIN             26
#define AUDIO_BUFFER_SIZE   12000
#define CLAP_ADC_CHANNEL    0
// clang-format on

#include "FreeRTOS.h"
#include "hardware/adc.h"
#include "hardware/clocks.h"
#include "hardware/dma.h"
#include "hardware/timer.h"
#include "pico/stdlib.h"
#include "task.h"

#include <stdint.h>

int clap_init_microphone(uint16_t *adc_buf);
bool clap_detect(uint16_t *audio_buffer, size_t start, size_t end);
void clap_resume();
void clap_suspend();