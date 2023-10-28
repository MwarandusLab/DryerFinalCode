#include <LiquidCrystal_I2C.h>
#include <Adafruit_Sensor.h>
#include <RTClib.h>
#include <DHT_U.h>
#include <Wire.h>
#include <SPI.h>
#include <DHT.h>
#include <SD.h>


#define T1 9 // Digital pin connected to the DHT sensor 
#define T2 10 // Digital pin connected to the DHT sensor 
#define T3 8 // Digital pin connected to the DHT sensor 
#define B1 6 // Digital pin connected to the DHT sensor 
#define B2 5 // Digital pin connected to the DHT sensor 
#define B3 7 // Digital pin connected to the DHT sensor 

#define DHTTYPE DHT22

DHT_Unified dht_T1(T1, DHTTYPE);
DHT_Unified dht_T2(T2, DHTTYPE);
DHT_Unified dht_T3(T3, DHTTYPE);

DHT_Unified dht_B1(B1, DHTTYPE);
DHT_Unified dht_B2(B2, DHTTYPE);
DHT_Unified dht_B3(B3, DHTTYPE); 

RTC_DS3231 rtc;

const int chipSelect = 4;  // Replace with the appropriate chip select pin for SD card module

float temperature;
float humidity;

LiquidCrystal_I2C lcd_A(0x26, 16, 4);
LiquidCrystal_I2C lcd_B(0x27, 16, 4);

uint32_t delayMS;

void setup() {
  lcd_A.init();
  lcd_A.backlight();

  lcd_B.begin(16, 4);
  lcd_B.backlight();

  rtc.begin();

  dht_T1.begin();
  dht_T2.begin();
  dht_T3.begin();

  dht_B1.begin();
  dht_B2.begin();
  dht_B3.begin();

  lcd_A.clear();
  lcd_A.setCursor(6, 1);
  lcd_A.print("SYSTEM");
  lcd_A.setCursor(2, 2);
  lcd_A.print("INITIALIZATION");
  delay(1000);

  lcd_B.clear();
  lcd_B.setCursor(6, 1);
  lcd_B.print("SYSTEM");
  lcd_B.setCursor(2, 2);
  lcd_B.print("INITIALIZATION");
  delay(1000);

  if (!SD.begin(chipSelect)) {
    lcd_A.clear();
    lcd_A.setCursor(7, 1);
    lcd_A.print("SD CARD");
    lcd_A.setCursor(3, 2);
    lcd_A.print("INITIALIZATION");
    lcd_A.setCursor(7, 3);
    lcd_A.print("FAILED");
    lcd_B.clear();
    lcd_B.setCursor(7, 1);
    lcd_B.print("SD CARD");
    lcd_B.setCursor(3, 2);
    lcd_B.print("INITIALIZATION");
    lcd_B.setCursor(7, 3);
    lcd_B.print("FAILED");
    //return;
  }
  // Create a file to store the data
  File dataFile = SD.open("data.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.println("Temperature (°C)\tHumidity (%)\tTime");
    dataFile.close();
  } else {
    lcd_A.clear();
    lcd_A.setCursor(4,1);
    lcd_A.print("ERROR OPENING");
    lcd_A.setCursor(5, 2);
    lcd_A.print("DATA FILE!");
    delay(1000);
  }
  sensor_t sensor;
  dht_T1.temperature().getSensor(&sensor);
  dht_T2.temperature().getSensor(&sensor);
  dht_T2.temperature().getSensor(&sensor);

  dht_B1.temperature().getSensor(&sensor);
  dht_B2.temperature().getSensor(&sensor);
  dht_B3.temperature().getSensor(&sensor);

  dht_T1.humidity().getSensor(&sensor);
  dht_T2.humidity().getSensor(&sensor);
  dht_T3.humidity().getSensor(&sensor);

  dht_B1.humidity().getSensor(&sensor);
  dht_B2.humidity().getSensor(&sensor);
  dht_B3.humidity().getSensor(&sensor);

  delayMS = sensor.min_delay / 1000;
  
}

