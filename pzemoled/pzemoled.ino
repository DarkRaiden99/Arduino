
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
// warn, 32x32px
const unsigned char warn[] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x0f, 0xf0, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x7f, 0xff, 0x00, 0x01, 0xff, 0xff, 0x80, 
	0x01, 0xff, 0xff, 0x80, 0x03, 0xff, 0xff, 0xc0, 0x07, 0xfe, 0x7f, 0xe0, 0x07, 0xfe, 0x7f, 0xe0, 
	0x07, 0xfe, 0x7f, 0xe0, 0x0f, 0xfe, 0x7f, 0xf0, 0x0f, 0xfe, 0x7f, 0xf0, 0x0f, 0xfe, 0x7f, 0xf0, 
	0x0f, 0xfe, 0x7f, 0xf0, 0x0f, 0xff, 0xff, 0xf0, 0x0f, 0xff, 0xff, 0xf0, 0x07, 0xfe, 0x7f, 0xe0, 
	0x07, 0xfe, 0x7f, 0xe0, 0x03, 0xfe, 0x7f, 0xc0, 0x03, 0xff, 0xff, 0xc0, 0x01, 0xff, 0xff, 0x80, 
	0x00, 0xff, 0xff, 0x00, 0x00, 0x7f, 0xfe, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x03, 0xc0, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// crit, 32x32px
const unsigned char crit[] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 
	0x00, 0x01, 0x80, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x07, 0xe0, 0x00, 
	0x00, 0x07, 0xe0, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x0c, 0x38, 0x00, 0x00, 0x1c, 0x38, 0x00, 
	0x00, 0x1c, 0x3c, 0x00, 0x00, 0x3c, 0x3c, 0x00, 0x00, 0x3c, 0x3e, 0x00, 0x00, 0x7c, 0x3e, 0x00, 
	0x00, 0xfc, 0x3f, 0x00, 0x00, 0xfc, 0x3f, 0x80, 0x01, 0xfc, 0x3f, 0x80, 0x01, 0xfc, 0x3f, 0xc0, 
	0x03, 0xfc, 0x3f, 0xc0, 0x07, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xe0, 0x0f, 0xfe, 0x3f, 0xf0, 
	0x0f, 0xfc, 0x3f, 0xf8, 0x1f, 0xfe, 0x7f, 0xf8, 0x1f, 0xff, 0xff, 0xfc, 0x3f, 0xff, 0xff, 0xfc, 
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
const unsigned char ok[] PROGMEM  ={
  0x00, 0x01, 0x02, 0x04, 0x04, 0x48, 0x28, 0x10
};
//pwr, 8x8
const unsigned char pwr[] PROGMEM = {
	0x04, 0x08, 0x18, 0x3e, 0x7c, 0x18, 0x10, 0x20
};
//settings, 8x8
const unsigned char settings[] PROGMEM = {
	0x24, 0x74, 0x74, 0x24, 0x2e, 0x2e, 0x24, 0x24
};
//chart, 8x8
const unsigned char chart[] PROGMEM = {
	0x20, 0x51, 0x8a, 0x24, 0x71, 0xfb, 0xff, 0xff
};
// warns, 8x8px
const unsigned char warns[] PROGMEM = {
	0x3c, 0x66, 0xe7, 0xe7, 0xff, 0xe7, 0x66, 0x3c
};

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
unsigned long limitMillis = 0;    //Usage Limit Timer
unsigned long warnMillis = 0;     //Warning Timer
unsigned long warnrMillis = 0;    //Warning Reset Timer

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
bool sswitch2 = 0;  //Relay Trigger
bool schange = 0;   //Relay Last State
bool use = 0;       //Usage Last State
bool h2 = 0;        //Hourly Usage Last State
bool d2 = 0;        //Daily Usage Last State
bool pwarn = 0;     //Warning Last State
bool nwarn = 0;     //Warning State
bool cwarn = 0;     //Critical Warning State

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
float initialusage = 0; //Initial Usage
float limitusage = 0;   //Limit Usage

//Timers and Threshholds (in Milliseconds)
int screentimeout = 30000;  //Screen Timeout
int settingHold = 5000;     //Settings Hold Time
int resettimer = 10000;     //Screen Reset Timer
int relaytimer = 60000;     //Relay Reset Time
int meterrefresh = 2000;    //Meter Reading Refresh Time
int screenrefresh = 2000;   //Screen Refresh Time
int usagerefresh = 60000;   //Usage Refresh Time
int limitreset = 86400000;  //Limit Reset Time (1 Day)
int warntimer = 5000;   //Warning Timer
int warnreset = 60000;      //Warning Reset Time

