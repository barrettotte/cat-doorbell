// cat doorbell transmitter module

#include <Arduino.h>
#include <RH_ASK.h>   // Amplitude Shift Keying
#include <SPI.h>      // Serial Peripheral Interface

#define ECHO_PIN   2  // D2 ; HC-SR04 echo
#define TRIG_PIN   3  // D3 ; HC-SR04 trigger
#define LED_PIN    7  // D7 ; LED activated when in range

#define MAX_CM 15  // range to trigger in CM; ~32" == 81cm


float distance, duration;
RH_ASK radiohead;
const char *msg = "Hello world";


void setup(){
    pinMode(TRIG_PIN, OUTPUT);  
    pinMode(ECHO_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);

    Serial.begin(9600);
    Serial.println("\nSerial monitor initialized.");

    Serial1.begin(9600);
    Serial1.println("\nUART initialized.");

    if(!radiohead.init()){
        Serial.println("RF driver initialization failed.");
    }
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
        // send msg to receiver module
        Serial.print("Sending message ");
        Serial.println(msg);

        radiohead.send((uint8_t *) msg, strlen(msg));
        digitalWrite(LED_PIN, HIGH);

        radiohead.waitPacketSent();
        delay(500);
    } else{
        digitalWrite(LED_PIN, LOW);
    }
    delay(500);
}
