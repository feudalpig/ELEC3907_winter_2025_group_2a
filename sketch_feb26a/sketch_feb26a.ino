#include <arduinoFFT.h>
const uint16_t samples = 128; //This value MUST ALWAYS be a power of 2
//const float signalFrequency = 1000;
const float samplingFrequency = 1000;
int samplingPeriod;
const uint8_t amplitude = 100;

float vReal[samples];
float vImag[samples];

ArduinoFFT<float> FFT = ArduinoFFT<float>(vReal, vImag, samples, samplingFrequency); /* Create FFT object */

void setup(){
    samplingPeriod = 1000000 / samplingFrequency;
    Serial.begin(9600);
}

void loop() {
    // Get samples
    for (int i = 0; i < samples; i++)
    {
      vImag[i] = 0;
      vReal[i] = analogRead(A0);
      delayMicroseconds(samplingPeriod);
    }

    FFT.dcRemoval(vReal, samples);
    FFT.windowing(FFTWindow::Hamming, FFTDirection::Forward);	/* Weigh data */
    FFT.compute(FFTDirection::Forward); /* Compute FFT */
    FFT.complexToMagnitude(); /* Compute magnitudes */
    Serial.println(FFT.majorPeak());
    
    // Rest of the code
}
