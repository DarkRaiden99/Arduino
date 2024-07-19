/*--------------------------------------------------------------
  Program:      Test Meter PZEM004 and Blynk App 
  Description:  Communication test meter PZEM004 and platform IoT Blynk, visualization
                Prueba de comunicacion medidor PZEM004 y plataforma IoT Blynk, 
  Hardware:     ESP8266 12E NodeMCU Lolin . PZEM-004
    Software:     Arduino IDE v1.8.15 , Blynk App
    Date:         18 DEC 2022
    Created:       hrbembedded
    library       https://github.com/blynkkk/blynk-library/blob/master/src/BlynkSimpleEsp8266.h
                  https://github.com/mandulaj/PZEM-004T-v30
 Complte tutorial 
Examples blynk virtual data :   https://www.geekering.com/categories/embedded-sytems/esp8266/ricardocarreira/esp8266-nodemcu-simple-energy-meter-using-pzem004t/
SoftwareSerial for esp8266                          https://github.com/plerup/espsoftwareserial
TX  TO RX
RX  TO TX
Vcc   3.3V
Gnd
I2C LCD CONNECTION  
SDA   D2
SCL   D1
vcc   Vin
Gnd    Gnd
 --------------------------------------------------------------*/



/* Comment this out to disable prints and save space */

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <PZEM004Tv30.h>//https://github.com/mandulaj/PZEM-004T-v30
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
PZEM004Tv30 pzem(&Serial);

#define BLYNK_TEMPLATE_ID "TMPLEuJoFqDx"
#define BLYNK_TEMPLATE_NAME "ENERGY METER AUTOMATION"
#define BLYNK_AUTH_TOKEN "BBdCNbuV6KuPl7i7aRC72t3TLkjkdjcO"
char auth[] = BLYNK_AUTH_TOKEN;
char ssid [] = "Bad Signal";
char pass [] = "kavi5780";

float voltage=0;
float current=0;
float power=0;
float energy=0;
float frequency=0;
float pf=0;
unsigned long lastMillis = 0;
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup()
{
 lcd.begin();   // initializing the LCD
  lcd.backlight(); // Enable or Turn On the backlight 
 lcd.setCursor(0,0);  //Set cursor to first column of second row
 lcd.print("    Energy meter    ");
 delay(1000);
lcd.setCursor(0,1);  //Set cursor to first column of second row
lcd.print("   Monitor by IOT   ");
delay(1000);
lcd.setCursor(0,2);  //Set cursor to first column of second row
lcd.print("     Designed by   ");
delay(1000);
lcd.setCursor(0,3);  //Set cursor to first column of second row
lcd.print("HRB Embedded & Craft");
delay(3000);
lcd.clear();
lcd.print("Device connecting..."); 
delay(2000);
lcd.setCursor(0,2);  //Set cursor to first column of second row
lcd.print(".......please  wait");

Serial.begin(9600);
Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
lcd.clear();
}
void loop()
{
  
  Blynk.run();
  
    float voltage = pzem.voltage();
    if( !isnan(voltage) ){
        Serial.print("Voltage: "); Serial.print(voltage,1); Serial.println("V");
        lcd.setCursor(0,0); lcd.print("Voltage:      Volts"); lcd.setCursor(8,0); lcd.print(voltage,1);
    } else {
        Serial.println("Error reading voltage");
        lcd.clear();
        lcd.setCursor(0,0); lcd.print("Voltage = 0.00Volts "); 
        lcd.setCursor(0,1); lcd.print("Current = 0 Amps  "); 
        lcd.setCursor(0,3); lcd.print("  No Power  Supply  ");
    }

    float current = pzem.current();
    if( !isnan(current) ){
        Serial.print("Current: "); Serial.print(current,3); Serial.println("A");
        lcd.setCursor(0,1); lcd.print("Current:      Amps"); lcd.setCursor(8,1); lcd.print(current,3);
    } else {
        Serial.println("Error reading current");
        
    }

    float power = pzem.power();
    if( !isnan(power) ){
        Serial.print("Power: "); Serial.print(power,0); Serial.println("W");
        lcd.setCursor(0,2); lcd.print("     Watts"); lcd.setCursor(0,2); lcd.print(power,0);
    } else {
        Serial.println("Error reading power");
            }

    float energy = pzem.energy();
    if( !isnan(energy) ){
        Serial.print("Energy: "); Serial.print(energy, 3); Serial.println("kWh");
        lcd.setCursor(12,2); lcd.print("     Kwh"); lcd.setCursor(12,2); lcd.print(energy,3);
    } else {
        Serial.println("Error reading energy");
    }

    float frequency = pzem.frequency();
    if( !isnan(frequency) ){
        Serial.print("Frequency: "); Serial.print(frequency,1); Serial.println("Hz");
        lcd.setCursor(0,3); lcd.print("Freq:    Hz"); lcd.setCursor(5,3); lcd.print(frequency,1);
    } else {
        Serial.println("Error reading frequency");
    }

    float pf = pzem.pf();
    if( !isnan(pf) ){
        Serial.print("PF: "); Serial.println(pf);
        lcd.setCursor(12,3); lcd.print("PF: "); lcd.setCursor(15,3); lcd.print(pf);
    } else {
        Serial.println("Error reading power factor");
        
    }

    Serial.println();
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
