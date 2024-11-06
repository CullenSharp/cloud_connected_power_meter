// These libraries enable the Esp32 to connect to wifi.
// Connect to MQTT protocol.
// Communicate with INA219 sensor
#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
// Insert the type of OLED display with a '.h' following it.
//#include <"your led component">
#include <Adafruit_INA219.h>
// This library holds the AdafruitIO and the Wifi credentials.
#include<Wifi_config.h>
// This library measures the Current and Power.
#include<Measure.h>



const char POWER_FEED[] = IO_USERNAME "/feeds/power";
Adafruit_MQTT_Publish power = Adafruit_MQTT_Publish(&mqtt, POWER_FEED);




//Oled Screen setup below
// here....
//Variables for measurements.
float current_mA;
float power_mW;


void setup() {
  Serial.begin(115200);
  ina219.begin();
  ina219.setCalibration_16V_400mA();

  Serial.println();
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println();
  Serial.print("WiFi Connected to ");
  Serial.print(WIFI_SSID);
  Serial.println("IP address: ");
  Serial.print(WiFi.localIP());

  //Initiate OLED display here.
  //Display welcome message

  //LED
  // pinMode(14, OUTPUT);

  connect();
}

void loop() {
  // ping Adafruit IO a few times to make sure we remain connected.
  if(! mqtt.ping(3)){
    // Reconnect to Adafruit IO
    if(! mqtt.connected())
      connect();
  }

  //Turn Led off here.
  //digitalWrite(14, LOW);

  current_mA = measureCurrent();
  power_mW = measurePower();

  //Publish Data.
  if(! power.publish(power_mW)){
    Serial.println(F("Failed"));
  } else{
    Serial.println(F("OK!"));
  }

  //Display Data on OLED.
  //LED on.


}
