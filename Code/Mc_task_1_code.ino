class TrafficLight {
  private:
    int GREEN, YELLOW, RED;

  public:
    // Constructor
    TrafficLight(int green, int yellow, int red) {
      GREEN = green;
      YELLOW = yellow;
      RED= red;

      // Set pins as OUTPUT
      pinMode(GREEN, OUTPUT);
      pinMode(YELLOW, OUTPUT);
      pinMode(RED, OUTPUT);
    }

    // Method to turn on all lights
    void greenOn() {
      digitalWrite(GREEN, HIGH);
      digitalWrite(YELLOW, LOW);
      digitalWrite(RED, LOW);
    }

    
    void yellowOn() {
      digitalWrite(GREEN, LOW);
      digitalWrite(YELLOW, HIGH);
      digitalWrite(RED, LOW);
    }

    void redOn() {
      digitalWrite(GREEN, LOW);
      digitalWrite(YELLOW, LOW);
      digitalWrite(RED, HIGH);
    }

    // Method to turn off all lights
    void allOff() {
      digitalWrite(GREEN, LOW);
      digitalWrite(YELLOW, LOW);
      digitalWrite(RED, LOW);
    }
};

// Create an object of TrafficLight class
TrafficLight ard(4, 3, 2);

void setup() {
  
}
int state = 0;

void loop() {
  switch (state) {
    case 0:
      ard.greenOn();  //All lights are on and wait for the given time.
      delay(3000);        
      state = 1;
      break;

    case 1:
      ard.yellowOn();  
      delay(1000);        
      state = 2;
      break;

    case 2:
      ard.redOn();     
      delay(3000);         
      state = 3;
      break;

    case 3:
      ard.yellowOn();  
      delay(1000);         
      state = 0;
      break;
  }
}