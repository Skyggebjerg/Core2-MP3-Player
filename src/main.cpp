#include <Arduino.h>

#pragma mark - Depend ESP8266Audio and ESP8266_Spiram libraries
/*
Plays mp3 files from SPIFFS or SD (if SD, then toggle below between SPIFFS and SD)

Use the "Tools->ESP32 Sketch Data Upload" menu to write the MP3 to SPIFFS
Then upload the sketch normally.
https://github.com/me-no-dev/arduino-esp32fs-plugin
*/

#include <M5Core2.h>
#include <driver/i2s.h>
#include <WiFi.h>

#include "AudioFileSourceID3.h"
#include "AudioFileSourceSPIFFS.h" //OR: #include "AudioFileSourceSD.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"
#include "SPIFFS.h"

AudioGeneratorMP3 *mp3;
AudioFileSourceSPIFFS *file; //OR: AudioFileSourceSD *file;
AudioOutputI2S *out;
AudioFileSourceID3 *id3;

#define OUTPUT_GAIN 30

void setup() {
    M5.begin();  // Comment out: "Spk.begin();" in M5Core2.cpp !!! 
    M5.Axp.SetSpkEnable(true);
    //M5.Power.begin();
    WiFi.mode(WIFI_OFF);
    SPIFFS.begin();
    delay(500);

    M5.Lcd.setTextFont(2);
    M5.Lcd.printf("Sample MP3 playback begins...\n");
    Serial.printf("Sample MP3 playback begins...\n");

    file = new AudioFileSourceSPIFFS("/pno-cs.mp3"); //OR: file = new AudioFileSourceSD("/mp3/chikichiki.mp3");
    id3  = new AudioFileSourceID3(file);
    out  = new AudioOutputI2S(0, 0);  // Output to builtInDAC
    out->SetPinout(12, 0, 2);
    out->SetOutputModeMono(true);
    out->SetGain((float)OUTPUT_GAIN/100.0);
    mp3 = new AudioGeneratorMP3();
    mp3->begin(id3, out);
}

void loop() {
    if (mp3->isRunning()) {
        if (!mp3->loop()) mp3->stop();
    } else {
        Serial.printf("MP3 done\n");
        delay(1000);
    }
}