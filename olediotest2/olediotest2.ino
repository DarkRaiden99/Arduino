#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define LED D0
#define BTN D7

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int p = 0;
int b = 0;
int c = 0;

void setup() {
  Serial.begin(9600);

  pinMode(LED, OUTPUT);
  pinMode(BTN, INPUT);

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
  
  b = digitalRead(BTN);

  if(b == HIGH){
    p = 1;
    ++c;
  } else {
    p = 0;
    c = 0;
  }
  
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,10);
  display.print("Led: ");
  display.println(p);
  display.print("Btn: ");
  display.println(b);
  display.print("C: ");
  display.println(c);
  display.display();

  if(p == 0){
    digitalWrite(LED, LOW);
  } else {
    digitalWrite(LED, HIGH);
  }

  delay(1000);

}
