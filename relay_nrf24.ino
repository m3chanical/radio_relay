#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

/********** NRF24 Macros / Variables **********/
#define CE_PIN 9
#define CSN_PIN 10
const uint64_t read_pipe = 0xe8e8f0f0d1LL;  // Receive address. This will
                                            // be the same as the Transmit address
                                            // on the Pi. 
RF24 radio(CE_PIN, CSN_PIN);


/********** Relay Macros / Variables **********/
//#define RELAY_PIN_ONE   2 // Relay pin one connected to digital pin 2, etc.
//#define RELAY_PIN_TWO   3
//#define RELAY_PIN_THREE 4
//#define RELAY_PIN_FOUR  5

// ^ Not used yet.


void setup() {
  Serial.begin(57600);
  Serial.println("Relays on radio.");
  
  printf_begin();
  radio.begin();
  radio.setRetries(15, 15); // Even though this is going to be RX only
  radio.openReadingPipe(1, read_pipe);
  radio.startListening();
  radio.printDetails();
  
  /* Relay Pins */
  pinMode(2, OUTPUT);  // Relay 1
  pinMode(3, OUTPUT);  // Relay 2 ... 
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  
  /* LED Pins */
  pinMode(1, OUTPUT);  // Relay 1 LED
  pinMode(6, OUTPUT);  // Relay 2 LED
  pinMode(7, OUTPUT);  // Relay 3 LED
  pinMode(8, OUTPUT);  // Relay 4 LED
  
  /* Initialize to OFF relay state: HIGH logic signal */
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  
}
void loop() {
  if(radio.available()) {    // All the following code only runs when radio data is received.
    uint8_t data[2] = { 0 }; // First int indicates the relay to change, 
                             // Second int indicates whether to turn on or off.
    bool done = false;
    
    while(!done){
      done = radio.read(data, sizeof(data));
      Serial.println("Data Received: ");
      Serial.print("\tdata[0] = ");
      Serial.print(data[0]);
      Serial.print(" data[1] = ");
      Serial.println(data[1]);
      Serial.println(""); 
    } 
    
    switch(data[0]){  // data[0] -- relay number
                      // data[1] -- on or off (1 or 0)
      case 1:         
        if(data[1] == 1){ // Turn on the relay!
          digitalWrite(2, LOW);
          digitalWrite(1, HIGH);

        } else if (data[1] == 0){ // or, Turn it off!
          digitalWrite(2, HIGH);
          digitalWrite(1, LOW);

        }
        Serial.print("Received signal for Relay 1: ");
        Serial.println(data[1]);
        break;
        
      case 2:
        if(data[1] == 1){ // Turn on the relay!
          digitalWrite(3, LOW);
          digitalWrite(6, HIGH);
          
        } else if (data[1] == 0){
          digitalWrite(3, HIGH);
          digitalWrite(6, LOW);
          
        }
        Serial.print("Received signal for Relay 2: ");
        Serial.println(data[1]);
        break;
        
      case 3:
        if(data[1] == 1){ // Turn on the relay!
          digitalWrite(4, LOW);
          digitalWrite(7, HIGH);
          
        } else if (data[1] == 0){
          digitalWrite(4, HIGH);
          digitalWrite(7, LOW);
          
        }
        Serial.print("Received signal for Relay 3: ");
        Serial.println(data[1]);
        break;
        
      case 4:
        if(data[1] == 1){ // Turn on the relay!
          digitalWrite(5, LOW);
          digitalWrite(8, HIGH);
          
        } else if (data[1] == 0){
          digitalWrite(5, HIGH);
          digitalWrite(8, LOW);
          
        }
        Serial.print("Received signal for Relay 4: ");
        Serial.println(data[1]);
        break;  
    } // End Switch
  } // End If
} // End Loop
