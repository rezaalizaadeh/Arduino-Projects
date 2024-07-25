// Define the pin for the microphone analog output
const int microphonePin = A0;
// Define the pin for the potentiometer
const int potPin = A1;
// Define the pin for the LED indicator
const int ledPin = 13;

// Initialize variables
int snoreThreshold = 500; // Initial threshold value
int potValue = 0; // Variable to store potentiometer value
int noiseLevel = 50; // Noise level threshold for filtering
unsigned long lastSnoreTime = 0; // Time of last snore detection

void setup() {
  Serial.begin(9600); // Initialize serial communication for debugging
  pinMode(ledPin, OUTPUT); // Set LED pin as output
}

void loop() {
  // Read analog input from the microphone
  int microphoneValue = analogRead(microphonePin);
  // Read analog input from the potentiometer
  potValue = analogRead(potPin);
  
  // Check if the analog read operation was successful
  if (microphoneValue == -1 || potValue == -1) {
    Serial.println("Error reading sensor values");
    return; // Exit the loop if an error occurred
  }
  
  // Dynamically adjust the snore threshold using the potentiometer value
  snoreThreshold = map(potValue, 0, 1023, 100, 1000); // Map potentiometer value to threshold range
  
  // Apply noise filtering
  if (microphoneValue < noiseLevel) {
    microphoneValue = 0; // Set low values below noise level to zero
  }
  
  // Check if the sound level exceeds the snore threshold
  if (microphoneValue > snoreThreshold) {
    // Check if enough time has passed since the last snore detection
    unsigned long currentTime = millis();
    if (currentTime - lastSnoreTime > 1000) { // Minimum interval between snore detections (1 second)
      // Snore detected
      Serial.println("Snore detected!");
      
      // Turn on LED indicator
      digitalWrite(ledPin, HIGH);
      
      // Update last snore detection time
      lastSnoreTime = currentTime;
      
      // You can add further actions here, such as triggering an alarm or notification
    }
  } else {
    // Turn off LED indicator
    digitalWrite(ledPin, LOW);
  }
  
  // Add a small delay to prevent rapid reading
  delay(100);
}
