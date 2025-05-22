#include "microphone.h"

#include "hardware/adc.h"
#include "hardware/dma.h"
#include "hardware/timer.h"
#include "pico/stdlib.h"

int dma_chan;
uint16_t* mic_buf;

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

int init_microphone(uint16_t* adc_buf) {
  mic_buf = adc_buf;

  // ADC setup
  adc_init();
  adc_gpio_init(26);
  adc_select_input(ADC_CHANNEL);
  adc_fifo_setup(true, true, 1, false, false);
  adc_run(true);

  // DMA setup
  dma_chan = dma_claim_unused_channel(true);
  dma_channel_config cfg = dma_channel_get_default_config(dma_chan);
  channel_config_set_transfer_data_size(&cfg, DMA_SIZE_16);
  channel_config_set_read_increment(&cfg, false);
  channel_config_set_write_increment(&cfg, true);
  channel_config_set_dreq(&cfg, DREQ_ADC);

  dma_channel_configure(
    dma_chan,
    &cfg,
    mic_buf,
    &adc_hw->fifo,
    AUDIO_BUFFER_SIZE,
    true
  );

  dma_channel_set_irq0_enabled(dma_chan, true);
  irq_set_exclusive_handler(DMA_IRQ_0, dma_handler);
  irq_set_enabled(DMA_IRQ_0, true);

  dma_channel_start(dma_chan);

  struct repeating_timer timer;
  add_repeating_timer_us(-1000000 / SAMPLE_RATE, repeating_timer_callback, NULL, &timer);

  return dma_chan;
}