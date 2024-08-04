
#include <ESP8266WiFi.h>
#include <PZEM004Tv30.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//Define Buttons & Relay
#define BTN1 D5  //Left Button
#define BTN2 D6  //Right Button
#define RLY D7  //Main Relay

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

//PZEM Variables
float voltage=0;
float current=0;
float power=0;
float energy=0;
float frequency=0;
float pf=0;

//Timer Memories
unsigned long meterMillis = 0;    //Meter Timer
unsigned long dispMillis = 0;     //Display Timeout Timer
unsigned long disprMillis = 0;    //Display Refresh Timer
unsigned long resetMillis = 0;    //Reset Countdown
unsigned long relayMillis = 0;    //Relay Timer
unsigned long usageMillis = 0;    //Usage Timer
unsigned long settingMillis = 0;  //Setting Hold Timer

//Triggers and Conditions
bool meter = 0;     //PZEM Connection
bool wifistat = 0;  //Wifi Connection
bool s1 = 0;        //Left Button
bool s1last = 0;    //Left Button Last State
bool s2 = 0;        //Right Button
bool s2last = 0;    //Right Button Last State
bool dstat = 1;     //Display Status
bool dchange = 0;   //Display Force Refresh
bool dl = 0;        //Display Left Action
bool dr = 0;        //Display Right Action
bool sswitch = 0;   //Relay Trigger
bool schange = 0;   //Relay Last State
bool use = 0;       //Usage Last State
bool h2 = 0;        //Hourly Usage Last State
bool d2 = 0;        //Daily Usage Last State

//Other Memories
int dmemory [4] = {};   //Display Memory Array
float hAry [60] = {};   //Hourly Usage Memory Array
float hIn = 0;          //Hourly Initial Memory
float hourly = 0;       //Hourly Usage
float dAry [24] = {};   //Daily Usage Memory Array
float dIn = 0;          //Daily Initial Memory
float daily = 0;        //Daily Usage
int h1 = 0;             //H_Usage Array Location1
int h12 = 0;            //H_Usage Array Location2
int d1 = 0;             //D_Usage Array Location1
int d12 = 0;            //D_Usage Array Location2

//Timers and Threshholds (in Milliseconds)
int screentimeout = 30000; //Screen Timeout
int settingHold = 5000;    //Settings Hold Time
int resettimer = 10000;    //Screen Reset Timer
int relaytimer = 60000;    //Relay Reset Time
int meterrefresh = 2000;   //Meter Reading Refresh Time
int screenrefresh = 2000;  //Screen Refresh Time
int usagerefresh = 60000;  //Usage Refresh Time (2mins)

int voltHigh = 260; //Voltage High Limit
int voltLow = 200;  //Voltage Low Limit
int freqHigh = 53;  //Frequency High Limit
int freqLow = 47;   //Frequency Low Limit
int pageCount = 4;  //Page Count

void setup() {
  Serial.begin(9600);

  pinMode(BTN1, INPUT);
  pinMode(BTN2, INPUT);
  pinMode(RLY, OUTPUT);
  
  oledboot();   //OLED Display initialization

  bootscreen(); //Bootscreen
  delay(2000);

}

void loop() {
  s1 = digitalRead(BTN1);
  s2 = digitalRead(BTN2);

  button();

  meterreading();

  usagememory();

  relay();

  disp();

  delay(100);
}

