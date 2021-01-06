class Button {
  
    private:
    
        bool isPressed = false;
        int pin, currentState = LOW, lastBounceState = LOW;
        long startPressTime = 0, lastBounceTime = 0;

        // Returns true if button has stabilized for the duration of [debounceTime]
        bool debounceCheck(int debounceTime) {
            if (currentState != lastBounceState) {
                lastBounceTime = millis();
                lastBounceState = currentState;
            }
            return (millis() - lastBounceTime > debounceTime);
        }

    public:

        // Defines a button with pin x as input
        Button(int x) {
            pin = x;
            pinMode(pin, INPUT);
        }

        // Returns true if button is pressed
        bool getState() {
            return isPressed;
        }

        // Return microcontroller pin number as defined in the constructor
        int getPin() {
            return pin;
        }

        // Read the button state then checks if button is stable and if button state has changed 
        // 
        // [debounceTime] dictates how long the state needs to stay before considered stable
        //
        // Returns how long the time between button press and release in miliseconds
        int handlePress(int debounceTime) {
            currentState = digitalRead(pin);
            if (debounceCheck(debounceTime)) {
                if (currentState == HIGH && !isPressed) {
                    startPressTime = millis();
                    isPressed = true;
                } else if (currentState == LOW && isPressed) {
                    isPressed = false;
                    return millis() - startPressTime;
                }
            }
            return 0;
        }
};