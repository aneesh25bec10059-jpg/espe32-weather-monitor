#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// Screen dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Pin Definitions
#define DHTPIN 15
#define DHTTYPE DHT22
#define ALERT_LED 2

// Thresholds
#define TEMP_THRESHOLD 30.0 // Celsius

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  
  pinMode(ALERT_LED, OUTPUT);
  digitalWrite(ALERT_LED, LOW);

  dht.begin();

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(15, 25);
  display.println("Initializing...");
  display.display();
  delay(2000);
}

void loop() {
  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Check if reading failed
  if (isnan(temp) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print to Serial Monitor
  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print(" *C | Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Alert system: Turn LED ON if temperature exceeds threshold
  if (temp > TEMP_THRESHOLD) {
    digitalWrite(ALERT_LED, HIGH);
  } else {
    digitalWrite(ALERT_LED, LOW);
  }

  // Update OLED Display
  display.clearDisplay();
  
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("WEATHER MONITOR");
  display.drawLine(0, 10, 128, 10, WHITE);

  display.setTextSize(1);
  display.setCursor(0, 20);
  display.print("Temp: ");
  display.setTextSize(2);
  display.setCursor(0, 32);
  display.print(temp, 1);
  display.print(" C");

  display.setTextSize(1);
  display.setCursor(70, 20);
  display.print("Humidity:");
  display.setTextSize(2);
  display.setCursor(70, 32);
  display.print((int)humidity);
  display.print("%");

  if (temp > TEMP_THRESHOLD) {
    display.setTextSize(1);
    display.setCursor(0, 54);
    display.print("STATUS: HIGH TEMP!");
  } else {
    display.setTextSize(1);
    display.setCursor(0, 54);
    display.print("STATUS: NORMAL");
  }

  display.display();
  delay(2000); // Refresh every 2 seconds
}