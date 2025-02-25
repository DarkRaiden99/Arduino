#include "thingProperties.h"
#include "arduino_secrets.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <time.h>
#include <WiFiClientSecure.h>

// OLED Configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET D6
#define OLED_DC D4
#define OLED_CS D3
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, OLED_DC, OLED_RESET, OLED_CS);

// WiFi credentials
//const char* ssid = "shehan";       // Replace with your WiFi SSID
//const char* password = "12345678"; // Replace with your WiFi password

// IP Geolocation API Configuration
const char* apiKey = "f466aba2c1e440fbb7d86edd3de99972"; // Your ipgeolocation.io API key
const char* geoHost = "api.ipgeolocation.io";
const int httpsPort = 443;

// Web server instance on port 80
ESP8266WebServer server(80);

// FSR Sensor Configuration
const int fsrPins[3] = {A0, D1, D2}; // Analog pins connected to the 3 FSRs
int fsrValues[3] = {0, 0, 0};        // Variables to store FSR values
const int threshold = 490;            // Threshold to determine pressure applied
int computerVision = 0;              // Computer vision count
int currentStep = 0;                 // Tracks the sequence of FSR triggers
int passengerCount = 0;              // Total passenger count
unsigned long lastStepTime = 0;      // Timestamp of the last step
const unsigned long debounceDelay = 600; // Delay to avoid step interference

// Global variables
unsigned long sequenceStartTime = 0;
bool sequenceInProgress = false;

unsigned long cloudMillis = 0;    //Cloud Update Timer
unsigned long cloudsMillis = 0;   //Cloud Status Timer
int cloudupdatetime = 2000;   //Cloud Update Time
int cloudupdatetime2 = 10000;  //Cloud Update Time
int cloudstatustime = 5000;   //Cloud Status Time
bool wifistat = 0;    //Wifi Connection


// Sequence tracking for entry and exit
struct SequenceTracker {
    int currentStep;
    unsigned long lastStepTime;
    unsigned long sequenceStartTime;
    bool sequenceInProgress;
} entrySequence = {0, 0, 0, false}, exitSequence = {0, 0, 0, false};


// // Sequence tracking for entry and exit
// struct SequenceTracker {
//     int currentStep;
//     unsigned long lastStepTime;
//     unsigned long sequenceStartTime;
//     bool sequenceInProgress;


// SequenceTracker  entrySequence = {0, 0, 0, false};
// SequenceTracker  exitSequence = {0, 0, 0, false};

const unsigned long SEQUENCE_TIMEOUT = 3000; // 3 seconds timeout for incomplete sequences

const unsigned long incompleteStepTimeout = 2000; // 3 seconds
const unsigned long stepHoldTimeout = 3000;       // Timeout for standing on a step

unsigned long stepHoldStart[3] = {0, 0, 0};    // Track hold time for each step
bool isHoldingStep[3] = {false, false, false}; // Flags to track if a step is being held

bool waitingForStep1 = false;
bool waitingForStep2 = false;
bool waitingForStep3 = false;

bool sequenceActive = false;

unsigned long stepTimeoutStart = 0;

// Location tracking variables
String location = "Unknown";
unsigned long lastLocationUpdate = 0;
const unsigned long locationUpdateInterval = 300000; // 5 minutes in milliseconds

// NTP Time Configuration
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 5 * 3600 + 30 * 60; // GMT+5:30
const int daylightOffset_sec = 0;             // No daylight saving time in Sri Lanka

// Function to get the current time as a string
String getTimeString() {
  time_t now = time(nullptr);
  struct tm* timeinfo = localtime(&now);
  char buffer[20];
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
  return String(buffer);
}

