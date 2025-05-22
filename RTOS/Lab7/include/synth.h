#pragma once

#include "hardware/adc.h"
#include "hardware/clocks.h"
#include "hardware/dma.h"
#include "hardware/pwm.h"
#include "math.h"
#include "pico/stdlib.h"
#include "string.h"

void init_synth(uint gpio);
void synth_set_frequency(uint frequency);
void synth_set_volume(float volume);
void synth_resume();
void synth_suspend();
uint synth_get_frequency();
uint synth_get_volume();