void button() {
  //Button Reading
  if (s1 == 1 && s1last == 0) {
    s1last = 1;
    settingMillis = millis();
  } else if (s1 == 0 && s1last == 1) {
    s1last = 0;
    if(millis() - settingMillis < settingHold) {
      dl = 1;
    } else if (dmemory[0] != 0 && dmemory[3] == 0) {
      dl = 1;
    } else if (dmemory[3] == 1) {
      dmemory[3] = 0;
    }
  }
  if (s2 == 1 && s2last == 0) {
    s2last = 1;
  } else if (s2 == 0 && s2last == 1) {
    s2last = 0;
    dr = 1;
  }

  if(s1last == 1 && millis() - settingMillis > settingHold && dmemory[0] == 0) {
    dmemory[0] = 1;
    dmemory[2] = dmemory[1];
    dmemory[1] = 0;
    dmemory[3] = 1;
    dl = 0;
    dchange = 1;
  }

  //Button Action
  if (dmemory[0] == 0) {
    /*if(meter == 0) {
      dmemory[1] = 9;
    } else {*/
      if (dl == 1) {
        if (dmemory[1] > 0) {
          --dmemory[1];
        } else if (dmemory[1] == 0) {
          dmemory[1] = 4;
        }
        dl = 0;
        dchange = 1;
      } else if (dr == 1) {
        if (dmemory[1] < 4) {
          ++dmemory[1];
        } else if (dmemory[1] == 4) {
          dmemory[1] = 0;
        }
        dr = 0;
        dchange = 1;
      }
    //}
  }

  if (dmemory[0] == 1) {
    if (dl == 1) {
      if (dmemory[1] < 3) {
        ++dmemory[1];
      } else if (dmemory[1] == 3) {
        dmemory[1] = 0;
      }
      dl = 0;
      dchange = 1;
    }
    if (dr == 1) {
      switch (dmemory[1]) {
        case 0:
          dmemory[0] = 2;
          dmemory[1] = 0;
          break;
        case 1:
          dmemory[0] = 3;
          dmemory[1] = 0;
          break;
        case 2:
          dmemory[0] = 4;
          dmemory[1] = 0;
          break;
        case 3:
          dmemory[0] = 0;
          dmemory[1] = dmemory[2];
          break;
      }
      dr = 0;
      dchange = 1;
    }
  }

  if (dmemory[0] == 2) {
    if (dl == 1) {
      if (dmemory[1] == 0) {
        dmemory[1] = 1;
      } else {
        dmemory[1] = 0;
      }
      dl = 0;
      dchange = 1;
    }
    if (dr == 1) {
      if (dmemory[1] == 0) {
        dmemory[0] = 1;
        dmemory[1] = 0;
      } else {
        //WiFi Reset Code Here
        //dispwreset();
      }
      dr = 0;
      dchange = 1;
    }
  }

  if (dmemory[0] == 3) {
    if (dl == 1) {
      if (dmemory[1] < 3) {
        ++dmemory[1];
      } else if (dmemory[1] == 3) {
        dmemory[1] = 0;
      }
      dl = 0;
      dchange = 1;
    }
    if (dr == 1) {
      switch (dmemory[1]) {
        case 0:
          dmemory[0] = 1;
          dmemory[1] = 1;
          break;
        case 1:
          //Limit Set Code Here
          break;
        case 2:
          //Limit Increase Code
          break;
        case 3:
          //Limit Decrease Code
          break;
      }
      dr = 0;
      dchange = 1;
    }
  }

  if (dmemory[0] == 4) {
    if (dl == 1) {
      if (dmemory[1] == 0) {
        dmemory[1] = 1;
      } else {
        dmemory[1] = 0;
      }
      dl = 0;
      dchange = 1;
    }
    if (dr == 1) {
      if (dmemory[1] == 0) {
        dmemory[0] = 1;
        dmemory[1] = 2;
      } else {
        //Usage Reset Code Here
        //disps();
      }
      dr = 0;
      dchange = 1;
    }
  }
}

void disp() {
  if (millis() - disprMillis > screenrefresh || dchange == 1) {
    disprMillis = millis();
    display.clearDisplay();
    disptop();
    dispmain();
    display.display();
    dchange = 0;
    //test();
  }
}

void dispmain() {
  switch (dmemory[0]) {
    case 0:
      disp0();
      break;
    case 1:
      disp1();
      break;
    case 2:
      disp2();
      break;
    case 3:
      disp3();
      break;
    case 4:
      disp4();
      break;
  }
}

void disptop () {
  if (dmemory [0] == 0) {
    //add power icon here
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(12, 0);
    display.print("Power Meter");
    dispwifi();
  } else if (dmemory [0] == 1) {
    //add settings icon here
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(12, 0);
    display.print("Settings");
    dispwifi();
  } else if (dmemory [0] == 2) {
    //add settings icon here
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(12, 0);
    display.print("WiFi Settings");
    dispwifi();
  } else if (dmemory [0] == 3) {
    //add settings icon here
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(12, 0);
    display.print("Daily Usage Limit");
    dispwifi();
  } else if (dmemory [0] == 4) {
    //add settings icon here
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(12, 0);
    display.print("Usage Reset");
    dispwifi();
  }
}

