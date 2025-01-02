#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define BTN1 D6  //Main Button
#define BTN2 D5  //Reset Button

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

unsigned long dispMillis = 0;     //Display Timeout Timer
unsigned long disprMillis = 0;    //Display Refresh Timer
unsigned long settingMillis = 0;  //Setting Hold Timer

bool meter = 1;     //PZEM Connection
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
int dmemory [4] = {};      //Display Memory Array


int screenrefresh = 2000;  //Screen Refresh Time
int settingHold = 5000;    //Settings Hold Time

int voltHigh = 260; //Voltage High Limit
int voltLow = 200;  //Voltage Low Limit
int freqHigh = 53;  //Frequency High Limit
int freqLow = 47;   //Frequency Low Limit

void setup() {
  Serial.begin(9600);

  pinMode(BTN1, INPUT);
  pinMode(BTN2, INPUT);

  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
}

void loop() {
  s1 = digitalRead(BTN1);
  s2 = digitalRead(BTN2);

  disp();
  button();
  //dmemory[0] = 2; dmemory[1] = 1;

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
    if(meter == 0) {
      dmemory[1] = 9;
    } else {
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