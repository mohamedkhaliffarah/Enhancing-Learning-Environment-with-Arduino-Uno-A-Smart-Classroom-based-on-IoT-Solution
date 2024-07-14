#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2);  // Adjust the I2C address if necessary
#define DHTPIN 8         // DHT11 sensor data pin
#define DHTTYPE DHT11    // DHT sensor type

DHT dht(DHTPIN, DHTTYPE);
#define relay 9

void setup() {
  lcd.init();
  lcd.backlight();
  Wire.begin();

  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);  // Assuming the relay is active LOW
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  float temperature = dht.readTemperature();

  Serial.print("temperature: ");
  Serial.print(temperature);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("    AUTO FAN");
  lcd.setCursor(0, 1);
  lcd.print("Temp ");
  lcd.print(temperature, 1);  // Display temperature with one decimal place
  lcd.print(" C");        // Special character for degree symbol
  delay(1000);

  if (temperature > 25.0) {
    digitalWrite(relay, HIGH);  // Assuming the relay is active LOW
    
  } else {
    digitalWrite(relay, LOW);  // Assuming the relay is active LOW
    
  }
}
