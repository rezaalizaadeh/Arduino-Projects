#include <DHT.h>
#include <SoftwareSerial.h>

// Define sensor pins
#define DHTPIN 3
#define MQ135PIN A2
#define LDRPIN A1

// Define DHT sensor type
#define DHTTYPE DHT11

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// Create a SoftwareSerial object for Bluetooth communication
SoftwareSerial bluetoothSerial(10, 11); // RX, TX

void setup() {
  // Start serial communication with PC
  Serial.begin(9600);

  // Start serial communication with Bluetooth module
  bluetoothSerial.begin(9600);

  // Initialize DHT sensor
  dht.begin();
  
  // Initialize sensor pins
  pinMode(MQ135PIN, INPUT);
  pinMode(LDRPIN, INPUT);
}

void loop() {
  // Read temperature and humidity
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Read sensor values
  int airQuality = analogRead(MQ135PIN);
  int lightIntensity = analogRead(LDRPIN);

  // Check if any reads failed
  if (isnan(temperature) || isnan(humidity)) {
    bluetoothSerial.println("Error: Failed to read from DHT sensor!");
    return;
  }

  // Format data as JSON
  String data = "{\"temperature\": " + String(temperature) +
                ", \"humidity\": " + String(humidity) +
                ", \"air_quality\": " + String(airQuality) +
                ", \"light_intensity\": " + String(lightIntensity) + "}";

  // Print data to Serial Monitor
  Serial.println(data);

  // Send data over Bluetooth
  bluetoothSerial.println(data);

  // Check for Bluetooth commands
  if (bluetoothSerial.available()) {
    char command = bluetoothSerial.read();
    handleBluetoothCommand(command);
  }

  // Delay before next reading
  delay(2000); // Adjust the delay as needed
}

void handleBluetoothCommand(char command) {
  switch (command) {
    case 'T': // Temperature
      bluetoothSerial.print("Temperature: ");
      bluetoothSerial.print(dht.readTemperature());
      bluetoothSerial.println(" °C");
      break;

    case 'H': // Humidity
      bluetoothSerial.print("Humidity: ");
      bluetoothSerial.print(dht.readHumidity());
      bluetoothSerial.println(" %");
      break;

    case 'A': // Air Quality
      bluetoothSerial.print("Air Quality: ");
      bluetoothSerial.println(analogRead(MQ135PIN));
      break;

    case 'L': // Light Intensity
      bluetoothSerial.print("Light Intensity: ");
      bluetoothSerial.println(analogRead(LDRPIN));
      break;

    default:
      bluetoothSerial.println("Unknown command");
  }
}