// Function to update location
void updateLocation() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClientSecure client;
    client.setInsecure(); // Skip certificate verification

    if (!client.connect(geoHost, httpsPort)) {
      Serial.println("Location API connection failed!");
      return;
    }

    String url = "/ipgeo?apiKey=" + String(apiKey);

    HTTPClient http;
    http.begin(client, String("https://") + geoHost + url);
    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK) {
      String response = http.getString();
      StaticJsonDocument<1024> doc;
      DeserializationError error = deserializeJson(doc, response);

      if (!error) {
        const char* city = doc["city"];
        const char* state = doc["state_prov"];
        const char* country = doc["country_name"];
        location = String(city) + ", " + String(state) + ", " + String(country);
        Serial.println("\n=== Location Updated ===");
        Serial.println(location);
      }
    }
    http.end();
  }
}

// Function to log step number, FSR value, and timestamp
void logStep(int step, int value) {
  unsigned long currentTime = millis();
  Serial.printf("Step %d triggered at %.3f seconds with FSR value: %d\n", step, currentTime / 1000.0, value);
}

// Function to update OLED display
void updateOLEDDisplay() {
  display.clearDisplay(); // Clear previous content

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text

  display.setCursor(0, 0);
  display.print("FindSeat Tracker");
  display.setCursor(0, 10);
  display.print("Computer Vision:");

  display.setCursor(100, 10);
  display.print(computerVision); // Display computer vision count

  display.setCursor(0, 20);
  display.print("Carpet System:");

  display.setCursor(100, 20);
  display.print(passengerCount); // Display carpet system count

  display.setCursor(0, 30);
  display.print("Location:");
  display.setCursor(0, 40);
  display.print(location); // Display location

  display.display(); // Send buffer to the display
}

// Handle update from computer vision system
void handleUpdateCVCount() {
  if (server.hasArg("plain")) {
    String body = server.arg("plain");
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, body);

    if (!error) {
      computerVision = doc["count"];
      server.send(200, "application/json", "{\"status\":\"success\"}");
    } else {
      server.send(400, "application/json", "{\"status\":\"error\",\"message\":\"Invalid JSON\"}");
    }
  } else {
    server.send(400, "application/json", "{\"status\":\"error\",\"message\":\"No data received\"}");
  }
}


// Web server handler for root path
void handleRoot() {
  String html =R"rawliteral(
    <!DOCTYPE html>
    <html lang="en">
    <head>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <title>FindSeat - Bus Passenger Tracker</title>
      <style>
        /* General Reset */
        * {
          margin: 0;
          padding: 0;
          box-sizing: border-box;
        }
        body {
          font-family: 'Arial', sans-serif;
          background: linear-gradient(to right, #6dd5ed, #2193b0);
          color: #fff;
        }
        header {
          background-color: #004d40;
          color: white;
          text-align: center;
          padding: 20px;
          box-shadow: 0px 4px 6px rgba(0, 0, 0, 0.2);
        }
        header h1 {
          font-size: 2.8rem;
          margin-bottom: 10px;
        }
        header p {
          font-size: 1.2rem;
          margin-bottom: 10px;
        }
        .current-time {
          font-size: 1.2rem;
          font-weight: bold;
          color: #cfd8dc;
        }
        .container {
          display: flex;
          flex-direction: column;
          align-items: center;
          padding: 20px;
        }
        .count-section, .location-section {
          background-color: #00796b;
          border-radius: 12px;
          box-shadow: 0 8px 15px rgba(0, 0, 0, 0.2);
          padding: 20px;
          width: 90%;
          max-width: 500px;
          margin-bottom: 20px;
        }
        .count-boxes {
          display: flex;
          justify-content: space-between;
          gap: 20px;
        }
        .count-box {
          background-color: #004d40;
          padding: 15px;
          border-radius: 8px;
          text-align: center;
          width: 48%;
          box-shadow: 0 2px 8px rgba(0, 0, 0, 0.2);
        }
        .count-box h3 {
          font-size: 1.2rem;
          color: #aed581;
          margin-bottom: 10px;
        }
        .count-box p {
          font-size: 2.8rem;
          font-weight: bold;
          color: #fff;
        }
        .location-section p {
          font-size: 1.2rem;
          font-weight: bold;
        }
        footer {
          background-color: #004d40;
          text-align: center;
          padding: 10px;
        }
        footer p {
          font-size: 0.9rem;
          color: #cfd8dc;
        }
        @media (max-width: 768px) {
          .count-boxes {
            flex-direction: column;
            align-items: center;
          }
          .count-box {
            width: 90%;
          }
        }
      </style>
      <script>
        function fetchData() {
          fetch('/data')
            .then(response => response.json())
            .then(data => {
              document.getElementById('cv-count').textContent = data.computerVision;
              document.getElementById('carpet-count').textContent = data.passengerCount;
              document.getElementById('location').textContent = data.location;
            })
            .catch(error => console.error('Error fetching data:', error));
        }
 
        function updateTime() {
          const currentTime = new Date();
          const formattedTime = currentTime.toLocaleTimeString('en-US', {
            hour: '2-digit',
            minute: '2-digit',
            second: '2-digit',
          });
          document.getElementById('current-time').textContent = `Time: ${formattedTime}`;
        }
 
        setInterval(fetchData, 2000);
        setInterval(updateTime, 1000);
 
        fetchData();
        updateTime();
      </script>
    </head>
    <body>
      <header>
        <h1>FindSeat</h1>
        <p>Real-time Bus Passenger Tracking System</p>
        <div class="current-time" id="current-time">Loading time...</div>
      </header>
      <main class="container">
        <div class="count-section">
          <h2>Passenger Count</h2>
          <div class="count-boxes">
            <div class="count-box">
              <h3>Computer Vision Count</h3>
              <p id="cv-count">0</p>
            </div>
            <div class="count-box">
              <h3>Carpet System Count</h3>
              <p id="carpet-count">)rawliteral" + String(passengerCount) + R"rawliteral(</p>
            </div>
          </div>
        </div>
        
        <div class="location-section">
          <h2>Current Location</h2>
          <p id="location">)rawliteral" + location + R"rawliteral(</p>
        </div>
      </main>
      <footer>
        <p>&copy; 2024 FindSeat. All Rights Reserved.</p>
      </footer>
    </body>
    </html>
  )rawliteral";
    
     // Omitted for brevity, same as your existing HTML
  server.send(200, "text/html", html);
}

