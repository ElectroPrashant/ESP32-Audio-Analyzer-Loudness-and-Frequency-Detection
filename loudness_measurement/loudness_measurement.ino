#include <driver/i2s.h>


#define I2S_WS      15   // LRCL
#define I2S_SD      36   // DOUT from mic
#define I2S_SCK     14   // BCLK

#define I2S_PORT    I2S_NUM_0

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("I2S Mic Loudness Plotter Starting...");

  const i2s_config_t i2s_config = {
    .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = 16000,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = 256,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0
  };

  const i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_SD
  };

  i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_PORT, &pin_config);
  i2s_zero_dma_buffer(I2S_PORT);
}

void loop() {
  const int samples = 1024;
  int32_t buffer[samples];
  size_t bytesRead;

  i2s_read(I2S_PORT, &buffer, sizeof(buffer), &bytesRead, portMAX_DELAY);

  int32_t minVal = INT32_MAX;
  int32_t maxVal = INT32_MIN;

  int sampleCount = bytesRead / sizeof(int32_t);

  for (int i = 0; i < sampleCount; i++) {
    int32_t val = buffer[i] >> 14; // scale down to 12-bit like ADC
    if (val < minVal) minVal = val;
    if (val > maxVal) maxVal = val;
  }

  int32_t peakToPeak = maxVal - minVal;

  Serial.println(peakToPeak);  // Plot this in Serial Plotter
  delay(20); // balance between smooth plotting and watchdog safety
}