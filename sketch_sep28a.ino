// ----------------------------
// Pin Definitions
// ----------------------------
int micPin = A0;      
int redPin = 11;       
int greenPin = 9;    
int bluePin = 10;     

// ----------------------------
// Clap Detection Variables
// ----------------------------
int threshold = 120;                
int clapCount = 0;                  
int micPrevious = 0;                
unsigned long firstClapTime = 0;    
unsigned long clapWindow = 2000;    

unsigned long lastClapDetected = 0; 
unsigned long debounceTime = 150;   

// ----------------------------
// Setup
// ----------------------------
void setup() {
  Serial.begin(9600);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

// ----------------------------
// Main Loop
// ----------------------------
void loop() {
  int micValue = analogRead(micPin);

  // Rising edge detection + debounce
  if (micValue > threshold && micPrevious <= threshold) {
    if (millis() - lastClapDetected > debounceTime) {
      clapCount++;
      firstClapTime = millis();       // Reset 2-second window
      lastClapDetected = millis();    
      Serial.print("Clap count: ");
      Serial.println(clapCount);
    }
  }
  micPrevious = micValue;

  // Check if clap window expired
  if (clapCount >= 1 && (millis() - firstClapTime > clapWindow)) {
    Serial.print("Sequence ended with ");
    Serial.print(clapCount);
    Serial.println(" claps");

    // ----------------------------
    // RGB LED logic based on clap count
    // ----------------------------
    switch(clapCount) {
      case 1:
        analogWrite(redPin, 0);
        analogWrite(greenPin, 0);
        analogWrite(bluePin, 0);
        break;
      case 2:
        analogWrite(redPin, 255);
        analogWrite(greenPin, 0);
        analogWrite(bluePin, 255);  // Purple
        break;
      case 3:
        analogWrite(redPin, 0);
        analogWrite(greenPin, 255);
        analogWrite(bluePin, 0);    // Green
        break;
      case 4:
        analogWrite(redPin, 0);
        analogWrite(greenPin, 0);
        analogWrite(bluePin, 255);  // Blue
        break;
      default:
        analogWrite(redPin, 255);
        analogWrite(greenPin, 255);
        analogWrite(bluePin, 255);  // White for 5+ claps
        break;
    }

    clapCount = 0; // Reset for the next sequence
  }
}
