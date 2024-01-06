//Example created for ESP32
#include "HLK-LD2450.h"

// Definition of pins to which the sensor is connected
#define HLK_RX 21
#define HLK_TX 22

HLK_LD2450 hlk; // Declaration of a new object representing the sensor

void setup()
{
  Serial.begin(115200);
  if(!hlk.connect(256000, HLK_TX, HLK_RX)){ // Establishing a connection with the sensor
    Serial.print("ERROR: Invalid EspSoftwareSerial pin configuration");
    while (1) { // Don't continue with invalid configuration
      delay (1000);
    }
  } 
}

void loop()
{
  if(hlk.refreshRadarData()){ // Retrieving new data from the sensor
    // Displaying previously retrieved data of the first target
    Serial.print("Target 1: X: ");
    Serial.print(hlk.getTarget1X());
    Serial.print("mm, Y: ");
    Serial.print(hlk.getTarget1Y());
    Serial.print("mm, Speed: ");
    Serial.print(hlk.getTarget1Speed());
    Serial.print("cm/s \n");
  }
  else{
    Serial.println("Sensor is unavailable");
    delay(1000);
  }
  delay(100); // Waiting 100ms before the next data retrieval
}


