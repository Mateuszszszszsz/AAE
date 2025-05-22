#include "clap.h"

int dma_chan;
uint16_t *mic_buf;

bool repeating_timer_callback(struct repeating_timer *t) {
  if (!dma_channel_is_busy(dma_chan)) {

    return false;
  }

  adc_hw->cs |= ADC_CS_START_ONCE_BITS;
  return true;
}

void dma_handler() {
  dma_hw->ints0 = 1u << dma_chan;

  dma_channel_set_read_addr(dma_chan, &adc_hw->fifo, false);
  dma_channel_set_write_addr(dma_chan, mic_buf, false);
  dma_channel_set_trans_count(dma_chan, AUDIO_BUFFER_SIZE, true);
}

void clap_suspend() {
  irq_set_enabled(DMA_IRQ_0, false);

  adc_run(false);

  adc_fifo_setup(false, false, 0, false, false);

  adc_fifo_drain();
}

void clap_resume() {
  adc_fifo_setup(true, true, 1, false, false);
  adc_select_input(CLAP_ADC_CHANNEL);

  adc_run(true);

  while (!adc_hw->cs & ADC_CS_READY_BITS)
    ;

  dma_channel_start(dma_chan);
  
  irq_clear(DMA_IRQ_0);
  irq_set_enabled(DMA_IRQ_0, true);
}

bool clap_detect(uint16_t *audio_buffer, size_t start, size_t end) {
  const size_t WINDOW_SIZE = 240;
  const float RISE_RATIO_THRESHOLD = 5.0f;
  const size_t STEP = 60;
  const uint COOLDOWN_TICKS = pdMS_TO_TICKS(200);

  static TickType_t last_clap_tick = 0;

  TickType_t now = xTaskGetTickCount();
  if (now - last_clap_tick < COOLDOWN_TICKS)
    return false;

  start %= AUDIO_BUFFER_SIZE;
  end %= AUDIO_BUFFER_SIZE;

  size_t buffer_length = (end + AUDIO_BUFFER_SIZE - start) % AUDIO_BUFFER_SIZE;

  float prev_energy = 0;
  size_t i = start;

  int delay_counter = 0;

  for (size_t count = 0; count + 2 * WINDOW_SIZE <= buffer_length; count += STEP) {
    float energy = 0;
    for (size_t j = 0; j < WINDOW_SIZE; j++) {
      size_t idx = (i + j) % AUDIO_BUFFER_SIZE;
      float sample = (float)audio_buffer[idx];
      energy += sample * sample;
    }

    float energy_ratio = energy / prev_energy;
    if (prev_energy > 0 && energy_ratio > RISE_RATIO_THRESHOLD) {
      last_clap_tick = now;
      return true;
    }

    prev_energy = energy;
    i = (i + STEP) % AUDIO_BUFFER_SIZE;
  }

  return false;
}

int clap_init_microphone(uint16_t *adc_buf) {
  mic_buf = adc_buf;

  // ADC setup
  adc_init();
  adc_gpio_init(MIC_PIN);
  adc_select_input(CLAP_ADC_CHANNEL);
  adc_fifo_setup(true, true, 1, false, false);
  adc_run(true);

  // DMA setup
  dma_chan = dma_claim_unused_channel(true);
  dma_channel_config cfg = dma_channel_get_default_config(dma_chan);
  channel_config_set_transfer_data_size(&cfg, DMA_SIZE_16);
  channel_config_set_read_increment(&cfg, false);
  channel_config_set_write_increment(&cfg, true);
  channel_config_set_dreq(&cfg, DREQ_ADC);

  dma_channel_configure(dma_chan, &cfg, mic_buf, &adc_hw->fifo, AUDIO_BUFFER_SIZE, true);

  dma_channel_set_irq0_enabled(dma_chan, true);
  irq_set_exclusive_handler(DMA_IRQ_0, dma_handler);
  irq_set_enabled(DMA_IRQ_0, true);

  dma_channel_start(dma_chan);

  struct repeating_timer timer;
  add_repeating_timer_us(-1000000 / SAMPLE_RATE, repeating_timer_callback, NULL, &timer);

  return dma_chan;
}