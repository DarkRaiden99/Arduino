
#include <ESP8266WiFi.h>
#include <PZEM004Tv30.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//Define Buttons
#define BTN D5  //Main Button
#define RST D6  //Reset Button

//Define Relay
#define RLY D7 //Main Relay

//OLED Display Settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

PZEM004Tv30 pzem(&Serial); //PZEM Connection Pins

//Bitmaps
// boot, 128x64
const unsigned char boot [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x30, 0x00, 0x0f, 0x8f, 0xc7, 0xfe, 0x0f, 0xf0, 0x30, 0x3e, 0xfe, 0xff, 0x80, 
	0x00, 0x00, 0x00, 0x30, 0x00, 0x06, 0x1c, 0xe4, 0x62, 0x03, 0x70, 0x30, 0x62, 0x62, 0x73, 0xc0, 
	0x00, 0x00, 0x00, 0x70, 0x00, 0x06, 0x30, 0x30, 0x60, 0x03, 0x30, 0x38, 0x60, 0x60, 0x60, 0xe0, 
	0x00, 0x00, 0x00, 0xe0, 0x00, 0x06, 0x70, 0x38, 0x60, 0x03, 0x30, 0x58, 0x70, 0x62, 0x60, 0x60, 
	0x00, 0x00, 0x01, 0xe0, 0x00, 0x06, 0x60, 0x38, 0x60, 0x03, 0xe0, 0x4c, 0x3c, 0x7e, 0x60, 0x60, 
	0x00, 0x00, 0x01, 0xc0, 0x00, 0x06, 0x60, 0x38, 0x60, 0x03, 0x78, 0x8c, 0x1e, 0x62, 0x60, 0x60, 
	0x00, 0x00, 0x03, 0xc0, 0x00, 0x06, 0x70, 0x38, 0x60, 0x03, 0x18, 0xfe, 0x06, 0x60, 0x60, 0x60, 
	0x00, 0x00, 0x07, 0xc0, 0x00, 0x06, 0x30, 0x30, 0x60, 0x03, 0x18, 0x86, 0x06, 0x60, 0x60, 0xe0, 
	0x00, 0x00, 0x0f, 0x80, 0x00, 0x06, 0x1c, 0xe0, 0x60, 0x03, 0x39, 0x07, 0x46, 0x62, 0x71, 0xc0, 
	0x00, 0x00, 0x1f, 0x80, 0x00, 0x0f, 0x8f, 0xc1, 0xf8, 0x0f, 0xf3, 0xcf, 0xfc, 0xff, 0xff, 0x80, 
	0x00, 0x00, 0x1f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xf0, 0xfc, 0x7e, 0x47, 0xff, 0xff, 0x00, 
	0x00, 0x00, 0xff, 0x81, 0x80, 0x00, 0x00, 0x00, 0x03, 0x19, 0xc7, 0x18, 0xc2, 0x38, 0xbf, 0x80, 
	0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x03, 0x1b, 0x83, 0x98, 0xe2, 0x30, 0x39, 0x80, 
	0x00, 0x01, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x03, 0x1b, 0x01, 0x98, 0xe2, 0x39, 0x19, 0x80, 
	0x00, 0x03, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x03, 0x1b, 0x01, 0x8d, 0x34, 0x3f, 0x1b, 0x00, 
	0x00, 0x07, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x03, 0x37, 0x01, 0x8d, 0x34, 0x39, 0x1f, 0x00, 
	0x00, 0x0f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc3, 0x01, 0x8e, 0x1c, 0x30, 0x1b, 0x80, 
	0x00, 0x0f, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x83, 0x86, 0x1c, 0x38, 0x39, 0xc0, 
	0x00, 0x00, 0x07, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x03, 0x01, 0xc7, 0x06, 0x08, 0x38, 0xb8, 0xc0, 
	0x00, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x07, 0xc0, 0xfc, 0x04, 0x08, 0x7f, 0xfc, 0x70, 
	0x00, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x0f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x0f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0x00, 
	0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x18, 0xe3, 0x18, 0xb8, 0xbf, 0x80, 
	0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x38, 0xc0, 0x18, 0x30, 0x39, 0x80, 
	0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x38, 0xe4, 0x18, 0x39, 0x19, 0x80, 
	0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x58, 0xfc, 0x18, 0x3f, 0x1b, 0x00, 
	0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2c, 0xd8, 0xe4, 0x18, 0x39, 0x1f, 0x00, 
	0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x98, 0xc0, 0x18, 0x30, 0x1b, 0x80, 
	0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x67, 0x18, 0xe0, 0x18, 0x38, 0x39, 0xc0, 
	0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x1c, 0xe2, 0x18, 0x38, 0xb8, 0xc0, 
	0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf3, 0x3f, 0xfe, 0x7e, 0x7f, 0xfc, 0x70, 
	0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
//wifi, 8x8
const unsigned char wifi[] PROGMEM  ={
  0xe0, 0x18, 0x04, 0xc2, 0x22, 0x11, 0xc9, 0xc9
};
//cross, 8x8
const unsigned char cross[] PROGMEM  ={
  0x00, 0x42, 0x24, 0x10, 0x08, 0x24, 0x42, 0x00
};
//ok, 8x8
/*const unsigned char ok[] PROGMEM  ={
  0x00, 0x01, 0x02, 0x04, 0x04, 0x48, 0x28, 0x10
};*/

//Variables
float voltage=0;
float current=0;
float power=0;
float energy=0;
float frequency=0;
float pf=0;

unsigned long meterMillis = 0;  //Meter timer
unsigned long dispMillis = 0;   //Display Timeout Timer
unsigned long disprMillis = 0;   //Display Refresh Timer
unsigned long resetMillis = 0;   //Reset Countdown

//Triggers and Conditions
bool meter=0;     //PZEM Connection
bool wifistat=0;  //Wifi Connection
bool dstat=1;     //Display Status
bool dswitch=0;   //Display Switch
bool dchange=0;   //Display Last State
bool rswitch=0;   //Reset Switch
bool rchange=0;   //Reset Last State
//int dtimer=0;     //Display Timer
int dpage=0;       //Display Pages

void setup() {
  Serial.begin(9600);

  pinMode(BTN, INPUT);
  pinMode(RST, INPUT);
  pinMode(RLY, OUTPUT);
  
  oledboot(); //OLED Display initialization

  bootscreen(); //Bootscreen
  delay(2000);

}

void loop() {
  dswitch = digitalRead(BTN);
  rswitch = digitalRead(RST);

  resetbutton();

  mainbutton();

  meterreading();

  disp();

  delay(100);
}

void disp() {
  if (millis() - disprMillis > 2000) {
    disprMillis = millis();
    if (dstat == 1){
      display.clearDisplay();

      //Screen Top Bar
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0,0);
      display.print("Power Meter");
      display.drawBitmap(120,0,cross,8,8,WHITE);
      display.println();
      display.println();
      display.display();

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
          if(dpage == 2){
            dpage = 0;
          } else {
            ++dpage;
          }
        }
        switch (dpage) {
          case 0:
            disp1();
            break;
          case 1:
            disp2();
            break;
          case 2:
            disp3();
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
  display.print(energy,2);
  display.println(" kWh");
  display.display();
}

void disp3 () {
  display.setTextSize(1);
  display.print("Voltage:   "); display.print(voltage,2); display.println(" V");
  display.print("Current:   "); display.print(current,3); display.println(" A");
  display.print("Power:     "); display.print(power,2); display.println(" W");
  display.print("Usage:     "); display.print(energy,3); display.println(" kWh");
  display.print("Freq.:     "); display.print(frequency,1); display.println(" Hz");
  display.print("PF:        "); display.print(pf,2);
  display.display();
}

void mainbutton() {
    if( dswitch == 1 && dchange == 0 && rchange == 0) {
      if(dstat == 1) {
        dchange = 1;
      } else {
        dstat = 1;
        dchange = 0;
        delay(500);
      }
      dispMillis = millis();
  } else if(millis() - dispMillis > 30000) {
    dstat = 0;
  }
}

void resetbutton() {
  if( rswitch == 1 && dswitch == 1) {
    if(rchange == 0) {
      rchange = 1;
      dstat = 1;
      resetMillis = millis();
      dispMillis = millis();
    } else if(millis() - resetMillis > 10000) {
      pzem.resetEnergy();
    }
  } else {
    rchange = 0;
  }
}

void meterreading() {
  if (millis() - meterMillis > 2000) {
    meterMillis = millis();
    voltage = pzem.voltage();
    current = pzem.current();
    power = pzem.power();
    energy = pzem.energy();
    frequency = pzem.frequency();
    pf = pzem.pf();

    if( !isnan(voltage) ){
        meter = 1;
    } else {
        meter = 0;
        voltage = 0;
        current = 0;
        power = 0;
        energy = 0;
        frequency = 0;
        pf = 0;
    }
  }
}

void bootscreen() {
  display.clearDisplay();
  display.drawBitmap(0,0,boot,128,64,WHITE);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(30,56); display.print("Initializing");
  display.display();
}

void oledboot() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.display();
  delay(10);
  display.clearDisplay();
}