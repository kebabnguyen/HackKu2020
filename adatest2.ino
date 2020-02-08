#include <SPI.h>
#include <WiFi101.h>
#include <LiquidCrystal.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"



#define WLAN_SSD "KNL5"
#define WLAN_PASS "b2ff756d2d"

#define AIO_SERVER "io.adafruit.com"
#define AIO_PORT 1883

#define AIO_USERNAME "crsullivan13"
#define AIO_KEY "aio_SCEm662Ft7xph8aNJB2MVM8DTFWU"

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_PORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Subscribe printScreen = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feed/printScreen");

int status = WL_IDLE_STATUS;

void setup() {
  Serial.begin(9600);
  delay(10);
  Serial.print("Connecting to: ");
  Serial.print(WLAN_SSD);
  //WiFi.begin(WLAN_SSD, WLAN_PASS);
  while(status != WL_CONNECTED){
    WiFi.disconnect();
    Serial.println("Attempting connection");
    status = WiFi.begin(WLAN_SSD, WLAN_PASS);
    delay(3000);
  }
  Serial.println("Connection successful!");
  // allows the program to recieve data from the adafruit messages
  mqtt.subscribe(&printScreen);

  lcd.print("You can change this message with Google Assistant");

}

void MQTT_Connect(){
  int8_t connectionTester;
  Serial.print("Connecting to the MQTT protocol");
  //every itteration it will attempt to connect and then check condition
  while((connectionTester = mqtt.connect()) != 0 ){
    Serial.println("Attempting connection");
    mqtt.disconnect();
    delay(3000);
  }
  Serial.println("Connection was made!");
}

void loop() {
  lcd.scrollDisplayRight();
  MQTT_Connect();
  Adafruit_MQTT_Subscribe *subscription;
  while((subscription = mqtt.readSubscription(5000))){
    if(subscription == &printScreen){
      lcd.print((char*) printScreen.lastread);

    }

  }

}
