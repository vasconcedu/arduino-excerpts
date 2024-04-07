#include <RF24.h>
#include <RF24_config.h>
#include <nRF24L01.h>
#include <printf.h>
#include <SPI.h>

/*
    RF24 RX 

    This sketch is the RX side of an RF24 communication scheme
    based on transceiver module nRF24L01. It receives data from 
    TX and writes it to a digital output pin. The board used is
    an Arduino Uno.

    The circuit:
    - nRF24L01 GND - Uno GND
    - nRF24L01 VCC - Uno 3.3V
    - nRF24L01 CE - Uno 8
    - nRF24L01 CSN - Uno 7
    - nRF24L01 SCK - Uno 13
    - nRF24L01 MOSI - Uno 11
    - nRF24L01 MISO - Uno 12
    - nRF24L01 IRQ - not connected
    - Uno 2 - 330 Ohm resistor - LED - Uno GND

    Created 7 April 2024
    By Eduardo Vasconcelos 

*/

// As of instantiation, RF24::RF24 needs two arguments: 
// CE and CSN pin numbers respectively.
RF24 radioDriver(8, 7);

// The communication address will be the second 
// argument to RF24::openReadingPipe later on. Address
// length defaults to 5 bytes. TX should use the same 
// address as RX. 
uint8_t address[6] = "wolf0";

// Data pin and initial data state. Data pin is the
// one driving the LED.
int dataPin = 2;
int dataState = 1;

void setup() {
  Serial.begin(9600);

  // Setup output pin. Note that the LED is off (LOW) by default,
  // but since data state is set to 1, the firware will turn it on
  // as soon as looping starts.  
  pinMode(dataPin, OUTPUT);
  digitalWrite(dataPin, LOW);

  radioDriver.begin();

  // Set communication address. Arguments to RF24::openReadingPipe
  // are pipe number and communication address. Pipe number is 0. 
  radioDriver.openReadingPipe(0, address);

  // Set power amplifier level to RF24_PA_HIGH, 
  // which resolves to -6 dBm.
  radioDriver.setPALevel(RF24_PA_HIGH);
  radioDriver.startListening();
}

void loop() {
  // Continuously check radio driver for data from TX and 
  // write it to data pin, thus driving the LED accordingly. 
  if (radioDriver.available()) {
    radioDriver.read(&dataState, sizeof(dataState));
    
    if(dataState == 1) {
      Serial.println("[RX] Data state is 1");
      digitalWrite(dataPin, HIGH);
    } else if(dataState == 0) {
      Serial.println("[RX] Data state is 0");
      digitalWrite(dataPin, LOW);
    }
  }
}
