#include <Wire.h>
#include <MAX30100_PulseOximeter.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Adafruit_GFX.h>
#include <OakOLED.h>
#include <Blynk.h>
#include <BlynkSimpleEsp8266.h>

#include "button.h"


// Defined MAX30100 sampling & reporting period
#define SAMPLING_PERIOD_MS 5000
#define REPORTING_PERIOD_MS 2000

// Setup OLED 64 x 128 LCD
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Setup blynk device credentials
#define AUTH_BLYNK "k9nkD6eI76yIdR513T3i1pAUNqTyliPa"

// Setup network credentials
#define SSID "∆x"
#define SSID_PASSWORD "aabbccdd"

PulseOximeter Oximeter;
OakOLED oled;
Button b1(14);

float heartValue;
int oxygenValue;
unsigned long currentMillis;
uint32_t lastReport = 0;

void readOximeter(float heartValue, int oxygenValue);
void initializeOLED();
void initializeWiFi();
void initializeOximeter();
void initializeBlynk();
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
    initializeBlynk();

    Oximeter.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);

}

void loop () { 
    readOximeter(heartValue, oxygenValue);

    int pressedTime = b1.handlePress(100);

    if (pressedTime != 0) {
        if (pressedTime < 3000) {
            Serial.println("clicked!");
        } else {
            Serial.println("hold!");
        }
    }
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

// Initialize Blynk dashboard and platform connection
void initializeBlynk() {
    Blynk.begin(AUTH_BLYNK, SSID, SSID_PASSWORD);
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
    Blynk.run();
    heartValue = Oximeter.getHeartRate();
    oxygenValue = Oximeter.getSpO2();

    if (millis() - lastReport > REPORTING_PERIOD_MS) {
        Serial.print("Heart rate: ");
        Serial.println(heartValue);
        
        Serial.print("Oxygen Saturation: ");
        Serial.print(oxygenValue);
        Serial.println("%\n");

        Blynk.virtualWrite(V6, heartValue);
        Blynk.virtualWrite(V7, oxygenValue);

        showSensorValue();
        lastReport = millis();
    }
}