// API endpoint to send data
void handleData() {
  StaticJsonDocument<200> doc;
  doc["computerVision"] = computerVision;
  doc["passengerCount"] = passengerCount;
  doc["location"] = location;

  String response;
  serializeJson(doc, response);
  server.send(200, "application/json", response);
}

// // Handle update from computer vision system
// void handleUpdateCVCount() {
//   if (server.hasArg("plain")) {
//     String body = server.arg("plain");
//     StaticJsonDocument<200> doc;
//     DeserializationError error = deserializeJson(doc, body);

//     if (!error) {
//       computerVision = doc["count"];
//       server.send(200, "application/json", "{\"status\":\"success\"}");
//     } else {
//       server.send(400, "application/json", "{\"status\":\"error\",\"message\":\"Invalid JSON\"}");
//     }
//   } else {
//     server.send(400, "application/json", "{\"status\":\"error\",\"message\":\"No data received\"}");
//   }
// }

void setup() {
  Serial.begin(9600);

  // Initialize the OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Initializing...");
  display.display();

  // Initialize FSR pins
  for (int i = 0; i < 3; i++) {
    pinMode(fsrPins[i], INPUT);
  }

  initProperties();

  ArduinoCloud.begin(ArduinoIoTPreferredConnection, false);

  setDebugMessageLevel(0);

  // Connect to WiFi
  /*WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());*/
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("WiFi connected");
  display.print("IP: ");
  display.println(WiFi.localIP());
  display.display();

  // Configure time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  // Initial location update
  updateLocation();
  updateOLEDDisplay();

  // Set up web server
  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.on("/update-cv-count", HTTP_POST, handleUpdateCVCount);
  server.begin();
  Serial.println("Web server started");
}

