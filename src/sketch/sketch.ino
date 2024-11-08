#include <Wire.h>
#include <Adafruit_INA219.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Define the OLED display dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Create an instance of the display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); // -1 for no reset pin


// Create an INA219 instance
Adafruit_INA219 ina219;

void setup() {

  // Initialize the display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Address 0x3C for 128x64
  display.clearDisplay();                       // Clear the buffer

  // Set text color
  display.setTextColor(SSD1306_WHITE);
  
  Serial.begin(115200);
  // Initialize the INA219
  if (!ina219.begin()) {
    Serial.println("Could not find a valid INA219 sensor, check wiring!");
    while (1);
  }
}

void loop() {
  // Read voltage (in volts)
  float voltage = ina219.getBusVoltage_V();
  // Read current (in milliamps)
  float current = ina219.getCurrent_mA();
  
  // Read power (in milliwatts)
  float power = ina219.getPower_mW();
  
  // Print the results to the Serial Monitor
  Serial.print("Bus Voltage: ");
  Serial.print(voltage);
  Serial.println(" V");
  
  Serial.print("Current: ");
  Serial.print(current);
  Serial.println(" mA");
  
  Serial.print("Power: ");
  Serial.print(power);
  Serial.println(" mW");

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

  // Wait 1 second before the next reading
  delay(1000);
}
