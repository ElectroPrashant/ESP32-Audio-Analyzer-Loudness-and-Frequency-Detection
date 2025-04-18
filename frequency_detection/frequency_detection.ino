#include <Arduino.h>
#include <driver/i2s.h>
#include <arduinoFFT.h>


#define I2S_WS      15   // LRCL
#define I2S_SD      36   // DOUT from mic
#define I2S_SCK     14   // BCLK

#define I2S_PORT    I2S_NUM_0

#define SAMPLE_RATE     16000
#define SAMPLES         1024
#define FFT_SAMPLES     SAMPLES / 2

double vReal[FFT_SAMPLES];
double vImag[FFT_SAMPLES];

ArduinoFFT<double> FFT = ArduinoFFT<double>(vReal, vImag, SAMPLES, SAMPLE_RATE);


void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("I2S Mic Frequency Analyzer Starting...");

  const i2s_config_t i2s_config = {
    .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLE_RATE,
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
  int32_t buffer[SAMPLES];
  size_t bytesRead;

  i2s_read(I2S_PORT, &buffer, sizeof(buffer), &bytesRead, portMAX_DELAY);

  for (int i = 0; i < FFT_SAMPLES; i++) {
    vReal[i] = (double)(buffer[i * 2] >> 14); // Downscale to 12-bit
    vImag[i] = 0.0;
  }

  FFT.windowing(vReal, FFT_SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.compute(vReal, vImag, FFT_SAMPLES, FFT_FORWARD);
  FFT.complexToMagnitude(vReal, vImag, FFT_SAMPLES);

  // Find the peak frequency
  double maxMagnitude = 0;
  int maxIndex = 0;
  for (int i = 1; i < FFT_SAMPLES; i++) {
    if (vReal[i] > maxMagnitude) {
      maxMagnitude = vReal[i];
      maxIndex = i;
    }
  }

  double frequency = (maxIndex * SAMPLE_RATE) / (double)SAMPLES;
  Serial.print("Dominant Frequency: ");
  Serial.print(frequency);
  Serial.println(" Hz");

  delay(100);
}
