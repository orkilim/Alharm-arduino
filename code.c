#include <ArduinoBLE.h>

const int ledPin = LED_BUILTIN; // set ledPin to on-board LED
const int buttonPin = 2; // set buttonPin to digital pin 4

BLEService ledService("12345678-0000-1000-8000-12345678ABCD"); // create service

//original  19B10010-E8F2-537E-4F6C-D104768A1214 

// create switch characteristic and allow remote device to read and write
BLEIntCharacteristic ledCharacteristic("87654321-0000-1000-8000-DCBA87654321", BLERead);

//orignal 19B10011-E8F2-537E-4F6C-D104768A1214

// create button characteristic and allow remote device to get notifications

void ConnectHandler(BLEDevice central) {
  // central connected event handler
  Serial.print("Connected event, central: ");
  Serial.println(central.address());
  BLE.advertise();
}

void DisconnectHandler(BLEDevice central) {
  // central disconnected event handler
  Serial.print("Disconnected event, central: ");
  Serial.println(central.address());
  BLE.advertise();
}

boolean sentAlready=false;
int counter=0;
int doSend=false;


void setup() {
  Serial.begin(9600);
  //while (!Serial);

  pinMode(ledPin, OUTPUT); // use the LED as an output
  pinMode(buttonPin, INPUT); // use button pin as an input

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

   BLE.setEventHandler(BLEConnected, ConnectHandler);
    BLE.setEventHandler(BLEDisconnected, DisconnectHandler);

  // set the local name peripheral advertises
  BLE.setLocalName("Alharm");
  // set the UUID for the service this peripheral advertises:
  BLE.setAdvertisedService(ledService);

  // add the characteristics to the service
  ledService.addCharacteristic(ledCharacteristic);

  // add the service
  BLE.addService(ledService);

  ledCharacteristic.writeValue((byte)123);
  

  // start advertising
  BLE.advertise();

  Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {
  // poll for BLE events
  BLE.poll();

  // read the current button pin state
  char buttonValue = digitalRead(buttonPin);

  // has the value changed since the last read
  boolean buttonChanged = (ledCharacteristic.value() != buttonValue);
  if (buttonChanged) {
    // button state changed, update characteristics
    ledCharacteristic.writeValue(buttonValue);
    
  }
  
  while (digitalRead(buttonPin)==0) {
      if(counter<2)
      {
        doSend=true;
      }
      else{
        doSend=false;
      }
      Serial.println(counter);
      counter++;
      delay(1000);
  }
  
  counter=0;
  
  if(doSend){
      ledCharacteristic.writeValue((byte)90);
      if(sentAlready==false){
        Serial.println(ledCharacteristic);
        sentAlready=true;
    }
    else{
      //ledCharacteristic.writeValue((byte)10);
      sentAlready=false;
    }
  }  
    //counter=0;
    doSend=false;
  
  
}


