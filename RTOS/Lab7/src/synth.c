#include "synth.h"
#include "hardware/adc.h"
#include "hardware/dma.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"
#include <math.h>
#include <string.h>

#define SINE_TABLE_SIZE 4096
#define PWM_FREQ_HZ 600000u // Fixed PWM frequency
#define PWM_RESOLUTION 250
#define VOLUME_ADC_INPUT 27
#define VOLUME_ADC_CHANNEL 1

static uint16_t dma_buffer[SINE_TABLE_SIZE];
static uint dma_chan;
static uint slice_num;
static uint pwm_channel;

static volatile uint current_frequency = 200; // Start at 440 Hz
static volatile float current_volume = 1.0f;

static uint synth_gpio;

void fill_dma_buffer() {
  uint period_samples = PWM_FREQ_HZ / current_frequency;

  // Adjust the sample count if the period is too large (should not happen in our case)
  if (period_samples > SINE_TABLE_SIZE)
    period_samples = SINE_TABLE_SIZE;

  // Generate sine wave based on the current frequency and volume
  for (uint i = 0; i < period_samples; i++) {
    float phase = 2.0f * M_PI * i / period_samples;
    float val = (sinf(phase) * 0.5f + 0.5f) * current_volume;
    dma_buffer[i] = (uint16_t)(val * PWM_RESOLUTION);
  }

  // Update DMA configuration
  dma_channel_set_read_addr(dma_chan, dma_buffer, false);
  dma_channel_set_trans_count(dma_chan, period_samples, true);
}

static void dma_handler() {
  dma_hw->ints0 = 1u << dma_chan; // Clear interrupt

  fill_dma_buffer();
}

void init_synth(uint gpio) {
  synth_gpio = gpio;

  // GPIO & PWM setup
  gpio_init(gpio);
  gpio_set_function(gpio, GPIO_FUNC_PWM);

  slice_num = pwm_gpio_to_slice_num(gpio);
  pwm_channel = pwm_gpio_to_channel(gpio);

  pwm_config cfg = pwm_get_default_config();
  pwm_config_set_clkdiv_mode(&cfg, PWM_DIV_FREE_RUNNING);
  pwm_config_set_wrap(&cfg, PWM_RESOLUTION - 1);
  pwm_config_set_clkdiv(&cfg, (float)clock_get_hz(clk_sys) / (PWM_FREQ_HZ * PWM_RESOLUTION));
  pwm_init(slice_num, &cfg, true);

  // ADC
  gpio_init(VOLUME_ADC_INPUT);
  adc_init();
  adc_gpio_init(VOLUME_ADC_INPUT);
  adc_select_input(VOLUME_ADC_CHANNEL);

  // DMA setup
  dma_chan = dma_claim_unused_channel(true);
  dma_channel_config dma_cfg = dma_channel_get_default_config(dma_chan);

  channel_config_set_transfer_data_size(&dma_cfg, DMA_SIZE_16);
  channel_config_set_read_increment(&dma_cfg, true);
  channel_config_set_write_increment(&dma_cfg, false);
  channel_config_set_dreq(&dma_cfg, DREQ_PWM_WRAP0 + slice_num);
  channel_config_set_ring(&dma_cfg, false, 0); // No ring buffer

  dma_channel_configure(dma_chan, &dma_cfg,
                        &pwm_hw->slice[slice_num].cc + pwm_channel, // PWM duty register
                        dma_buffer,                                 // initial buffer
                        0,                                          // initial count (set in fill_dma_buffer)
                        false                                       // don't start yet
  );

  // DMA interrupt
  dma_channel_set_irq1_enabled(dma_chan, true);
  irq_set_exclusive_handler(DMA_IRQ_1, dma_handler);
  irq_set_enabled(DMA_IRQ_1, true);

  // Prime buffer and start
  fill_dma_buffer(current_frequency, current_volume);
}

void synth_suspend() {
  irq_set_enabled(DMA_IRQ_1, false);
  pwm_set_enabled(slice_num, false);
  gpio_set_function(synth_gpio, GPIO_FUNC_SIO);
  gpio_set_dir(synth_gpio, GPIO_OUT);
  gpio_put(synth_gpio, 1);
}

void synth_resume() {
  gpio_set_function(synth_gpio, GPIO_FUNC_PWM);
  pwm_set_enabled(slice_num, true);

  adc_run(false);
  adc_fifo_setup(false, false, 0, false, false);

  adc_select_input(VOLUME_ADC_CHANNEL);
  sleep_ms(20);
  // adc_run(true);
  while (!adc_hw->cs & ADC_CS_READY_BITS)
    ;

  irq_set_enabled(DMA_IRQ_1, true);
}

void synth_set_frequency(uint frequency) { current_frequency = frequency; }

void synth_set_volume(float volume) { current_volume = volume; }

uint synth_get_frequency() { return current_frequency; }

uint synth_get_volume() { return current_volume * 100; }