// cat doorbell receiver module

#include <Arduino.h>  // Arduino core
#include <RF24.h>     // RF 2.4GHz library
#include <nRF24L01.h> // 2.4GHz transceiver driver
#include <SPI.h>      // Serial Peripheral Interface

#define DEBUG   1               // toggle debug mode

// pinout
#define LED_PIN    5 // D5  ; LED notification that transmitter found something
#define SPKR_PIN   6 // D6  ; Piezo speaker beep when in range
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
uint8_t radioBuffer[1] = {0};

void setup(){
    pinMode(SPKR_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);

    #if DEBUG == 1
      Serial.begin(9600);
      while(!Serial) {}
    #endif
    DEBUG_PRINTLN("\nSerial monitor initialized.");

    initRadio();
    DEBUG_PRINTLN("Listening...");
    delay(1000);
}

void loop(){
   
    if(radio.available()) {
        radio.read(&radioBuffer, sizeof(radioBuffer));

        // DEBUG_PRINT("  Received message: ");
        // DEBUG_PRINTLN(radioBuffer[0]);

        // check if valid payload
        if (radioBuffer[0] != 0) {
            // DEBUG_PRINT("  Received message: ");
            // DEBUG_PRINTLN(radioBuffer[0]);
            DEBUG_PRINTLN("WE DID IT !!!!!!");

            // DEBUG_PRINTLN("Cat is in front of door!");
            digitalWrite(LED_PIN, HIGH);
            tone(SPKR_PIN, 1500, 500); // 0.5s @ 1.5 KHz
            delay(500);
        } 
        digitalWrite(LED_PIN, LOW);
        // delay(500);
    }
}

// initialize radio module
void initRadio(){
    radio.begin();
    radio.setChannel(108); // channel above most WiFi
    radio.setDataRate(RF24_250KBPS);
    radio.setRetries(3, 5);   // delay * 250 us, retries
    radio.setPayloadSize(sizeof(radioBuffer));
    radio.setPALevel(RF24_PA_MAX);
    radio.openReadingPipe(0, PIPE_ADDR);
    radio.startListening(); // set as receiver
}