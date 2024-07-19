#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const unsigned char wifiicon[] PROGMEM  ={ // wifi icon
  0x7e, 0x81, 0x3c, 0x42, 0x18, 0x24,0x00, 0x18
};

int p = 0;

void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(10); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

}

void loop() {
  display.clearDisplay();

  display.drawBitmap(120,0,wifiicon,8,8,WHITE);
  display.display();

  /*display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);
  display.println("IoT Based Power Meter");
  display.display();

  display.setTextSize(1);
  display.println();
  display.print("Voltage:   ");
  display.print(voltage,1);
  display.println(" V");
  display.display();

  display.print("Current:   ");
  display.print(current,3);
  display.println(" A");
  display.display();

  display.print("Power:     ");
  display.print(power,1);
  display.println(" W");
  display.display();

  display.print("Usage:     ");
  display.print(energy,2);
  display.println(" kWh");
  display.display();

  display.print("Freq.:     ");
  display.print(frequency,1);
  display.println(" Hz");
  display.display();

  display.print("PF:        ");
  display.print(pf,2);
  display.display();

  display.setTextSize(2);
  display.setCursor(38,16); display.println("Error");
  display.setCursor(27,32); display.println("Reading");
  display.setTextSize(1);
  display.setCursor(29,48); display.print("No Power Supply");
  display.display();*/

  /*display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.print("P: ");
  display.print(p);
  display.display();

  if(p == 2){
    p = 0;
  } else {
    ++p;
  }*/

  delay(2000);

}