float voltoffset = 0;     //Voltage Offset
float curoffset = 0;      //Current Offset
int voltHigh = 260;       //Voltage High Limit
int voltLow = 200;        //Voltage Low Limit
int freqHigh = 53;        //Frequency High Limit
int freqLow = 47;         //Frequency Low Limit
int pageCount = 4;        //Page Count
float usagelim = 0;       //Usage Limit
float usagelimNew = 0.5;  //Usage Limit Temp

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

  usagelimit();

  relay();

  warning();

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
    if(cwarn == 0) {
      if (dl == 1) {
        if (dmemory[1] > 0) {
          --dmemory[1];
        } else if (dmemory[1] == 0) {
          dmemory[1] = 4;
        } else if(dmemory[1] == 8) {
          dmemory[1] = dmemory[2];
          pwarn = 1;
        }
        dl = 0;
        dchange = 1;
      } else if (dr == 1) {
        if (dmemory[1] < 4) {
          ++dmemory[1];
        } else if (dmemory[1] == 4) {
          dmemory[1] = 0;
        } else if (dmemory[1] == 8) {
          dmemory[1] = dmemory[2];
          pwarn = 1;
        }
        dr = 0;
        dchange = 1;
      }
    }
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
        wifireset();
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
          usagelimNew = 0.5;
          break;
        case 1:
          usagelim = usagelimNew;
          break;
        case 2:
          if(usagelimNew < 10) {
            usagelimNew = usagelimNew + 0.5;
          } else {
            usagelimNew = 10;
          }
          break;
        case 3:
          if(usagelimNew > 0) {
            usagelimNew = usagelimNew - 0.5;
          } else {
            usagelimNew = 0;
          }
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
        usagereset();
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
    dispbody();
    display.display();
    dchange = 0;
    //test();
  }
}

void dispbody() {
  switch (dmemory[0]) {
    case 0:
      dispmain();
      break;
    case 1:
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(12, 0); display.print("Settings");
      display.setCursor(12, 14); display.print("WiFi Settings");
      display.setCursor(12, 23); display.print("Daily Usage Limit");
      display.setCursor(12, 32); display.print("Usage Reset");
      display.setCursor(12, 41); display.print("Back");
      dispbottom();
      dispcursor();
      break;
    case 2:
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 16); display.print("Saved SSID:");
      display.setCursor(0, 26); display.print("~ Not Configured");
      display.setCursor(22, 40); display.print("Back");
      display.setCursor(74, 40); display.print("Reset");
      dispbottom();
      dispcursor();
      break;
    case 3:
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 16); display.print("Current:");
      if (usagelim > 0) {
        display.setCursor(56, 16); display.print(usagelim, 3); display.print(" kWh");
      } else {
        display.setCursor(56, 16); display.print("No Limit");
      }
      display.setCursor(0, 24); display.print("New:");
      if (usagelimNew > 0) {
        display.setCursor(56, 24); display.print(usagelimNew, 3); display.print(" kWh");
      } else {
        display.setCursor(56, 24); display.print("No Limit");
      }
      display.setCursor(8, 40); display.print("Back");
      display.setCursor(42, 40); display.print("Set");
      display.setCursor(72, 40); display.print("(+)");
      display.setCursor(102, 40); display.print("(-)");
      dispbottom();
      dispcursor();
      break;
    case 4:
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(15, 12); display.print("Following action");
      display.setCursor(9, 22); display.print("cannot be undo, do");
      display.setCursor(6, 32); display.print("you want to proceed");
      display.setCursor(22, 44); display.print("Back");
      display.setCursor(70, 44); display.print("Proceed");
      dispbottom();
      dispcursor();
      break;
  }
}

void disptop () {
  if (dmemory [0] == 0) {
    display.drawBitmap(0,0,pwr,8,8,WHITE);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(12, 0);
    display.print("Power Meter");
    dispwifi();
  } else if (dmemory [0] == 1) {
    display.drawBitmap(0,0,settings,8,8,WHITE);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(12, 0);
    display.print("Settings");
    dispwifi();
  } else if (dmemory [0] == 2) {
    display.drawBitmap(0,0,wifi,8,8,WHITE);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(12, 0);
    display.print("WiFi Settings");
    dispwifi();
  } else if (dmemory [0] == 3) {
    display.drawBitmap(0,0,chart,8,8,WHITE);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(12, 0);
    display.print("Daily Usage Limit");
    dispwifi();
  } else if (dmemory [0] == 4) {
    display.drawBitmap(0,0,chart,8,8,WHITE);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(12, 0);
    display.print("Usage Reset");
    dispwifi();
  }
  if(sswitch == 1 || sswitch2 == 1 || nwarn == 1) {
    display.drawBitmap(120,0,warns,8,8,WHITE);
  }
}

