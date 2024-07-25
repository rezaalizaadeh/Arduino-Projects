// Define the pin connected to the alcohol sensor
const int alcoholPin = A0;

// Define the pin connected to the LED
const int ledPin = 13;

void setup() {
  Serial.begin(9600); // Initialize serial communication for debugging
  pinMode(ledPin, OUTPUT); // Set LED pin as an output
}

void loop() {
  // Read analog value from alcohol sensor
  int alcoholValue = analogRead(alcoholPin);
  
  // Convert analog value to voltage (0-5V)
  float voltage = alcoholValue * (5.0 / 1023.0);
  
  // Convert voltage to alcohol concentration (adjust as needed for your sensor)
  float alcoholConcentration = voltage * 100; // Assuming linear relationship, adjust accordingly
  
  // Print alcohol concentration for debugging
  Serial.print("Alcohol Concentration: ");
  Serial.print(alcoholConcentration);
  Serial.println("%");
  
  // If alcohol concentration exceeds a certain threshold, blink the LED
  if (alcoholConcentration > 5.0) { // Adjust threshold as needed
    blink(ledPin, 500); // Blink LED with a 500ms interval
  }
}

// Function to blink the LED
void blink(int pin, int interval) {
  digitalWrite(pin, HIGH); // Turn on LED
  delay(interval / 2); // Wait half of the interval
  digitalWrite(pin, LOW); // Turn off LED
  delay(interval / 2); // Wait the remaining half of the interval
}
