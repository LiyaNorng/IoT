/* 
 *  
 *  Liya Norng
 *  6/26/21
  Sketch generated by the Arduino IoT Cloud Thing "Untitled"
  https://create.arduino.cc/cloud/things/a07813e5-aac0-41d7-958f-7c2c802c5be2 

  Arduino IoT Cloud Variables description

  The following variables are automatically generated and updated when changes are made to the Thing

  CloudSwitch supraBoiGarage;

  Variables which are marked as READ/WRITE in the Cloud Thing will also have functions
  which are called when their values are changed from the Dashboard.
  These functions are generated with the Thing and added at the end of this sketch.
*/
#include "arduino_secrets.h"
#include "thingProperties.h"
#include <DHT.h> // library for DHT temp


#define DHTPIN 2
DHT dht(DHTPIN, DHT11);

#define RelayPin1 5 // D1
#define wifiled 16 // D0
#define SwitchPin1 0 // D3
int toggleState_1 = 0; // Define integer to remember the toggle state for relay 1

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  
  delay(1500); 
  dht.begin();
  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  
  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
 */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
  
  pinMode(RelayPin1, OUTPUT);
  pinMode(wifiled, OUTPUT);
  pinMode(SwitchPin1, INPUT_PULLUP);

  // start up
  digitalWrite(RelayPin1, HIGH);
  digitalWrite(wifiled, HIGH); // turn off wifi LED
}

void manuel_control(){
  if(digitalRead(SwitchPin1) == LOW){
    delay(200);
    relayOnOff(1);
  }
}

void relayOnOff(int relay){
  switch(relay){
    case 1:
      if(toggleState_1 == 0){
        digitalWrite(RelayPin1, LOW); // turn on relay 1
        toggleState_1 = 1;
        Serial.println("Device 1 ON ---> relay");
      }
      else{
        digitalWrite(RelayPin1, HIGH); // turn off relay 1
        toggleState_1 = 0;
        Serial.println("Device 1 OFF ---> relay");
      }
      delay(200);
      break;
    default:
      break;
  }
}

void loop() {
  ArduinoCloud.update();
  // Your code here 
  delay(200);
   manuel_control();
  readHumidit();
  readTemperatur();
   if(WiFi.status() != WL_CONNECTED){
    digitalWrite(wifiled, HIGH);
   }
   else{
    digitalWrite(wifiled, LOW);
    //Serial.println("connected");
   } 
}

void readHumidit(){
  float h = dht.readHumidity();
  if(isnan(h)){
     Serial.println("Failed to read from DHT sensor!");
     return;
  }
   Serial.print("Reading humidity!  ");
   Serial.println(h);
  humid = h;
}

void readTemperatur(){
  float t = dht.readTemperature();
  if(isnan(t)){
      Serial.println("Failed to read from DHT sensor!");
      return;
  }
  Serial.print("Reading temperature!  ");
  Serial.println(t);
  temp = t;
}

void onSupraBoiGarageChange() {
  // Do something

  if(supraBoiGarage == 1){
    digitalWrite(RelayPin1, LOW);
    Serial.println("Device 1 ON ---> cloud");
    toggleState_1 = 1;
    delay(200);
  }
  else{
    digitalWrite(RelayPin1, HIGH);
    Serial.println("Device 1 OFF ---> cloud");
    toggleState_1 = 0;
    delay(200);
  }
}