void loop() {
  delay(delayMS);

  sensors_event_t event_T1;
  sensors_event_t event_T2;
  sensors_event_t event_T3;

  dht_T1.temperature().getEvent(&event_T1);
  dht_T2.temperature().getEvent(&event_T2);
  dht_T3.temperature().getEvent(&event_T3);

  if((!isnan(event_T1.temperature)) || (!isnan(event_T2.temperature)) || (!isnan(event_T3.temperature)) || (!isnan(event_T1.relative_humidity)) || (!isnan(event_T2.relative_humidity)) || (!isnan(event_T3.relative_humidity))){
    lcd_A.clear();
    lcd_A.setCursor(0, 1);
    lcd_A.print("T1: ");
    lcd_A.setCursor(4, 1);
    lcd_A.print(event_T1.temperature, 1);
    lcd_A.setCursor(9, 1);
    lcd_A.print("C");
    lcd_A.setCursor(0, 2);
    lcd_A.print("T2: ");
    lcd_A.setCursor(4, 2);
    lcd_A.print(event_T2.temperature, 1);
    lcd_A.setCursor(9, 2);
    lcd_A.print("C");
    lcd_A.setCursor(0, 3);
    lcd_A.print("T3: ");
    lcd_A.setCursor(4, 3);
    lcd_A.print(event_T3.temperature, 1);
    lcd_A.setCursor(9, 3);
    lcd_A.print("C");

    dht_T1.humidity().getEvent(&event_T1);
    dht_T2.humidity().getEvent(&event_T2);
    dht_T3.humidity().getEvent(&event_T3);

    lcd_A.setCursor(12, 1);
    lcd_A.print(event_T1.relative_humidity, 1);
    lcd_A.setCursor(17, 1);
    lcd_A.print("%");
    lcd_A.setCursor(12, 2);
    lcd_A.print(event_T2.relative_humidity, 1);
    lcd_A.setCursor(17, 2);
    lcd_A.print("%");
    lcd_A.setCursor(12, 3);
    lcd_A.print(event_T3.relative_humidity, 1);
    lcd_A.setCursor(17, 3);
    lcd_A.print("%");
    delay(1000);
  }

  sensors_event_t event_B1;
  sensors_event_t event_B2;
  sensors_event_t event_B3;

  dht_B1.humidity().getEvent(&event_B1);
  dht_B2.humidity().getEvent(&event_B2);
  dht_B3.humidity().getEvent(&event_B3);

  if((!isnan(event_B1.temperature)) || (!isnan(event_B2.temperature)) || (!isnan(event_B3.temperature)) || (!isnan(event_B1.relative_humidity)) || (!isnan(event_B2.relative_humidity)) || (!isnan(event_B3.relative_humidity))){
    lcd_B.clear();
    lcd_B.setCursor(0, 1);
    lcd_B.print("B1: ");
    lcd_B.setCursor(4, 1);
    lcd_B.print(event_B1.temperature, 1);
    lcd_B.setCursor(9, 1);
    lcd_B.print("C");
    lcd_B.setCursor(0, 2);
    lcd_B.print("B2: ");
    lcd_B.setCursor(4, 2);
    lcd_B.print(event_B2.temperature, 1);
    lcd_B.setCursor(9, 2);
    lcd_B.print("C");
    lcd_B.setCursor(0, 3);
    lcd_B.print("B3: ");
    lcd_B.setCursor(4, 3);
    lcd_B.print(event_B3.temperature, 1);
    lcd_B.setCursor(9, 3);
    lcd_B.print("C");

    lcd_B.setCursor(12, 1);
    lcd_B.print(event_B1.relative_humidity, 1);
    lcd_B.setCursor(17, 1);
    lcd_B.print("%");
    lcd_B.setCursor(12, 2);
    lcd_B.print(event_B2.relative_humidity, 1);
    lcd_B.setCursor(17, 2);
    lcd_B.print("%");
    lcd_B.setCursor(12, 3);
    lcd_B.print(event_B3.relative_humidity, 1);
    lcd_B.setCursor(17, 3);
    lcd_B.print("%");
    delay(1000);
  }

  float newTemperature_T1;
  float newHumidity_T1;

  newTemperature_T1 = event_T1.temperature;
  newHumidity_T1 = event_T1.relative_humidity;

  // Check if there is a change in temperature by 1 degree or humidity by 1%
  if (abs(newHumidity_T1 - event_T1.relative_humidity) >= 2.0 || abs(newTemperature_T1 - event_T1.temperature) >= 2.0) {
    event_T1.temperature = newTemperature_T1;
    event_T1.relative_humidity = newHumidity_T1;
    // Get the current time from the RTC module
    DateTime now = rtc.now();
    String timestamp = String(now.year()) + "-" + String(now.month()) + "-" + String(now.day()) + " " + String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());

    // Append the data to the data file on the SD card
    File dataFile = SD.open("data.txt", FILE_WRITE);
    if (dataFile) {
      dataFile.print("Sensor_T1 - Temperature_T1: ");
      dataFile.print(event_T1.temperature, 1);
      dataFile.print(" °C, Humidity_T1: ");
      dataFile.print(event_T1.relative_humidity, 1);
      dataFile.println(" %");
      dataFile.print("Time: ");
      dataFile.println(timestamp);

      dataFile.close();
      //Serial.println("Data written to SD card!");
    } else {
      //Serial.println("Error opening data file!");
    }
  }

  float newTemperature_T2;
  float newHumidity_T2;

  newTemperature_T2 = event_T2.temperature;
  newHumidity_T2 = event_T2.relative_humidity;

  if (abs(newTemperature_T2 - event_T2.temperature) >= 2.0 || abs(newHumidity_T2 - event_T2.relative_humidity) >= 2.0) {
    event_T2.temperature = newTemperature_T2;
    event_T2.relative_humidity = newHumidity_T2;

    DateTime now = rtc.now();
    String timestamp = String(now.year()) + "-" + String(now.month()) + "-" + String(now.day()) + " " + String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());

    // Append the data to the data file on the SD card
    File dataFile = SD.open("data.txt", FILE_WRITE);
    if (dataFile) {
      dataFile.print("Sensor_T1 - Temperature_T1: ");
      dataFile.print(event_T2.temperature, 1);
      dataFile.print(" °C, Humidity_T1: ");
      dataFile.print(event_T2.relative_humidity, 1);
      dataFile.println(" %");
      dataFile.print("Time: ");
      dataFile.println(timestamp);

      dataFile.close();
      //Serial.println("Data written to SD card!");
    } else {
      //Serial.println("Error opening data file!");
    }
  }

  
  float newTemperature_T3;
  float newHumidity_T3;

  newTemperature_T3 = event_T3.temperature;
  newHumidity_T3 = event_T3.relative_humidity;

  if (abs(newTemperature_T3 - event_T3.temperature) >= 2.0 || abs(newHumidity_T3 - event_T3.relative_humidity) >= 2.0) {
    event_T3.temperature = newTemperature_T3;
    event_T3.relative_humidity = newHumidity_T3;
    DateTime now = rtc.now();
    String timestamp = String(now.year()) + "-" + String(now.month()) + "-" + String(now.day()) + " " + String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());

    // Append the data to the data file on the SD card
    File dataFile = SD.open("data.txt", FILE_WRITE);
    if (dataFile) {
      dataFile.print("Sensor_T1 - Temperature_T1: ");
      dataFile.print(event_T3.temperature, 1);
      dataFile.print(" °C, Humidity_T1: ");
      dataFile.print(event_T3.relative_humidity, 1);
      dataFile.println(" %");
      dataFile.print("Time: ");
      dataFile.println(timestamp);

      dataFile.close();
      //Serial.println("Data written to SD card!");
    } else {
      //Serial.println("Error opening data file!");
    }
  }

  float newTemperature_B1;
  float newHumidity_B1;
  
  newTemperature_B1 = event_B1.temperature;
  newHumidity_B1 = event_B1.relative_humidity;

  if (abs(newTemperature_B1 - event_B1.temperature) >= 2.0 || abs(newHumidity_B1 - event_B1.relative_humidity) >= 2.0) {
    event_B1.temperature = newTemperature_B1;
    event_B1.relative_humidity = newHumidity_B1;
    DateTime now = rtc.now();
    String timestamp = String(now.year()) + "-" + String(now.month()) + "-" + String(now.day()) + " " + String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());

    // Append the data to the data file on the SD card
    File dataFile = SD.open("data.txt", FILE_WRITE);
    if (dataFile) {
      dataFile.print("Sensor_T1 - Temperature_T1: ");
      dataFile.print(event_B1.temperature, 1);
      dataFile.print(" °C, Humidity_T1: ");
      dataFile.print(event_B1.relative_humidity, 1);
      dataFile.println(" %");
      dataFile.print("Time: ");
      dataFile.println(timestamp);

      dataFile.close();
      //Serial.println("Data written to SD card!");
    } else {
      //Serial.println("Error opening data file!");
    }
  }
  float newTemperature_B2;
  float newHumidity_B2;
  
  newTemperature_B2 = event_B2.temperature;
  newHumidity_B2 = event_B2.relative_humidity;

  if (abs(newTemperature_B2 - event_B2.temperature) >= 2.0 || abs(newHumidity_B2 - event_B2.relative_humidity) >= 2.0) {
    event_B2.temperature = newTemperature_B2;
    event_B2.relative_humidity = newHumidity_B2;
    DateTime now = rtc.now();
    String timestamp = String(now.year()) + "-" + String(now.month()) + "-" + String(now.day()) + " " + String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());

    // Append the data to the data file on the SD card
    File dataFile = SD.open("data.txt", FILE_WRITE);
    if (dataFile) {
      dataFile.print("Sensor_T1 - Temperature_T1: ");
      dataFile.print(event_B2.temperature, 1);
      dataFile.print(" °C, Humidity_T1: ");
      dataFile.print(event_B2.relative_humidity, 1);
      dataFile.println(" %");
      dataFile.print("Time: ");
      dataFile.println(timestamp);

      dataFile.close();
      //Serial.println("Data written to SD card!");
    } else {
      //Serial.println("Error opening data file!");
    }
  }

  float newTemperature_B3;  
  float newHumidity_B3;

  newTemperature_B3 = event_B3.temperature;
  newHumidity_B3 = event_B3.relative_humidity;

  if (abs(newTemperature_B3 - event_B3.temperature) >= 2.0 || abs(newHumidity_B3 - event_B3.relative_humidity) >= 2.0) {
    event_B3.temperature = newTemperature_B3;
    event_B3.relative_humidity = newHumidity_B3;
    // Append the data to the data file on the SD card
    // Get the current time from the RTC module
    DateTime now = rtc.now();
    String timestamp = String(now.year()) + "-" + String(now.month()) + "-" + String(now.day()) + " " + String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());

    File dataFile = SD.open("data.txt", FILE_WRITE);
    if (dataFile) {
      dataFile.print("Sensor_B3 - Temperature_B3: ");
      dataFile.print(event_B3.temperature, 1);
      dataFile.print(" °C, Humidity_B3: ");
      dataFile.print(event_B3.relative_humidity, 1);
      dataFile.println(" %");

      dataFile.print("Time: ");
      dataFile.println(timestamp);

      dataFile.close();
      //Serial.println("Data written to SD card!");
    } else {
      //Serial.println("Error opening data file!");
    }
  }
  delay(1000);

}
