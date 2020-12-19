#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64
#define REPORTING_PERIOD_MS 1000
#define OLED_RESET -1

PulseOximeter Oximeter;
const int buttonPin = 14;
uint32_t lastReport = 0;
// auto timer = timer_create_default();
// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {

    //Setup baud-rate to 115200 bps
    Serial.begin(115200);

    //Initialize and checking Oximeter Sensor
    Serial.println("Initializing oximeter sensor");
    if(!Oximeter.begin()) {
        Serial.println("FAILED");
        while(1);
    }
    else {
        Serial.println("SUCCESS");
    }

    pinMode(buttonPin, INPUT); 
    
    
    // Oximeter.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
}

void loop () {
    
    // Getting Heart Rate and Oxygen value from MAX30100 Sensor
    Oximeter.update();
    float heartValue = Oximeter.getHeartRate();
    int oxygenValue = Oximeter.getSpO2();
    if (millis() - lastReport > REPORTING_PERIOD_MS) {
        Serial.print("Heart rate: ");
        Serial.println(heartValue);
        Serial.print("Oxygen Saturation: ");
        Serial.print(oxygenValue);
        Serial.println("%\n");

        lastReport = millis();
    }

    // long int duration = pulseInLong(buttonPin, HIGH, 500000);
    // int timesPressed = 0;

    // // Handle press or hold button
    // if (duration > 1000000 && duration  < 3000000) {
    //     Serial.println("Clicked");
    //     timesPressed++;
    // } else if (duration > 3000000) { // if 3 seconds
    //     Serial.println("Hold!");
    // }
    
    // // Double click handler
    // if (timesPressed == 1) { // On first press, start the timer
    //     timer.tick();
    // } 

    // if (timer.done) {
    //     Serial.println("Clicked");
    // } else if (timesPressed == 2 && !timer.done) {
    //     Serial.println("Double click!");
    //     timesPressed = 0;
    //     timer.cancel();
    // }
}

