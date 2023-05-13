
#include <SPI.h>  // enabled Bydefault for some LoRa modules
#include <LoRa.h>
#include "BluetoothSerial.h"

//define the pins used by the transceiver module
#define SCK 18
#define MISO 19
#define MOSI 23
#define SS 5
#define RST 14
#define DIO0 2

BluetoothSerial SerialBT;  // make an object from the class

void setup() {
  //initialize Serial Monitor
  Serial.begin(9600);
  while (!Serial);
  Serial.println("LoRa User 1");
  SerialBT.begin("User 1");
  //setup LoRa transceiver module
  LoRa.setPins(SS, RST, DIO0);
  
  while (!LoRa.begin(433E6)) {  // 433MHz works for Asia 
    Serial.println(".");
    delay(500);
  }
   
  LoRa.setSyncWord(0xF3); // match the Receiver and Sender
  Serial.println("LoRa Initializing OK!");
}

void loop() {

// Recieveing the data
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) {
      String LoRaData = LoRa.readString();
      Serial.println(LoRaData); 
      SerialBT.print(LoRaData);
      SerialBT.println();
    }

  }

  // Sending the data
  if(SerialBT.available()){
    String d = SerialBT.readString();
    Serial.print("Sending packet: ");
    Serial.print(d);
    Serial.print(" : ");
    Serial.println(d);
    //Send LoRa packet to receiver
    LoRa.beginPacket();
    LoRa.print(d);
    LoRa.endPacket();
    delay(2000);  // make sure the delay is same
  }
}
