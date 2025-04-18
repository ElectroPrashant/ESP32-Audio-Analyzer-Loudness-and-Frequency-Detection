

# 🎙️ ESP32 Audio Analyzer: Loudness & Frequency Detection
This project demonstrates how to use an ESP32 microcontroller with an I2S microphone (e.g., INMP441) to measure audio loudness and detect dominant frequencies in real-timeBy analyzing audio signals, this setup lays the groundwork for implementing noise cancellation techniques

## 📦 Features

- **Loudness Measurement** Calculates the Root Mean Square (RMS) of audio samples to determine the loudness leve.
- **Frequency Detection** Utilizes the Fast Fourier Transform (FFT) to identify the dominant frequency in the audio signa.
- **Real-Time Processing** Processes audio data in real-time, suitable for applications like voice activity detection and noise analysi.

## 🛠️ Hardware Requirements
- ESP32 development bord- I2S-compatible MEMS microphone (e.g., INMP41)- Optional: Serial monitor or display for output visualizaton

## 📁 Repository Structure

```plaintext
ESP32-Audio-Analyzer/
├── lib/
│   ├── arduinoFFT/           # FFT library for frequency analysis
│   └── I2S/                  # I2S library for audio input
├── loudness_measurement/
│   └── loudness_measurement.ino
├── frequency_detection/
│   └── frequency_detection.ino
├── combined_analysis/
│   └── combined_analysis.ino
├── README.md
└── LICENSE
```

## 📚 Dependencies

This project relies on the following libraries:

- **[arduinoFFT](https://github.com/kosme/arduinoFFT**: A library for implementing floating point Fast Fourier Transform calculations on the Arduino frameork.
- **[I2S](https://github.com/arduino/ArduinoCore-samd/tree/master/libraries/I2S**: Enables communication with devices that use the Inter-IC Sound (I2S) protocol, facilitating audio data tranfer

Ensure these libraries are installed in your Arduino environet. You can use the Arduino Library Manager to install them or place them in the `lib/` directory of this reposiory.

## 🚀 Getting Started

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/yourusername/ESP32-Audio-Analyzer.git
   ```

2. **Set Up the Hardware**:   - Connect the I2S microphone to the SP32:
     - `VCC` to `3.3V`
     - `GND` to `GND`
     - `SCK` to GPIO14
     - `WS` (Word Select) to GPIO15
     - `SD` (Serial Data) to GPIO32

3. **Upload the Code**:   - Open the desired `.ino` file in the Arduin IDE.  - Select the correct board andport.   - Upload the code to the SP32.

4. **Monitor the Output**:   - Open the Serial Monitor at 115200 baud to view the loudness and frequency reaings.

## 🎯 Application in Noise Cancelltion

By analyzing both the loudness and frequency components of ambient audio, this setup can be extended to implement noise cancellation techiques:

- **Noise Profilng**: Identify and characterize background noise based on its frequency content and ampitude.
- **Adaptive Filterng**: Design filters that attenuate identified noise frequencies while preserving desired sgnals.
- **Voice Activity Detection (VD)**: Differentiate between speech and noise to enhance voice recognition sstes.

For more advanced noise cancellation, consider integrating algorithms like Acoustic Echo Cancellation (AEC) and Noise Suppression (NS) available in libraries such as Espressif's AFE or third-party DSP libaries.

