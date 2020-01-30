#include <RF24Network.h>
#include <SPI.h>
#include <RF24.h>
#include <CRC32.h>

#define BAUDRATE 115200
#define BYTES_PER_FRAME 1

RF24 radio(7, 8); // CE, CSN
RF24Network network(radio);      // Network uses that
const uint8_t chan = 0xAA;
const uint16_t node = 00;

byte data[BYTES_PER_FRAME];
boolean newData = false;

int inPin = 2;

union ArrayToInteger {
  byte array[4];
  uint32_t integer;
};

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(inPin, INPUT);
  Serial.begin(BAUDRATE);
  delay(3000);            // Power up delay
  SPI.begin();
  radio.begin();
  network.begin(chan, node);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setRetries(1, 1);
  radio.setAutoAck(false);
}

void loop() {
  if (digitalRead(inPin)) {
    data[0] = 1;
    Serial.println("Set data[0] = 1");
  } else {
    data[0] = 0;
    Serial.println("Set data[0] = 0");
  }
  network.update();
  RF24NetworkHeader header();
  network.multicast(header, data, BYTES_PER_FRAME, 1);
}
