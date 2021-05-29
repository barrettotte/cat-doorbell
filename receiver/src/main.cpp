// cat doorbell receiver module

#include <Arduino.h>
#include <RF24.h>     // RF 2.4GHz library
#include <nRF24L01.h> // Driver for 2.4GHz transceiver driver
#include <SPI.h>      // Serial Peripheral Interface

#define CSN_PIN    4  // D4 ; Chip Select Not (CSN) active low
#define CE_PIN     5  // D5 ; Chip Enable (CE) active high
#define SPKR_PIN   6  // D6 ; Piezo speaker beep when in range
#define LED_PIN    7  // D7 ; LED activated when in range

#define BUFF_SIZE 16  // 2 bytes to receive

const byte address[6] = "00011"; // cat-doorbell comms address

// globals
RF24 radio(9, 8);  // CE,CSN
char radioBuffer[32] = {0};


void setup(){
    pinMode(SPKR_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);

    Serial.begin(9600);
    Serial.println("\nSerial monitor initialized.");

    Serial1.begin(9600);
    Serial1.println("\nUART initialized.");

    // init radio module as receiver
    radio.begin();
    radio.openReadingPipe(0, address);
    radio.startListening();
}

void loop(){
    Serial.println("Listening...");

    if(radio.available()) {
        radio.read(&radioBuffer, sizeof(radioBuffer));

        Serial.print("Received message: ");
        Serial.println((char *) radioBuffer);

        digitalWrite(LED_PIN, HIGH);
        tone(SPKR_PIN, 1500, 500); // 0.5s @ 1.5 KHz 
        delay(500);
    } else{
        digitalWrite(LED_PIN, LOW);
    }
    delay(500);
}
