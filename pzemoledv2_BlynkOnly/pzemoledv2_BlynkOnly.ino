
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6PHEpRman"
#define BLYNK_TEMPLATE_NAME "PowerMeter"
#define BLYNK_AUTH_TOKEN "udE4Za_B1ZKxEhtOUYryvN8dzXeVsrus"


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <PZEM004Tv30.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


char auth[] = BLYNK_AUTH_TOKEN;
char ssid [] = "SLT_FIBRE";
char pass [] = "rusith11";

PZEM004Tv30 pzem(&Serial); //PZEM Connection Pins

//Variables
float voltage=0;
float current=0;
float power=0;
float energy=0;
float frequency=0;
float pf=0;
unsigned long lastMillis = 0;
unsigned long meterMillis = 0;

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
}

void loop() {

    Blynk.run();

    meterreading();

    blynkupdate();

    delay(100);
}

void meterreading() {
  if (millis() - meterMillis > 2000) {
    meterMillis = millis();
    voltage = pzem.voltage();

    if( !isnan(voltage) ){
      current = pzem.current();
      power = pzem.power();
      energy = pzem.energy();
      frequency = pzem.frequency();
      pf = pzem.pf();
      if(power < 1){
        power = 0;
      }
    } else {
      voltage = 0;
      current = 0;
      power = 0;
      frequency = 0;
      pf = 0;
    }
  }
}

void blynkupdate() {
  if (millis() - lastMillis > 5000) {
      lastMillis = millis();
            
      Blynk.virtualWrite(V1, voltage);
      Blynk.virtualWrite(V2, current);            
      Blynk.virtualWrite(V3, power);
      Blynk.virtualWrite(V4, energy);
      Blynk.virtualWrite(V5, frequency);
      Blynk.virtualWrite(V6, pf);            
    }  
}