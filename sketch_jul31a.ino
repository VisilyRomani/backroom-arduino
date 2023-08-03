#include <SPI.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "arduino_secrets.h"


// WiFi settings
const char* ssid = SECRET_SSID;
const char* password = SECRET_PASSWORD;

WiFiClient wifiClient;
PubSubClient pubSubClient;

void setupWiFi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setupMQTT(){
  pubSubClient.setClient(wifiClient);
  pubSubClient.setServer(SECRET_BROKER_IP,1883);
  if(pubSubClient.connect("arduino-1")){
    Serial.println("Connected");
  }else{
    pubSubClient.state();
    Serial.println("Error Connecting");
  }
}

void setup() {
  Serial.begin(9600);
  SPI.begin();
  setupWiFi();
  setupMQTT();
}

void loop(){
  
  // if(pubSubClient.connected()){
  //   pubSubClient.publish("test", "test message from arduino looped");
  //   Serial.println("Test Message sent");
  //   delay(5);
  // }else{
  //   Serial.println("Not Connected in Loop");
  //   delay(6);
  // }
}
