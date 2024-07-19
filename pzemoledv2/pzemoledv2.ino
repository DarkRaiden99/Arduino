
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


//OLED Display Settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

PZEM004Tv30 pzem(&Serial); //PZEM Connection Pins

//Icons
/*
const unsigned char intro[] PROGMEM = {
 B00000000, B00000000,
 B00000000, B01000000,
 B00000000, B10000000,
 B00000001, B10000000,
 B00000011, B00000000,
 B00000111, B00000000,
 B00001110, B00000000,
 B00011111, B11111100,
 B00111111, B11111000,
 B00000000, B01110000,
 B00000000, B11100000,
 B00000000, B11000000,
 B00000001, B10000000,
 B00000001, B00000000,
 B00000010, B00000000,
 B00000000, B00000000,
}; */
const unsigned char wifi[] PROGMEM  ={
  0xe0, 0x18, 0x04, 0xc2, 0x22, 0x11, 0xc9, 0xc9
};
const unsigned char cross[] PROGMEM  ={
  0x00, 0x42, 0x24, 0x10, 0x08, 0x24, 0x42, 0x00
};
const unsigned char ok[] PROGMEM  ={
  0x00, 0x01, 0x02, 0x04, 0x04, 0x48, 0x28, 0x10
};

//Variables
float voltage=0;
float current=0;
float power=0;
float energy=0;
float frequency=0;
float pf=0;
unsigned long lastMillis = 0;

//Triggers
int meter=0; //PZEM Connection
int wifistat=0; //Wifi Connection
int dstat=0; //OLED Switch

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  
  //OLED Display Inintialization
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  //OLED GFX Library Initialization
  display.display();
  delay(10);
  display.clearDisplay();

  //Boot Screen
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  //display.drawBitmap(0,18,intro,16,16,WHITE);
  display.setCursor(48,18); display.println("IoT Based");
  display.setCursor(48,28); display.println("Power Meter");
  display.setTextSize(1);
  display.setCursor(34,56); display.println("By Group 10");
  display.display();
  delay(3000);
  display.clearDisplay();
}

void loop() {

    Blynk.run();

    float voltage = pzem.voltage();
    float current = pzem.current();
    float power = pzem.power();
    float energy = pzem.energy();
    float frequency = pzem.frequency();
    float pf = pzem.pf();

    //Checking PZEM Connection
    if( !isnan(voltage) ){
        meter = 1;
    } else {
        meter = 0;
    }

    display.clearDisplay();

    //Screen Top Bar
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(2,0);
    display.print("Power Meter");
    display.drawBitmap(108,0,ok,8,8,WHITE);
    display.drawBitmap(120,0,wifi,8,8,WHITE);
    display.println();
    display.println();
    display.display();


    if(meter == 0){
        display.setTextSize(2);
        display.setCursor(38,16); display.println("Error");
        display.setCursor(27,32); display.println("Reading");
        display.setTextSize(1); display.setCursor(22,56); display.print("No Power Supply");
        display.display();
    } else {
        display.setTextSize(1);
        display.print("Voltage:   "); display.print(voltage,2); display.println(" V");
        display.print("Current:   "); display.print(current,3); display.println(" A");
        display.print("Power:     "); display.print(power,2); display.println(" W");
        display.print("Usage:     "); display.print(energy,3); display.println(" kWh");
        display.print("Freq.:     "); display.print(frequency,1); display.println(" Hz");
        display.print("PF:        "); display.print(pf,2);
        display.display();
    }

    delay(2000);
   
    delay(1000);

      //Publish data every 5 seconds (5000 milliseconds). Change this value to publish at a different interval.
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