void loop() {

  cloudupdatecycle();
  cloudupdate();

  // Read FSR values
  for (int i = 0; i < 3; i++) {
    fsrValues[i] = analogRead(fsrPins[i]);
  }
  unsigned long currentTime = millis();

     // Handle entry sequence (1 -> 2 -> 3)
    if (!entrySequence.sequenceInProgress) {
        if (fsrValues[0] > threshold && (currentTime - entrySequence.lastStepTime > debounceDelay)) {
            logStep(1, fsrValues[0]);
            entrySequence.currentStep = 1;
            entrySequence.lastStepTime = currentTime;
            entrySequence.sequenceStartTime = currentTime;
            entrySequence.sequenceInProgress = true;
        }
    } else if (entrySequence.currentStep == 1) {
        if (fsrValues[1] > threshold && (currentTime - entrySequence.lastStepTime > debounceDelay)) {
            logStep(2, fsrValues[1]);
            entrySequence.currentStep = 2;
            entrySequence.lastStepTime = currentTime;
        }
    } else if (entrySequence.currentStep == 2) {
        if (fsrValues[2] > threshold && (currentTime - entrySequence.lastStepTime > debounceDelay)) {
            logStep(3, fsrValues[2]);
            passengerCount++;
            Serial.println("Passenger IN - Count: " + String(passengerCount));
            entrySequence.currentStep = 0;
            entrySequence.lastStepTime = currentTime;
            entrySequence.sequenceInProgress = false;
        }
    }

    // Handle exit sequence (3 -> 2 -> 1)
    if (!exitSequence.sequenceInProgress) {
        if (fsrValues[2] > threshold && (currentTime - exitSequence.lastStepTime > debounceDelay)) {
            logStep(3, fsrValues[2]);
            exitSequence.currentStep = 3;
            exitSequence.lastStepTime = currentTime;
            exitSequence.sequenceStartTime = currentTime;
            exitSequence.sequenceInProgress = true;
        }
    } else if (exitSequence.currentStep == 3) {
        if (fsrValues[1] > threshold && (currentTime - exitSequence.lastStepTime > debounceDelay)) {
            logStep(2, fsrValues[1]);
            exitSequence.currentStep = 4;
            exitSequence.lastStepTime = currentTime;
        }
    } else if (exitSequence.currentStep == 4) {
        if (fsrValues[0] > threshold && (currentTime - exitSequence.lastStepTime > debounceDelay)) {
            logStep(1, fsrValues[0]);
            if (passengerCount > 0) {
                passengerCount--;
            }
            Serial.println("Passenger OUT - Count: " + String(passengerCount));
            exitSequence.currentStep = 0;
            exitSequence.lastStepTime = currentTime;
            exitSequence.sequenceInProgress = false;
        }
    }

    // Check for sequence timeouts
    if (entrySequence.sequenceInProgress && 
        (currentTime - entrySequence.sequenceStartTime > SEQUENCE_TIMEOUT)) {
        Serial.println("Entry sequence timeout - resetting");
        entrySequence.currentStep = 0;
        entrySequence.sequenceInProgress = false;
    }

    if (exitSequence.sequenceInProgress && 
        (currentTime - exitSequence.sequenceStartTime > SEQUENCE_TIMEOUT)) {
        Serial.println("Exit sequence timeout - resetting");
        exitSequence.currentStep = 0;
        exitSequence.sequenceInProgress = false;
    }


  // Serve web requests
  server.handleClient();

  // Update OLED display
  updateOLEDDisplay();

  delay(100);
}

void cloudupdatecycle() {
  if(wifistat == 0) {
    if(millis() - cloudMillis > cloudupdatetime2) {
      cloudMillis = millis();
      ArduinoCloud.update();
      cloudconnection();
    }
  } else {
    if(millis() - cloudMillis > cloudupdatetime) {
      cloudMillis = millis();
      ArduinoCloud.update();
      cloudconnection();
    }
  }
}

void cloudconnection() {
  if(millis() - cloudsMillis > cloudstatustime) {
    cloudsMillis = millis();
    if (ArduinoCloud.connected()) {
      wifistat = 1;
    } else {
      wifistat = 0;
    }
  }
}

void cloudupdate() {
  //Variables
}
