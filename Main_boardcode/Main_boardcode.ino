MAIN BOARD

// Time constants
const int greenTime = 5000;     // 5 seconds
const int yellowTime = 2000;    // 2 seconds
const int redTime = 5000;       // 5 seconds
const int pedCrossingTime = 10000;  // 10 seconds

// Traffic light states
enum TrafficState {
    STATE_GREEN,
    STATE_YELLOW,
    STATE_RED,
    STATE_COMPLETE_CYCLE_GREEN,
    STATE_COMPLETE_CYCLE_YELLOW,
    STATE_PED_CROSSING,
    STATE_PED_FINISHING
};

// TrafficLight class as per diagram
class TrafficLight {
private:
    int Green;   // Pin for green light
    int Yellow;  // Pin for yellow light
    int Red;     // Pin for red light

public:
    TrafficLight(int green, int yellow, int red) {
        Green = green;
        Yellow = yellow;
        Red = red;
        
        pinMode(Green, OUTPUT);
        pinMode(Yellow, OUTPUT);
        pinMode(Red, OUTPUT);
    }

    void greenOn() {
        digitalWrite(Green, HIGH);
        digitalWrite(Yellow, LOW);
        digitalWrite(Red, LOW);
    }

    void yellowOn() {
        digitalWrite(Green, LOW);
        digitalWrite(Yellow, HIGH);
        digitalWrite(Red, LOW);
    }

    void redOn() {
        digitalWrite(Green, LOW);
        digitalWrite(Yellow, LOW);
        digitalWrite(Red, HIGH);
    }

    void allOff() {
        digitalWrite(Green, LOW);
        digitalWrite(Yellow, LOW);
        digitalWrite(Red, LOW);
    }
};

// PedestrianLight class as per diagram
class PedestrianLight {
private:
    int Red;    // Pin for red light
    int Green;  // Pin for green light

public:
    PedestrianLight(int red, int green) {
        Red = red;
        Green = green;
        
        pinMode(Red, OUTPUT);
        pinMode(Green, OUTPUT);
    }

    void greenOn() {
        digitalWrite(Red, LOW);
        digitalWrite(Green, HIGH);
    }

    void redOn() {
        digitalWrite(Red, HIGH);
        digitalWrite(Green, LOW);
    }

    void allOff() {
        digitalWrite(Red, LOW);
        digitalWrite(Green, LOW);
    }
};

// Global variables
TrafficLight trafficLight(11, 12, 13);        // Green, Yellow, Red pins
PedestrianLight pedestrianLight(4, 3);     // Red, Green pins
TrafficState currentState = STATE_GREEN;
unsigned long stateTimer = 0;
boolean buttonPressed = false;
boolean inTransition = false;  // New flag to track if we're transitioning states

void setup() {
    Serial.begin(9600);  // Initialize UART
    stateTimer = millis();
    pedestrianLight.redOn();
}

void loop() {
    // Check for UART messages from pedestrian button controller
    if (Serial.available() > 0) {
        char received = Serial.read();
        if (received == 'P' && !inTransition) {  // Only accept button press if not in transition
            buttonPressed = true;
        }
    }

    // State machine
    switch (currentState) {
        case STATE_GREEN:
            inTransition = false;  // Reset transition flag in stable state
            trafficLight.greenOn();
            pedestrianLight.redOn();
            
            if (buttonPressed) {
                currentState = STATE_YELLOW;
                stateTimer = millis();
                inTransition = true;  // Set transition flag
            }
            else if (millis() - stateTimer >= greenTime) {
                currentState = STATE_YELLOW;
                stateTimer = millis();
                inTransition = true;  // Set transition flag
            }
            break;

        case STATE_YELLOW:
            trafficLight.yellowOn();
            pedestrianLight.redOn();
            
            if (millis() - stateTimer >= yellowTime) {
                currentState = STATE_RED;
                stateTimer = millis();
            }
            break;

        case STATE_RED:
            inTransition = false;  // Reset transition flag in stable state
            trafficLight.redOn();
            pedestrianLight.redOn();
            
            if (buttonPressed) {
                currentState = STATE_PED_CROSSING;
                stateTimer = millis();
                buttonPressed = false;
            }
            else if (millis() - stateTimer >= redTime) {
                currentState = STATE_GREEN;
                stateTimer = millis();
            }
            break;

        case STATE_COMPLETE_CYCLE_GREEN:
            trafficLight.greenOn();
            pedestrianLight.redOn();
            
            if (millis() - stateTimer >= 3000) {
                currentState = STATE_COMPLETE_CYCLE_YELLOW;
                stateTimer = millis();
            }
            break;

        case STATE_COMPLETE_CYCLE_YELLOW:
            trafficLight.yellowOn();
            pedestrianLight.redOn();
            
            if (millis() - stateTimer >= yellowTime) {
                currentState = STATE_PED_CROSSING;
                stateTimer = millis();
            }
            break;

        case STATE_PED_CROSSING:
            trafficLight.redOn();
            pedestrianLight.greenOn();
            
            if (millis() - stateTimer >= pedCrossingTime) {
                currentState = STATE_PED_FINISHING;
                stateTimer = millis();
            }
            break;

        case STATE_PED_FINISHING:
            trafficLight.redOn();
            pedestrianLight.redOn();
            
            if (millis() - stateTimer >= 1000) {
                currentState = STATE_GREEN;
                stateTimer = millis();
            }
            break;
    }
}