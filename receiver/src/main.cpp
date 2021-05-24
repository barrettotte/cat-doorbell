// cat doorbell receiver module

#include <Arduino.h>
#include <RH_ASK.h>   // Amplitude Shift Keying
#include <SPI.h>      // Serial Peripheral Interface

#define SPKR_PIN   6  // D6 ; Piezo speaker beep when in range
#define LED_PIN    7  // D7 ; LED activated when in range

#define BUFF_SIZE 16  // 2 bytes to receive


RH_ASK radiohead;
uint8_t rfBuffer[12];
uint8_t rfBuffLen = sizeof(rfBuffer);


void setup(){
    pinMode(SPKR_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);

    Serial.begin(9600);
    Serial.println("\nSerial monitor initialized.");

    Serial1.begin(9600);
    Serial1.println("\nUART initialized.");

    if(!radiohead.init()){
        Serial.println("RF driver initialization failed.");
    }
}

void loop(){
    Serial.println("Listening...");

    if(radiohead.recv(rfBuffer, &rfBuffLen)){
        Serial.print("Received message: ");
        Serial.println((char *) rfBuffer);

        digitalWrite(LED_PIN, HIGH);
        tone(SPKR_PIN, 1500, 500); // 0.5s @ 1.5 KHz 
        delay(500);
    } else{
        digitalWrite(LED_PIN, LOW);
    }
    delay(500);
}
