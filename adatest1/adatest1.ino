/***************************************************
  Adafruit MQTT Library ESP8266 Adafruit IO Anonymous Time Query

  Must use the latest version of ESP8266 Arduino from:
    https://github.com/esp8266/Arduino

  Works great with Adafruit's Huzzah ESP board & Feather
  ----> https://www.adafruit.com/product/2471
  ----> https://www.adafruit.com/products/2821

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
#include <SPI.h>
#include <WiFi101.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* WiFi Access Point *********************************/
#define WLAN_SSID       "KNL5"
#define WLAN_PASS       "b2ff756d2d"

/************************* Adafruit.io Setup *********************************/
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883

#define AIO_USERNAME    "kanguyen4060"
#define AIO_KEY         "aio_xQsu79yjCjqtHqnaJga3gBYaja4a"

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Subscribe LED = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/LED");


void setup() {

  Serial.begin(9600);
  delay(10);
  pinMode(6,OUTPUT);
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    
    Serial.print(F("."));
    WiFi.disconnect();
    delay(3000);
  }
  Serial.println(" WiFi connected.");

  mqtt.subscribe(&LED);

}

void MQTT_connect() {
  int8_t ret;
  Serial.print("Connecting to MQTT... ");
  while((ret = mqtt.connect()) != 0) {
    Serial.println(mqtt.connectErrorString(ret));
    mqtt.disconnect();
    delay(3000);
  }
  Serial.println("you connec");
}
void loop() {

  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    Serial.println("in while");
    if(subscription == &LED) {
      Serial.print(F("Got: "));
      Serial.println((char *)LED.lastread);
      if (strcmp((char*) LED.lastread, "ON") == 0 ) {
        digitalWrite(6,HIGH);
      }
      else {
        digitalWrite(6,LOW);
      }
    }
  }


}
