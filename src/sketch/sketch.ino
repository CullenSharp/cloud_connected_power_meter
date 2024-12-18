#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "config.h"
#include "AdafruitIO_WiFi.h"

// Define the OLED display dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Create an instance of the display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); // -1 for no reset pin


//Create Adafruit IO instances for uploading to the cloud
AdafruitIO_Feed *voltage = io.feed("Voltage");
AdafruitIO_Feed *current = io.feed("Current");
AdafruitIO_Feed *power = io.feed("Power");

// Create an INA219 instance
Adafruit_INA219 ina219;

// Gets MAC Address for the esp32, This function is needed
// If you want to communicate with PSU's IoT Network
// Head over to config.h if you want to connect to PSU IoT.
void getMACAddress()
{
  uint8_t baseMACArr[6];
  esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, baseMACArr);
  if (ret == ESP_OK){
    Serial.printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
                  baseMACArr[0], baseMACArr[1], baseMACArr[2],
                  baseMACArr[3], baseMACArr[4], baseMACArr[5]);
                  
  }
  else{
    Serial.println("... I couldn't read it...");
  }
}

void send_data_to_screen(float power){

  //get updated power value each time
  power = ina219.getPower_mW();
  
  //clear display
  display.clearDisplay();
  
  // Set text size
  display.setTextSize(1);
  
  // Display the first line of text
  display.setCursor(0, 0); // Column 0, Row 0
  display.println("Power Consumption:\n");

  // Display the second line of text
  display.setCursor(0, 32); // Column 0, Row 32
  display.println(power);
  display.println("milliWatts");

  // Send the buffer to the display
  display.display();

  delay(100);
}

void setup() {

  // Initialize the display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Address 0x3C for 128x64
  display.clearDisplay();                       // Clear the buffer

  // Set text color
  display.setTextColor(SSD1306_WHITE);
  
  Serial.begin(115200);

  // Wait for serial monitor to open
  while(! Serial);
  Serial.println("Hello World!");

  // Initialize Wifi Service
  WiFi.mode(WIFI_STA);
  WiFi.begin();

  // Get the MAC Address. This should run regardless if you have internet or not...
  Serial.print("Your ESP32's MAC address is: ");
  getMACAddress();

  // Connect to Adafruit IO
  Serial.print("Attempting Connection with Adafruit IO");
  io.connect();

  // Check if connection was successful
  while(io.status() < AIO_CONNECTED){
    Serial.print(".");
    delay(500);
  }

  // Initialize INA219
  if (!ina219.begin()){
    Serial.println("Unable to Initialize INA 219 Library");
  while(1);
  }

  // Connection Successful
  Serial.println(io.statusText());
}

void loop() {
  // Fetch values from the INA219
  float voltageValue = ina219.getBusVoltage_V();
  float currentValue = ina219.getCurrent_mA();
  float powerValue = ina219.getPower_mW();

  // Print the results to the Serial Monitor
  //Serial.print("Bus Voltage: ");
  //Serial.print(voltageValue);
  //Serial.println(" V");

  //Serial.print("Current: ");
  //Serial.print(currentValue);
  //Serial.println(" mA");

  Serial.print("Power: ");
  Serial.print(powerValue);
  Serial.println(" mW");

  for (int i = 0; i < 20; ++i){
    send_data_to_screen(powerValue);
  }

  // Send the results to Adafruit IO cloud
  io.run();

  // Save the readings to the Adafruit IO feeds 0
  Serial.println("");
  //voltage->save(String(voltageValue));
  //current->save(String(currentValue));
  power->save(String(powerValue));

  //delay(2000); // Wait for 2 seconds before the next reading (reduces throttle)
}