void disp0() {
  switch (dmemory[1]) {
    case 0:
      display.setTextSize(1); display.setTextColor(SSD1306_WHITE); display.setCursor(0, 16);
      display.print("Voltage:   "); display.print(voltage,2); display.println(" V");
      display.print("Current:   "); display.print(current,3); display.println(" A");
      display.print("Power:     "); display.print(power,2); display.println(" W");
      display.print("Usage:     "); display.print(energy,3); display.println(" kWh");
      display.print("Freq.:     "); display.print(frequency,1); display.println(" Hz");
      display.print("PF:        "); display.print(pf,2);
      break;
    case 1:
      display.setTextSize(2); display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 16); display.print(voltage,1); display.println(" V");
      display.setCursor(0, 40); display.print(current,3); display.println(" A");
      break;
    case 2:
      display.setTextSize(2); display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 16); display.print(power,1); display.println(" W");
      display.setCursor(0, 40); display.print(energy,3); display.println(" kWh");
      break;
    case 3: //Last Usage
      display.setTextSize(1); display.setTextColor(SSD1306_WHITE); display.setCursor(0, 16);
      display.println("Energy Consumption");
      display.setCursor(0, 30);
      display.print("1hr:     "); display.print(hourly,3); display.println(" kwh");
      display.print("24hrs:   "); display.print(daily,3); display.println(" kWh");
      display.setCursor(0, 56);
      display.print("Total:   "); display.print(energy,3); display.println(" kWh");
      break;
    case 4: //Daily Limit
      display.setTextSize(1); display.setTextColor(SSD1306_WHITE); display.setCursor(0, 16);
      display.println("Usage Limit");
      display.setCursor(0, 30);
      display.print("Limit:   "); display.print(hourly,3); display.println(" kwh");
      display.print("Usage:   "); display.print(daily,3); display.println(" kWh");
      display.setCursor(0, 56);
      display.print("Total:   "); display.print(energy,3); display.println(" kWh");
      break;
    case 8: //Warning Page
      dispw();
      break;
    case 9: //No Power Supply
      display.setTextSize(2); display.setTextColor(SSD1306_WHITE);
      display.setCursor(34, 16); display.print("Error");
      display.setCursor(22, 32); display.print("Reading");
      display.setTextSize(1);
      display.setCursor(18, 56); display.print("No Power Supply");
      break;
    }
}

void disp1() {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(12, 0); display.print("Settings");
  display.setCursor(12, 14); display.print("WiFi Settings");
  display.setCursor(12, 23); display.print("Daily Usage Limit");
  display.setCursor(12, 32); display.print("Usage Reset");
  display.setCursor(12, 41); display.print("Back");
  dispbottom();
  dispcursor();
}

void disp2() {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 16); display.print("Saved SSID:");
  display.setCursor(0, 26); display.print("SLT_FIBRE");
  display.setCursor(22, 40); display.print("Back");
  display.setCursor(74, 40); display.print("Reset");
  dispbottom();
  dispcursor();
}

void disp3() {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 16); display.print("Current:");
  display.setCursor(0, 24); display.print("New:");
  display.setCursor(8, 40); display.print("Back");
  display.setCursor(42, 40); display.print("Set");
  display.setCursor(72, 40); display.print("(+)");
  display.setCursor(102, 40); display.print("(-)");
  dispbottom();
  dispcursor();
}

void disp4() {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 12); display.print("   Following action    cannot be undo, Do  you want to proceed");
  display.setCursor(22, 44); display.print("Back");
  display.setCursor(70, 44); display.print("Proceed");
  dispbottom();
  dispcursor();
}

void dispw() {
  if(schange == 0) {
    if(voltHigh - voltage < 10 && voltHigh - voltage >= 0) {
      //Caution Icon
      display.setTextSize(1); display.setTextColor(SSD1306_WHITE);
      display.setCursor(56, 20); display.print("Overvoltage");
      display.setCursor(68, 30); display.print(voltage, 3);
    }
  } else {
    //Warning Icon
    display.setTextSize(1); display.setTextColor(SSD1306_WHITE);
    display.setCursor(56, 12); display.print("Overvoltage");
    display.setCursor(68, 22); display.print(voltage, 3);
    display.setCursor(70, 34); display.print("Device");
    display.setCursor(53, 42); display.print("Disconnected");
  }
}

//void disps() //Complete Code Here

//void dispwreset() //Complete Code Here

