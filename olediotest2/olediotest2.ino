#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define BTN D5  //Main Button
#define RST D6  //Reset Button

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET     -1 
#define SCREEN_ADDRESS 0x3C 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

float voltage = 230;
float current = 1;
float power = 200;
float energy = 10;
float frequency = 50;
float pf = 0.96;

float daily = 2;
float hourly = 0.2;

unsigned long dispMillis = 0;   //Display Timeout Timer
unsigned long disprMillis = 0;  //Display Refresh Timer

bool meter = 0;     //PZEM Connection
bool wifistat = 0;  //Wifi Connection
bool dstat = 1;     //Display Status
bool dswitch = 0;   //Display Switch
bool dchange = 0;   //Display Last State
bool rswitch = 0;   //Reset Switch
bool rchange = 0;   //Reset Last State
bool sswitch = 0;   //Relay Trigger
bool schange = 0;   //Relay Last State

int pageCount = 4;  //Page Count
int dpage = 0;          //Display Page Memory

int screenrefresh = 1000;  //Screen Refresh Time

void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)
  display.clearDisplay();
}

void loop() {
  dswitch = digitalRead(BTN);
  rswitch = digitalRead(RST);

  disp();
  
  delay(100);
}

void disp() {
  if (millis() - disprMillis > screenrefresh || dchange == 1) {
    disptop();

  }
}


void dispo() {
  if (millis() - disprMillis > screenrefresh) {
    disprMillis = millis();
    if (dstat == 1){
      display.clearDisplay();

      disptop();
      
      if(meter == 0){
        display.setTextSize(2);
        display.setCursor(38,16); display.println("Error");
        display.setCursor(27,32); display.println("Reading");
        display.setTextSize(1); display.setCursor(22,56); display.print("No Power Supply");
        display.display();

        dchange = 0;
      } else {
        if (dchange == 1) {
          dchange = 0;
          if(dpage == pageCount){
            dpage = 1;
          } else {
            ++dpage;
          }
        }
        switch (dpage) {
          case 1:
            disp3();
            break;
          case 2:
            disp1();
            break;
          case 3:
            disp2();
            break;
          case 4:
            disp4();
            break;
        }
      }
    } else {
      display.clearDisplay();
      display.setCursor(0,0);
      display.print(" ");
      display.display();
    }
  }
}

void disp1 () {
  display.println("");
  display.setTextSize(2);
  display.print(voltage,1);
  display.println(" V");

  display.setTextSize(1);
  display.println("");

  display.setTextSize(2);
  display.print(current,3);
  display.println(" A");
  display.display();
}

void disp2 () {
  display.println("");
  display.setTextSize(2);
  display.print(power,1);
  display.println(" W");

  display.setTextSize(1);
  display.println("");

  display.setTextSize(2);
  display.print(energy,3);
  display.println(" kWh");
  display.display();
}

void disp3 () {
  display.setTextSize(1);
  display.println("");
  display.print("Voltage:   "); display.print(voltage,2); display.println(" V");
  display.print("Current:   "); display.print(current,3); display.println(" A");
  display.print("Power:     "); display.print(power,2); display.println(" W");
  display.print("Usage:     "); display.print(energy,3); display.println(" kWh");
  display.print("Freq.:     "); display.print(frequency,1); display.println(" Hz");
  display.print("PF:        "); display.print(pf,2);
  display.display();
}

void disp4 () {
  display.setTextSize(1);
  display.println();
  display.println("Energy Consumption");
  display.println();

  display.print("Hourly:  ");
  display.print(hourly,3);
  display.println(" kwh");

  display.print("Daily:   ");
  display.print(daily,3);
  display.println(" kWh");

  display.println();
  display.print("Total:   ");
  display.print(energy,3);
  display.println(" kWh");
  display.display();
}

void disptop () {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.print("Power Meter");
  display.drawBitmap(120,0,cross,8,8,WHITE);
  display.println();
  display.display();
}