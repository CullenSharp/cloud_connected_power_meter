#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include "config.h"
#include "AdafruitIO_WiFi.h"

AdafruitIO_Feed *digital = io.feed("digital");

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

void setup() {
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


  // Connection Sucessful
  Serial.println(io.statusText());
}

void loop() {
  
}