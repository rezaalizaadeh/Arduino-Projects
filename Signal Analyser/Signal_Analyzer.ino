#include <SoftwareSerial.h>

const int micPin = A0;          // Microphone connected to analog pin A0
SoftwareSerial BTSerial(1,0);   // RX, TX for HC-05

const int threshold = 500;      // Adjust threshold for zero crossing detection

void setup() {
  Serial.begin(9600);          // Initialize serial communication with PC
  BTSerial.begin(9600);        // Initialize serial communication with HC-05
}

void loop() {
  int minValue = 1023;
  int maxValue = 0;
  long sum = 0;
  int sampleCount = 1000;
  int previousSignal = analogRead(micPin);
  int zeroCrossingCount = 0;
  unsigned long startTime = millis();
  unsigned long lastZeroCrossingTime = startTime;
  unsigned long currentTime;

  for (int i = 0; i < sampleCount; i++) {
    int currentSignal = analogRead(micPin);
    sum += currentSignal;
    if (currentSignal < minValue) {
      minValue = currentSignal;
    }
    if (currentSignal > maxValue) {
      maxValue = currentSignal;
    }
    // Detect zero crossing
    if ((previousSignal > threshold && currentSignal <= threshold) || (previousSignal < threshold && currentSignal >= threshold)) {
      zeroCrossingCount++;
      lastZeroCrossingTime = millis();
    }
    previousSignal = currentSignal;
    // Adjust delay or remove for faster sampling
    delayMicroseconds(100); // Adjust delay as needed
  }

  float mean = sum / (float)sampleCount;
  float power = calculatePower(sum, sampleCount);
  float fundamentalPeriod = calculateFundamentalPeriod(zeroCrossingCount, startTime, lastZeroCrossingTime);

  // Output to Serial Monitor
  Serial.print("Mean: ");
  Serial.println(mean);
  Serial.println(" (ADC units)");
  Serial.print("Min: ");
  Serial.println(minValue);
  Serial.println(" (ADC units)");
  Serial.print("Max: ");
  Serial.println(maxValue);
  Serial.println(" (ADC units)");
  Serial.print("Power: ");
  Serial.println(power);
  Serial.println(" (ADC units^2)");
  Serial.print("Fundamental Period: ");
  Serial.println(fundamentalPeriod);


  // Transmit data via Bluetooth
  BTSerial.print("Mean: ");
  BTSerial.println(mean);
  BTSerial.print("Min: ");
  BTSerial.println(minValue);
  BTSerial.print("Max: ");
  BTSerial.println(maxValue);
  BTSerial.print("Power: ");
  BTSerial.println(power);
  BTSerial.print("Fundamental Period: ");
  BTSerial.println(fundamentalPeriod);

  // Wait a bit before next reading
  delay(1000);
}

float calculatePower(long sum, int sampleCount) {
  // Placeholder function for power calculation
  float meanSignal = sum / (float)sampleCount;
  return meanSignal;
}

float calculateFundamentalPeriod(int zeroCrossingCount, unsigned long startTime, unsigned long lastZeroCrossingTime) {
  unsigned long currentTime = millis();
  if (zeroCrossingCount > 1) {
    float period = (float)(currentTime - startTime) / (zeroCrossingCount - 1);
    return period * 2; // Multiply by 2 to account for both positive and negative zero crossings
  } else {
    return 0.0; // Not enough zero crossings detected
  }
}