void dispmain() {
  switch (dmemory[1]) {
    case 0:
      display.setTextSize(1); display.setTextColor(SSD1306_WHITE); display.setCursor(0, 16);
      display.print("Voltage:   "); display.print(voltage,2); display.println(" V");
      display.print("Current:   "); display.print(current,3); display.println(" A");
      display.print("Power:     "); display.print(power,2); display.println(" W");
      display.print("Usage:     "); display.print(energy,3); display.println(" kWh");
      display.print("Freq.:     "); display.print(frequency,1); display.println(" Hz");
      //display.print("PF:        "); display.print(pf,2);
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
      display.setCursor(0, 29);
      display.print("1hr:     "); display.print(hourly,3); display.print(" kWh");
      display.setCursor(0, 39);
      display.print("24hrs:   "); display.print(daily,3); display.print(" kWh");
      display.setCursor(0, 56);
      display.print("Total:   "); display.print(energy,3); display.print(" kWh");
      break;
    case 4: //Daily Limit
      display.setTextSize(1); display.setTextColor(SSD1306_WHITE); display.setCursor(0, 16);
      display.println("Usage Limit");
      display.setCursor(0, 29);
      display.print("Limit:   ");
      if (usagelim > 0) {
        display.print(usagelim, 3); display.print(" kWh");
      } else {
        display.print("No Limit");
      }
      display.setCursor(0, 39);
      display.print("Usage:   "); display.print(limitusage, 3); display.print(" kWh");
      display.setCursor(0, 56);
      display.print("Total:   "); display.print(energy, 3); display.print(" kWh");
      break;
    case 8: //Warning Page
      if(schange == 0) {
        display.drawBitmap(2,16,warn,32,32,WHITE);
        display.setTextSize(1); display.setTextColor(SSD1306_WHITE);
        if(voltHigh - voltage <= 10) {
          display.setCursor(56, 20); display.print("Overvoltage");
        } else {
          display.setCursor(54, 20); display.print("Undervoltage");
        }
        display.setCursor(68, 30); display.print(voltage, 2); display.print(" V");
      } else {
        display.drawBitmap(2,16,crit,32,32,WHITE);
        display.setTextSize(1); display.setTextColor(SSD1306_WHITE);
        if(voltHigh - voltage <= 10) {
          display.setCursor(56, 20); display.print("Overvoltage");
        } else {
          display.setCursor(54, 20); display.print("Undervoltage");
        }
        display.setCursor(68, 22); display.print(voltage, 2); display.print(" V");
        display.setCursor(70, 34); display.print("Device");
        display.setCursor(53, 42); display.print("Disconnected");
      }
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

  if (sswitch == 1 || sswitch2 == 1) {
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
      voltage = voltage + voltoffset;
      current = pzem.current() + curoffset;
      power = pzem.power();
      energy = pzem.energy();
      frequency = pzem.frequency();
      pf = pzem.pf();
      if(use == 0) {
        hAry[0] = energy;
        dAry[0] = energy;
        initialusage = energy;
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

void usagereset() {
  if(meter == 0) {
    display.clearDisplay();
    display.setTextSize(1); display.setTextColor(SSD1306_WHITE);
    display.setCursor(32, 15); display.print("Power Meter");
    display.setCursor(24, 25); display.print("Not Connected!");
    display.display();
    delay(2000);
    dmemory[1] = 0;
  } else {
    display.clearDisplay();
    display.setTextSize(1); display.setTextColor(SSD1306_WHITE);
    display.setCursor(31, 15); display.print("Usage Reset");
    display.setCursor(38, 25); display.print("Complete!");
    display.display();
    delay(2000);
    pzem.resetEnergy();
    dmemory[1] = 0;
  }
}

void wifireset() {
  if(wifistat == 0) {
    display.clearDisplay();
    display.setTextSize(1); display.setTextColor(SSD1306_WHITE);
    display.setCursor(6, 15); display.print("WiFi Not Configured!");
    display.setCursor(20, 25); display.println("Connect to WiFi");
    display.setCursor(8, 35); display.println("Station and Config");
    display.display();
    delay(3000);
  }
}

void usagelimit() {
  if(millis() - limitMillis < limitreset) {
    limitusage = energy - initialusage;
    if(initialusage >= usagelim && usagelim != 0) {
      sswitch2 = 1;
    } else {
      sswitch2= 0;
    }
  } else {
    limitMillis = millis();
    initialusage = energy;
    limitusage = 0;
    sswitch2 = 0;
  }
}

void warning() {
  if ((voltHigh - voltage < 10 && voltage < voltHigh) || (voltage - voltLow < 10 && voltage > voltLow) && schange == 0) {
    if (pwarn == 0 && nwarn == 0) {
      nwarn = 1;
      warnMillis = millis();
      warnrMillis = millis();
      if(dmemory[0] == 0) {
        dmemory[2] = dmemory[1];
        dmemory[1] = 8;
      } 
    }
    if (nwarn == 1 && pwarn == 0 && millis() - warnMillis > warntimer) {
      pwarn = 1;
      if(dmemory[0] == 0) {
        dmemory[1] = dmemory[2];
      }
    }
  } else if (schange == 1) {
    if(dmemory[0] == 0) {
      dmemory[2] = dmemory[1];
    }
    cwarn = 1;
    dmemory[0] = 0;
    dmemory[1] = 8;
    dchange = 1;
  }
  if (schange == 0 && cwarn == 1) {
    cwarn = 0;
    dmemory[1] = dmemory[2];
    dmemory[0] = 0;
    dchange = 1;
  }
  if (pwarn == 1 && millis() - warnrMillis > warnreset) {
    nwarn = 0;
    pwarn = 0;
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