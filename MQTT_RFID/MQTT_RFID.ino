#include <SPI.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <MFRC522.h>
#include "arduino_secrets.h"

#define READER_NO 2
#define SS_PIN_1 D2
#define SS_PIN_2 D3
#define RST_PIN 5

/*
Silkscreen Pin to GPIO number for WeMos D1 R1
  static const uint8_t D0   = 3;
  static const uint8_t D1   = 1;
  static const uint8_t D2   = 16;
  static const uint8_t D3   = 5;
  static const uint8_t D4   = 4;
  static const uint8_t D5   = 14;
  static const uint8_t D6   = 12;
  static const uint8_t D7   = 13;
  static const uint8_t D8   = 0;
  static const uint8_t D9   = 2;
  static const uint8_t D10  = 15;
  static const uint8_t D11  = 13;
  static const uint8_t D12  = 12;
  static const uint8_t D13  = 14;
  static const uint8_t D14  = 4;
  static const uint8_t D15  = 5;
*/



// WiFi settings
const char* ssid = SECRET_SSID;
const char* password = SECRET_PASSWORD;

WiFiClient wifiClient;
PubSubClient pubSubClient;
MFRC522 rfid_reader[READER_NO];
byte ssPins[] = {SS_PIN_1, SS_PIN_2};



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

void setupRFID(){
  for (uint8_t reader = 0; reader < READER_NO; reader++) {
    rfid_reader[reader].PCD_Init(ssPins[reader], RST_PIN);
    rfid_reader[reader].PCD_DumpVersionToSerial();
  }
  Serial.println("RFID FINISH");
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
  setupRFID();
}

void loop(){
  for(byte i=0 ; i < READER_NO; i++){
    if(rfid_reader[i].PICC_ReadCardSerial() && rfid_reader[i].PICC_IsNewCardPresent()){
      Serial.print("Card No: ");
      Serial.print(i);
      Serial.println("Card UID: ");
      dump_byte_array(rfid_reader[i].uid.uidByte, rfid_reader[i].uid.size);
      Serial.println("---------");
    }
  }

  if(pubSubClient.connected()){
    // pubSubClient.publish("test", "test message from arduino looped");
    // Serial.println("Test Message sent");
    // delay(5);
  }else{
    // Serial.println("Not Connected in Loop");
    // delay(6);
  }
}
void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

