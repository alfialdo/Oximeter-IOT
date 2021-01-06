#include <Wire.h>
#include <MAX30100_PulseOximeter.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Adafruit_GFX.h>
#include <OakOLED.h>
#include <ThingerESP8266.h>

#define PUSH_BUTTON 14

// Defined MAX30100 sampling & reporting period
#define SAMPLING_PERIOD_MS 5000
#define REPORTING_PERIOD_MS 2000

// Setup OLED 64 x 128 LCD
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Setup thinger device credentials
#define USERNAME "alfialdo"
#define DEVICE_ID "oximeter"
#define DEVICE_CREDENTIAL "7VyWsXnEp3sOZx"
ThingerESP8266 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

// Setup network credentials
#define SSID "∆x"
#define SSID_PASSWORD "aabbccdd"

PulseOximeter Oximeter;
OakOLED oled;

float heartValue;
int oxygenValue;
unsigned long currentMillis;
uint32_t lastReport = 0;
bool circuitIsEnabled = true;

void readOximeter(float heartValue, int oxygenValue);
void initializeOLED();
void initializeWiFi();
void initializeOximeter();
void initializeThinger();
void handleButton();
void showSensorValue();

void setup() {

    // Setup serial port baud-rate to 115200 bps
    Serial.begin(115200);

    // Defined button input
    pinMode(PUSH_BUTTON, INPUT); 

    // Initialize Cloud Based Oximeter component
    initializeOLED();
    initializeWiFi();
    initializeOximeter();
    initializeThinger();

    Oximeter.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);

}

void loop () {
    // thing.handle();
    // int buttonValue = digitalRead(PUSH_BUTTON);
    // uint32_t lastSample = 0;
    readOximeter(heartValue, oxygenValue);
    // Oximeter.shutdown();
    // handleButton();

}

// Initialize and checking Oximeter Sensor
void initializeOximeter() {
    Serial.println("Initializing Oximeter Sensor");
    if(!Oximeter.begin()) {
        Serial.println("FAILED");
        while(1);
    }
    else {
        Serial.println("SUCCESS");
    }
}

// Initialize OLED 64 x 128 LCD
void initializeOLED() {
    oled.begin();
    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setTextColor(1);
    oled.setCursor(0, 0);
}

 // Initialize and checking WiFi connection
void initializeWiFi() {
    WiFi.begin(SSID, SSID_PASSWORD);
    while(WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("WiFi connected");
    Serial.println("IP address: "); 
    Serial.println(WiFi.localIP());
}

// Initialize thinger.io dashboard and platform connection
void initializeThinger() {
    thing.add_wifi(SSID, SSID_PASSWORD);
    thing["heartRate"] >> outputValue(heartValue);
    thing["oxygenSaturation"] >> outputValue(oxygenValue);
}

// Display Heart Rate and Oxygen Rate value on OLED LCD
void showSensorValue() {
    oled.clearDisplay();

    oled.setTextSize(1);
    oled.setTextColor(1);
    oled.setCursor(0, 0);
    oled.println("Heart Rate:");

    oled.setTextSize(1);
    oled.setTextColor(1);
    oled.setCursor(0,16);
    oled.print(Oximeter.getHeartRate());
    oled.println(" BPM");


    oled.setTextSize(1);
    oled.setTextColor(1);
    oled.setCursor(0, 30);
    oled.println("Oxygen Saturation:");

    oled.setTextSize(1);
    oled.setTextColor(1);
    oled.setCursor(0,45);
    oled.print(Oximeter.getSpO2());
    oled.println(" %");
    oled.display();
}

// Getting Heart Rate and Oxygen value from MAX30100 Sensor 
void readOximeter(float heartValue, int oxygenValue) {
    Oximeter.update();
    heartValue = Oximeter.getHeartRate();
    oxygenValue = Oximeter.getSpO2();

    if (millis() - lastReport > REPORTING_PERIOD_MS) {
        Serial.print("Heart rate: ");
        Serial.println(heartValue);
        
        Serial.print("Oxygen Saturation: ");
        Serial.print(oxygenValue);
        Serial.println("%\n");

        showSensorValue();
        lastReport = millis();
    }
}

// Handle button press and hold, then execute functions
void handleButton() {
    long int pulseDuration = pulseIn(PUSH_BUTTON, HIGH, 1000000);

    // If pulseDuration is less than 3 seconds
    if (pulseDuration == 0) return;
    if (pulseDuration  < 3000000) {
        if (circuitIsEnabled) {
            // Oximeter.shutdown();
            circuitIsEnabled = false;
        } else {
            // initializeOximeter();
            circuitIsEnabled = true;
        }
        Serial.println("Clicked");
    } else {
        // TODO: implement emergency message using things.call_endpoint
        Serial.println("Hold!");
    }
}

