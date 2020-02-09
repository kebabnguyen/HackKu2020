#include <SPI.h>
#include <WiFi101.h>
#include <LiquidCrystal.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

//defining ssid and password for the wifi network that the arduino will connect to
#define WLAN_SSD "KNL5"
#define WLAN_PASS "b2ff756d2d"

//defining the adafruit server and port that the arduino will attempt to access
#define AIO_SERVER "io.adafruit.com"
#define AIO_PORT 1883

//adafruit user and key to access the user's feeds
#define AIO_USERNAME "crsullivan13"
#define AIO_KEY "aio_SCEm662Ft7xph8aNJB2MVM8DTFWU"

//lcd constructor -- RS EN 4 5 6 7
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
//creates client that can connect to specific ip address
WiFiClient client;

//creates the Adafruit client object that uses the mqtt protocol to tansfer telemetric data with adafruit io
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_PORT, AIO_USERNAME, AIO_KEY);

//this creates a subscribe object that is subscribe to the printScreen feed on adafruit io, which allows the arduino to interface with the
//data being transferred from the GA to IFTTT, which is linked to adafruit io allowing for the data to go the arduino
Adafruit_MQTT_Subscribe printScreen = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/printScreen");


int status = WL_IDLE_STATUS; //local variable to monitor the status of the wifi connection

//connects board to MQTT protocol which allows for easy telemetry data tansfer
void MQTT_Connect(){
  int8_t connectionTester;
  Serial.print("Connecting to the MQTT protocol... ");

  //every itteration it will attempt to connect and then check condition
  //connectionTester hold 8 bit int and .connect() returns 8 bit int
  //.connect() calls connectServer and if all statements are upheld then returns 0
  //sets connectionTester to the return of connect and if that doesn't equal 0 then this means the connection failed and the loop begins
  //calling connect in the while loop condition attempts a connection
  while((connectionTester = mqtt.connect()) != 0 ){
    Serial.println("Attempting connection");
    mqtt.disconnect();//to flush and excess connections that might have been made
    delay(3000);
  }
  Serial.println("Connection was made!");
}

//connects arduino board to the wifi
void WIFI_connect() {
  Serial.print("Connecting to: ");
  Serial.print(WLAN_SSD);
  while(status != WL_CONNECTED){ //while the ard. isn't connected to the wifi,
    WiFi.disconnect(); //disconnects to make sure each attempt is a new one
    Serial.println("Attempting connection");
    status = WiFi.begin(WLAN_SSD, WLAN_PASS); //begin function returns two values: WL_CONNECTED( it's connected) or WL_IDLE_STATUS (not connected)
    delay(3000);
  }
  Serial.println("Connection successful!");
}

void setup() {
  lcd.begin(16,1); //starts the lcd screen, it will have 81 chars in the first upper line
  lcd.clear(); //clears the screen and sets the cursor to the top left
  Serial.begin(9600);
  delay(10);
  lcd.print("Change via G.A.!");
  WIFI_connect();
  // allows the program to recieve data from the adafruit messages
  mqtt.subscribe(&printScreen);
  MQTT_Connect();
}


void loop() {
  //this is the loop function for the arduino, used to get the data from the IFTTT/adafruti io hub and display it as text on the LCD screen

  //creates subscription pointer
  Adafruit_MQTT_Subscribe *subscription;

  //while there is something that can be read it is true and will run the loop
  while((subscription = mqtt.readSubscription(500))){
    //if subscription has the same path as printScreen then it is true
    if(subscription == &printScreen){
      lcd.clear();//clears the screen
      lcd.print((char*) printScreen.lastread);//prints the last read data from the feed onto the lcd screen
    }
  }
  lcd.scrollDisplayRight();//scrolls the text
}
