#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define BTN1 D5  //Main Button
#define BTN2 D6  //Reset Button

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
bool switch1 = 0;   //Left Button
bool switch2 = 0;   //Right Button
bool dstat = 1;     //Display Status


int screenrefresh = 2000;  //Screen Refresh Time

int dmemory [2] = {};

void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
}

void loop() {
  switch1 = digitalRead(BTN1);
  switch2 = digitalRead(BTN2);

  disp();
  //dmemory[0] = 2; dmemory[1] = 1;

  delay(100);
}

void disp() {
  if (millis() - disprMillis > screenrefresh) {
    disprMillis = millis();
    display.clearDisplay();
    disptop();
    dispmain();
    display.display();
    test();
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
      //disp08();
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

void test() {
  if (dmemory[0] == 0) {
    switch (dmemory[1]) {
      case 0:
        dmemory[1] = 1;
        break;
      case 1:
        dmemory[1] = 2;
        break;
      case 2:
        dmemory[1] = 3;
        break;
      case 3:
        dmemory[1] = 4;
        break;
      case 4:
        dmemory[1] = 9;
        break;
      case 9:
        dmemory[0] = 1;
        dmemory[1] = 0;
        break;
    }
  } else if (dmemory[0] == 1) {
    switch (dmemory[1]) {
      case 0:
        dmemory[1] = 1;
        break;
      case 1:
        dmemory[1] = 2;
        break;
      case 2:
        dmemory[1] = 3;
        break;
      case 3:
        dmemory[0] = 2;
        dmemory[1] = 0;
        break;
    }
  } else if (dmemory[0] == 2) {
    switch (dmemory[1]) {
      case 0:
        dmemory[1] = 1;
        break;
      case 1:
        dmemory[0] = 3;
        dmemory[1] = 0;
        break;
    }
  } else if (dmemory[0] == 3) {
    switch (dmemory[1]) {
      case 0:
        dmemory[1] = 1;
        break;
      case 1:
        dmemory[1] = 2;
        break;
      case 2:
        dmemory[1] = 3;
        break;
      case 3:
        dmemory[0] = 4;
        dmemory[1] = 0;
        break;
    }
  } else if (dmemory[0] == 4) {
    switch (dmemory[1]) {
      case 0:
        dmemory[1] = 1;
        break;
      case 1:
        dmemory[0] = 0;
        dmemory[1] = 0;
        break;
    }
  }
}