void dispbottom() {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(20, 56); display.print("Move");
  display.setCursor(72, 56); display.print("Select");
}

void dispcursor () {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  if (dmemory[0] == 1) {
    switch (dmemory[1]) {
      case 0:
        display.setCursor(2, 14);display.print(">");
        break;
      case 1:
        display.setCursor(2, 23);display.print(">");
        break;
      case 2:
        display.setCursor(2, 32);display.print(">");
        break;
      case 3:
        display.setCursor(2, 41);display.print(">");
        break;
    }
  } else if (dmemory[0] == 2) {
    switch (dmemory[1]) {
      case 0:
        display.setCursor(13, 40);display.print(">");
        break;
      case 1:
        display.setCursor(66, 40);display.print(">");
        break;
    }
  } else if (dmemory[0] == 3) {
    switch (dmemory[1]) {
      case 0:
        display.setCursor(1, 40);display.print(">");
        break;
      case 1:
        display.setCursor(35, 40);display.print(">");
        break;
      case 2:
        display.setCursor(66, 40);display.print(">");
        break;
      case 3:
        display.setCursor(96, 40);display.print(">");
        break;
    }
  } else if (dmemory[0] == 4) {
      switch (dmemory[1]) {
      case 0:
        display.setCursor(13, 44);display.print(">");
        break;
      case 1:
        display.setCursor(62, 44);display.print(">");
        break;
    }
  }
}

void dispwifi() {
  //display.drawBitmap(120,0,cross,8,8,WHITE);
}

void relay() {
  if(voltage >= voltHigh || (voltage <= voltLow && voltage > 0)) {
    schange = 1;
    relayMillis = millis();
  } else if (frequency >= freqHigh || (frequency <= freqLow && frequency > 0)) {
    schange = 1;
    relayMillis = millis();
  }

  if (schange == 1 && millis() - relayMillis < relaytimer) {
    sswitch = 1;
  } else {
    schange = 0;
    sswitch = 0;
  }

  if (sswitch == 1) {
    digitalWrite(RLY, HIGH);
  } else {
    digitalWrite(RLY, LOW);
  }
}

void meterreading() {
  if (millis() - meterMillis > meterrefresh) {
    meterMillis = millis();
    voltage = pzem.voltage();

    if( !isnan(voltage) ){
      meter = 1;
      current = pzem.current();
      power = pzem.power();
      energy = pzem.energy();
      frequency = pzem.frequency();
      pf = pzem.pf();
      if(use == 0) {
        hAry[0] = energy;
        dAry[0] = energy;
        use = 1;
      }
    } else {
      meter = 0;
      voltage = 0;
      current = 0;
      power = 0;
      frequency = 0;
      pf = 0;
    }
  }
}

void usagememory() {
  if (use == 0) {
    usageMillis = millis();
  }
  if(millis() - usageMillis > usagerefresh && use == 1) {
    usageMillis = millis();
    if (h2 == 0 && h1 < 59) {
      ++h1;
      hAry[h1] = energy;
      hourly = hAry[h1] - hAry[0];
    } else if (h2 == 0 && h1 == 59) {
      h1 = 0;
      ++d1;
      hAry[0] = energy;
      hourly = hAry[0] - hAry[1];
      h2 = 1;
    } else {
      if(h1 < 59) {
        ++h1;
        h12 = h1 + 1;
        hAry[h1] = energy;
        hourly = hAry[h1] - hAry[h12];
      } else if (h1 == 59) {
        h1 = 0;
        ++d1;
        hAry[0] = energy;
        hourly = hAry[0] - hAry[1];
      }
    }
    if(d1 > 23) {
      d1 = 0;
    }
    if(d2 == 0 && d1 == 0) {
      daily = hourly;
    } else if (d2 == 0 && d1 > 0 && d1 < 23) {
      dAry[d1] = energy;
      daily = dAry[d1] - dAry[0];
    } else if (d2 == 0 && d1 == 23) {
      dAry[23] = energy;
      daily = dAry[23] - dAry[0];
      if (h1 == 59) {
        d2 = 1;
      }
    } else {
      if(d1 < 23) {
        d12 = d1 + 1;
        dAry[d1] = energy;
        daily = dAry[d1] - dAry[d12];
      } else if (d1 == 23) {
        dAry[d1] = energy;
        daily = dAry[d1] - dAry[0];
      }
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
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
}