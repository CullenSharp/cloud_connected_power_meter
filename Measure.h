
//Initializez the INA219 sensor.
Adafruit_INA219 ina219;

float measureCurrent(){
  float shuntvoltage = ina219.getShuntVoltage_mV();
  float busvoltage = ina219.getBusVoltage_V();
  float current_mA = ina219.getCurrent_mA();
  float loadvoltage = busvoltage + (shuntvoltage / 1000);

  Serial.println("Bus Voltage: ");
  Serial.print(busvoltage);
  Serial.print("V.");

  Serial.println("Shunt Voltage: ");
  Serial.print(shuntvoltage);
  Serial.print("V.");

  Serial.println("Load Voltage: ");
  Serial.print(loadvoltage);
  Serial.print("V.");

  Serial.println("Current: ");
  Serial.print(current_mA);
  Serial.print("mA.");

  if (current_mA < 0){
    current_mA = 0.0;
  } return current_mA;
}

float measurePower(){
  float shuntvoltage = ina219.getShuntVoltage_mV();
  float busvoltage = ina219.getBusVoltage_V();
  float current_mA = ina219.getCurrent_mA();
  float loadvoltage = busvoltage + (shuntvoltage / 1000);

  if(current_mA < 0){
    current_mA = 0.0;
  }return current_mA * loadvoltage;
}