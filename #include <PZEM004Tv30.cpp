#include <PZEM004Tv30.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

PZEM004Tv30 pzem(&Serial);

float voltage=0;
float current=0;
float power=0;
float energy=0;
float frequency=0;
float pf=0;
unsigned long lastMillis = 0;

void setup() {
  Serial.begin(9600);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.display();
  delay(10);
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10,0); display.println("IoT Based");
  display.setCursor(32,16); display.println("Power");
  display.setCursor(30,32); display.println("Meter");
  display.setTextSize(1);
  display.setCursor(38,50); display.println("Group 10");
  display.display();
  delay(3000);
  display.clearDisplay();
}

void loop() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.println("IoT Based Power Meter");
    display.println();
    display.display();

    float voltage = pzem.voltage();
    if( !isnan(voltage) ){
        Serial.print("Voltage: "); Serial.print(voltage); Serial.println("V");
        display.print("Voltage:   "); display.print(voltage,1); display.println(" V");
        display.display();

    } else {
        Serial.println("Error reading voltage");
        display.setTextSize(2);
        display.setCursor(38,16); display.println("Error");
        display.setCursor(27,32); display.println("Reading");
        display.setTextSize(1); display.setCursor(29,48); display.print("No Power Supply");
        display.display();
    }

    float current = pzem.current();
    if( !isnan(current) ){
        Serial.print("Current: "); Serial.print(current); Serial.println("A");
        display.print("Current:   "); display.print(current,3); display.println(" A");
        display.display();
    } else {
        Serial.println("Error reading current");
    }

    float power = pzem.power();
    if( !isnan(power) ){
        Serial.print("Power: "); Serial.print(power); Serial.println("W");
        display.print("Power:     "); display.print(power,1); display.println(" W");
        display.display();
    } else {
        Serial.println("Error reading power");
    }

    float energy = pzem.energy();
    if( !isnan(energy) ){
        Serial.print("Energy: "); Serial.print(energy,3); Serial.println("kWh");
        display.print("Usage:     "); display.print(energy,2); display.println(" kWh");
        display.display();
    } else {
        Serial.println("Error reading energy");
    }

    float frequency = pzem.frequency();
    if( !isnan(frequency) ){
        Serial.print("Frequency: "); Serial.print(frequency, 1); Serial.println("Hz");
        display.print("Freq.:     "); display.print(frequency,1); display.println(" Hz");
        display.display();
    } else {
        Serial.println("Error reading frequency");
    }

    float pf = pzem.pf();
    if( !isnan(pf) ){
        Serial.print("PF: "); Serial.println(pf);
        display.print("PF:        "); display.print(pf,2);
        display.display();
    } else {
        Serial.println("Error reading power factor");
    }

    Serial.println();
    delay(2000);
}