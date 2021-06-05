// cat doorbell transmitter module

#include <Arduino.h>  // Arduino core
#include <RF24.h>     // RF 2.4GHz library
#include <nRF24L01.h> // 2.4GHz transceiver driver
#include <SPI.h>      // Serial Peripheral Interface

#define DEBUG   0               // toggle debug mode
#define MAX_CM 15               // range to trigger in CM; ~32" == 81cm

// pinout
#define ECHO_PIN   2 // D2  ; HC-SR04 echo
#define TRIG_PIN   3 // D3  ; HC-SR04 trigger
#define LED_PIN    5 // D5  ; LED activated when object in range of HC-SR04
#define CE_PIN     7 // D7  ; Chip Enable (CE) - set NRF24L01 to transmit/receive
#define CSN_PIN    8 // D8  ; Chip Select Not (CSN) - NRF24L01 listen to SPI port for data
#define MOSI_PIN  11 // D11 ; Master Out Slave In (MOSI) - SPI input to NRF24L01
#define MISO_PIN  12 // D12 ; Master In Slave Out (MISO) - SPI output to NRF24L01
#define SCK_PIN   13 // D13 ; Serial Clock (SCK) - clock pulse from SPI bus master

// debug precompile
#if DEBUG == 1
  #define DEBUG_PRINT(s)   Serial.print(s)
  #define DEBUG_PRINTLN(s) Serial.println(s)
#else
  #define DEBUG_PRINT(s)
  #define DEBUG_PRINTLN(s)
#endif

// constants
const uint64_t PIPE_ADDR = 0x63617431; // "cat1" in hex

// prototypes
void initRadio();

// globals
RF24 radio(CE_PIN, CSN_PIN);
float distance, duration;
uint8_t payload[1];


void setup(){
    pinMode(TRIG_PIN, OUTPUT);  
    pinMode(ECHO_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);

    #if DEBUG == 1
      Serial.begin(9600);
      while (!Serial);
    #endif
    DEBUG_PRINTLN("\nSerial monitor initialized.");

    initRadio();
    payload[0] = '!';
    delay(1000);
}

// send ultrasonic ping to get distance of object
void ping(){
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);

    duration = pulseIn(ECHO_PIN, HIGH);
    distance = duration * (0.034 / 2); // 1/2 speed of sound wave

    DEBUG_PRINT("Distance: ");
    DEBUG_PRINT(distance);
    DEBUG_PRINTLN(" cm");
}

void loop(){
    ping(); // ping for object

    // check if cat is in front of door
    if(distance < MAX_CM){
        digitalWrite(LED_PIN, HIGH);
        delay(500);

        if(radio.write(&payload, sizeof(payload))) {
            DEBUG_PRINT("Sent payload: ");
            DEBUG_PRINTLN(payload[0]);

            // visualize payload sent
            for(int i = 0; i < 8; i++) {
                digitalWrite(LED_PIN, LOW);
                delay(50);
                digitalWrite(LED_PIN, HIGH);
                delay(50);
            }
        } else {
            DEBUG_PRINTLN("Tx failed");
        }
    } else{
        digitalWrite(LED_PIN, LOW);
    }
    delay(500);
}

// initialize radio module
void initRadio(){
    radio.begin();
    radio.setChannel(108); // channel above most WiFi
    radio.setDataRate(RF24_250KBPS);
    radio.setRetries(15, 15);  // delay * 250 us,count
    radio.setPayloadSize(sizeof(payload));
    radio.setPALevel(RF24_PA_MIN);
    radio.openWritingPipe(PIPE_ADDR);
    radio.stopListening(); // set as transmitter
}