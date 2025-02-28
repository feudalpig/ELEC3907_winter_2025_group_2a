#include <arduinoFFT.h>
#include <math.h>
const uint16_t samples = 128; //This value MUST ALWAYS be a power of 2
//const float signalFrequency = 1000;
const float samplingFrequency = 1000;
int samplingPeriod;
const uint8_t amplitude = 100;

float vReal[samples];
float vImag[samples];

float noteFreq;
float noteConvert;
int octave;
int note;


ArduinoFFT<float> FFT = ArduinoFFT<float>(vReal, vImag, samples, samplingFrequency); /* Create FFT object */

void setup(){
    samplingPeriod = 1000000 / samplingFrequency - 125;
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
    noteFreq = FFT.majorPeak();
    noteConvert = (12*log10(noteFreq/27.5)/log10(2))+1;
    Serial.println(noteConvert);
    octave = (round(noteConvert) - 1)/12;
    note = round(noteConvert - 12*octave);
    Serial.println(note);
    if(note > 2) {
      octave++;  }
    Serial.println(octave);

    Serial.println(FFT.majorPeak());
    // Rest of the code
}

//test
