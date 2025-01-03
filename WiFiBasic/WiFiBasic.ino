#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//OLED Display Settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

bool wifi = 0;

void setup() {
    // WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
    // it is a good practice to make sure your code sets wifi mode how you want it.

    // put your setup code here, to run once:
    Serial.begin(115200);

    oledboot();

    bootscreen(); //Bootscreen
    
    //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wm;

    // reset settings - wipe stored credentials for testing
    // these are stored by the esp library
    // wm.resetSettings();

    // Automatically connect using saved credentials,
    // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
    // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
    // then goes into a blocking loop awaiting configuration and will return success result

    bool res;
    // res = wm.autoConnect(); // auto generated AP name from chipid
    res = wm.autoConnect("Power Meter"); // anonymous ap
    // res = wm.autoConnect("AutoConnectAP","password"); // password protected ap

    if(!res) {
        Serial.println("Failed to connect");
        wifi = 0;
        // ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
        wifi = 1;
    }
    delay(2000);
}

void loop() {
    // put your main code here, to run repeatedly:   
    if (wifi == 0) {
        display.clearDisplay();
        display.setTextSize(2); display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 16); display.println("WiFi");
        display.setCursor(0, 40); display.println("Not Connected");
        display.display();
    } else {
        display.clearDisplay();
        display.setTextSize(2); display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 16); display.println("WiFi");
        display.setCursor(0, 40); display.println("Connected");
        display.display();
    }

    delay(1000);
}

void oledboot() {
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
}

void bootscreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(30,56); display.print("Initializing");
  display.display();
}