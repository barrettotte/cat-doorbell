// cat doorbell transmitter module

#include <Arduino.h>
#include <RF24.h>     // RF 2.4GHz library
#include <nRF24L01.h> // Driver for 2.4GHz transceiver driver
#include <SPI.h>      // Serial Peripheral Interface

#define ECHO_PIN   2  // D2 ; HC-SR04 echo
#define TRIG_PIN   3  // D3 ; HC-SR04 trigger
#define CSN_PIN    4  // D4 ; Chip Select Not (CSN) active low
#define CE_PIN     5  // D5 ; Chip Enable (CE) active high
#define LED_PIN    7  // D7 ; LED activated when in range

#define MAX_CM 15  // range to trigger in CM; ~32" == 81cm

const byte address[6] = "00011"; // cat-doorbell comms address
const char *msg = "Hello world"; // payload to receiver module

// globals
RF24 radio(9, 8);  // CE,CSN
float distance, duration;


void setup(){
    pinMode(TRIG_PIN, OUTPUT);  
    pinMode(ECHO_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);

    Serial.begin(9600);
    Serial.println("\nSerial monitor initialized.");

    Serial1.begin(9600);
    Serial1.println("\nUART initialized.");

    // init radio module as transmitter
    radio.begin();
    radio.openWritingPipe(address);
    radio.stopListening();
}

// send ultrasonic ping to get distance of object
void ping(){
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);

    duration = pulseIn(ECHO_PIN, HIGH);
    distance = duration * (0.034 / 2); // 1/2 speed of sound wave

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.print(" cm\n");
}

void loop(){

    ping(); // get ping distance

    // check if cat is in front of door
    if(distance < MAX_CM){
        Serial.print("Sending message ");
        Serial.println(msg);

        // send msg to receiver module
        radio.write(&msg, sizeof(msg));

        delay(500);
    } else{
        digitalWrite(LED_PIN, LOW);
    }
    delay(500);
}
