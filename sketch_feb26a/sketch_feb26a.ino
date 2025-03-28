#include <arduinoFFT.h>
#include <math.h>
#include <TimerOne.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Constants
const uint16_t samples = 128;
const float samplingFrequency = 1000;
int samplingPeriod;
const uint8_t amplitude = 100;

// FFT Variables
float vReal[samples];
float vImag[samples];

// Note Detection Variables
float magnitude;
float noteFreq;
float noteConvert;
int octave;
int note;
int cents;
char *noteline;
char *accuracyline;

// I2C LCD Display
LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the LCD address to 0x27 for a 16 chars and 2 line display

// FFT Object
ArduinoFFT<float> FFT = ArduinoFFT<float>(vReal, vImag, samples, samplingFrequency);

void setup() {
    samplingPeriod = 1000000 / samplingFrequency - 125;
    Serial.begin(9600);

    // Initialize the LCD
    lcd.init();
    lcd.backlight(); // Turn on the backlight
    lcd.setCursor(0, 0);
    lcd.print("Tuner");
    noteline = (char*)malloc(sizeof(char)*16);
    noteline = (char*)malloc(sizeof(char)*16);
    pinMode(9,OUTPUT);
    Timer1.initialize(100);  //100us = 10khz
    Timer1.pwm(9,205);

    pinMode(5,OUTPUT);
    analogWrite(5, 127);
    //Timer1.initialize(400);  //400us = 2.5khz
    //Timer1.pwm(2,205);
    noteline = "                ";
    accuracyline = "                ";
}

void loop() {
    // Get samples

    while(analogRead(A0) < 470)

    for (int i = 0; i < samples; i++)
    {
      vImag[i] = 0;
      vReal[i] = analogRead(A0);
      delayMicroseconds(samplingPeriod);
    }

    // Perform FFT
    FFT.dcRemoval(vReal, samples);
    FFT.windowing(FFTWindow::Hamming, FFTDirection::Forward);	/* Weigh data */
    FFT.compute(FFTDirection::Forward); /* Compute FFT */
    FFT.complexToMagnitude(); /* Compute magnitudes */
    FFT.majorPeak(vReal, samples, samplingFrequency, &noteFreq, &magnitude);
    //noteFreq = FFT.majorPeak();
    noteConvert = (12*log10(noteFreq/27.5)/log10(2))+1;
    Serial.println(noteConvert);
    octave = (round(noteConvert) - 1)/12;
    note = round(noteConvert - 12*octave);
    Serial.println(note);
    cents = round((noteConvert - round(noteConvert)) * 10);
    if(note > 3) {
      octave++;  }
    Serial.println(octave);

    switch(note){
      case 1:
        noteline = "      A         ";
        break;
      case 2:
        noteline = "   A#/Bb        ";
        break;
      case 3:
        noteline = "      B         ";
        break;
      case 4:
        noteline = "      C        ";
        break;
      case 5:
        noteline = "   C#/Db        ";
        break;
      case 6:
        noteline = "      D         ";
        break;
      case 7:
        noteline = "   D#/Eb        ";
        break;
      case 8:
        noteline = "      E         ";
        break;
      case 9:
        noteline = "      F         ";
        break;
      case 10:
        noteline = "   F#/Gb        ";
        break;
      case 11:
        noteline = "      G         ";
        break;
      case 12:
        noteline = "   G#/Ab        ";
        break;
    }
    noteline[9] = '0' + octave;

    switch(cents){
      case -5:
        accuracyline = " ||||| ==       ";
        break;
      case -4:
        accuracyline = "  |||| ==       ";
        break;
      case -3:
        accuracyline = "   ||| ==       ";
        break;
      case -2:
        accuracyline = "    || ==       ";
        break;
      case -1:
        accuracyline = "     | ==       ";
        break;
      case 0:
        accuracyline = "       ==       ";
        break;
      case 1:
        accuracyline = "       == |     ";
        break;
      case 2:
        accuracyline = "       == ||    ";
        break;
      case 3:
        accuracyline = "       == |||   ";
        break;
      case 4:
        accuracyline = "       == ||||  ";
        break;
      case 5:
        accuracyline = "       == ||||| ";
        break;
    }

  if (magnitude < 300) {
    noteline = "    N/A         ";
    noteline[9] = ' ';
    accuracyline = "       ==       ";
  }

    Serial.println(cents);
}

    // Determine note name
    switch (note) {
        case 1: noteline = "A"; break;
        case 2: noteline = "A#/Bb"; break;
        case 3: noteline = "B"; break;
        case 4: noteline = "C"; break;
        case 5: noteline = "C#/Db"; break;
        case 6: noteline = "D"; break;
        case 7: noteline = "D#/Eb"; break;
        case 8: noteline = "E"; break;
        case 9: noteline = "F"; break;
        case 10: noteline = "F#/Gb"; break;
        case 11: noteline = "G"; break;
        case 12: noteline = "G#/Ab"; break;
    }

    // Determine accuracy
    switch (cents) {
        case -5: accuracyline = "||||| =="; break;
        case -4: accuracyline = "|||| =="; break;
        case -3: accuracyline = "||| =="; break;
        case -2: accuracyline = "|| =="; break;
        case -1: accuracyline = "| =="; break;
        case 0: accuracyline = "=="; break;
        case 1: accuracyline = "== |"; break;
        case 2: accuracyline = "== ||"; break;
        case 3: accuracyline = "== |||"; break;
        case 4: accuracyline = "== ||||"; break;
        case 5: accuracyline = "== |||||"; break;
    }

    // Display on LCD
    lcd.setCursor(0, 0);
    lcd.print("Note: ");
    lcd.print(noteline);
    lcd.print(" ");
    lcd.print(octave);
    lcd.print("      "); // Clear the rest of the line with spaces

    lcd.setCursor(0, 1);
    lcd.print("Accuracy: ");
    lcd.print(accuracyline);
    lcd.print("      "); // Clear the rest of the line with spaces

    // Debugging output to Serial Monitor
    Serial.println("================");
    Serial.println(noteline);
    Serial.println(accuracyline);
    Serial.println("================");
    Serial.println(FFT.majorPeak());
    Serial.println(magnitude);
}