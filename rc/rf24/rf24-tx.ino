#include <RF24.h>
#include <RF24_config.h>
#include <nRF24L01.h>
#include <printf.h>
#include <SPI.h>

/*
    RF24 TX 

    This sketch is the TX side of an RF24 communication scheme
    based on transceiver module nRF24L01. It transmits data to 
    RX based on a digital input pin's readings. The board used is
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
    - Uno 2 - Uno GND will transmit data state 0
    - Uno 2 - disconnected will transmit data state 1

    Created 7 April 2024
    By Eduardo Vasconcelos 

*/

// As of instantiation, RF24::RF24 needs two arguments: 
// CE and CSN pin numbers respectively.
RF24 radioDriver(8, 7);

// The communication address will be the second 
// argument to RF24::openReadingPipe later on. Address
// length defaults to 5 bytes. RX should use the same 
// address as TX. 
uint8_t address[6] = "wolf0";

// Data pin and initial data state. Data pin is the
// one acting as the input.
int dataPin = 2;
int dataState = 0;

void setup() {
  Serial.begin(9600);
  radioDriver.begin();

  // Setup input pin. Note that INPUT_PULLUP is used. This 
  // activates one of ATMega328P's internal pull-up resistors,
  // consequently setting the pin to HIGH when it is not connected
  // and ensuring a well-defined logical level at the pin under 
  // all conditions.
  pinMode(dataPin, INPUT_PULLUP);

  // Set communication address. RF24::openReadingPipe
  // takes address as its only argument. 
  radioDriver.openWritingPipe(address);

  // Set power amplifier level to RF24_PA_HIGH, 
  // which resolves to -6 dBm.
  radioDriver.setPALevel(RF24_PA_HIGH);
  radioDriver.stopListening();
}

void loop() {
  // Continuously transmit data pin reading. 
  if (digitalRead(dataPin) == HIGH) {
    Serial.println("[TX] Data state is 1");
    dataState = 1;
    radioDriver.write(&dataState, sizeof(dataState));
  } else if (digitalRead(dataPin) == LOW) {
    Serial.println("[TX] Data state is 0");
    dataState = 0;
    radioDriver.write(&dataState, sizeof(dataState));
  }
}
