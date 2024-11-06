

#define IO_USERNAME ""
#define IO_KEY ""
#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 1883

// These hold the Wifi credentials we will be using.
#define WIFI_SSID ""
#define WIFI_PASS ""

WiFiClient client;

const char MQTT_SERVER[] = AIO_SERVER;
const char MQTT_CLIENTID[] = IO_KEY "-" __DATE__ "-" __TIME__;
const char MQTT_USERNAME[] = IO_USERNAME;
const char MQTT_PASSWORD[] = IO_KEY;

Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, AIO_SERVERPORT, MQTT_CLIENTID, MQTT_USERNAME, MQTT_PASSWORD);


// Connect to Adafruit IO via MQTT.
void connect(){
  
  Serial.println(F("Connecting to Adafruit IO..."));
  int8_t ret;
  while ((ret = mqtt.connect()) != 0){
    switch (ret){
      case 1: Serial.println(F("Wrong protocol.")); break;
      case 2: Serial.println(F("ID rejected.")); break;
      case 3: Serial.println(F("Server Unavailable.")); break;
      case 4: Serial.println(F("Wrong UserName/Password.")); break;
      case 5: Serial.println(F("Not authorized.")); break;
      case 6: Serial.println(F("Failed to subscribe")); break;
      default: Serial.println(F("Connection Failed")); break;
    }
    if (ret > 0){
      mqtt.disconnect();
      Serial.println(F("Retrying connection..."));
      delay(5000);
    }
  }
  Serial.println(F("Adafruit IO Connected!"));
}

