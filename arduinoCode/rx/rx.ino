#include <SPI.h>
#include <RF24Network.h>
#include <RF24.h>
#include <FastLED.h>
#include <CRC32.h>

#define LED_PIN     5
#define NUM_LEDS    5
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define BAUDRATE 115200
#define BYTES_PER_FRAME 1

RF24 radio(7, 8); // CE, CSN
RF24Network network(radio);      // Network uses that
const uint8_t chan = 0xAA;
const uint16_t node = 01;

static byte data[BYTES_PER_FRAME];

int ledPin = 2;

union ArrayToInteger {
  byte array[4];
  uint32_t integer;
};

CRGB leds[NUM_LEDS];

void setLEDS2Color(int r, int g, int b, int brightness) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].r = r;
    leds[i].g = g;
    leds[i].b = b;
  }
  FastLED.setBrightness(brightness);
  FastLED.show();
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(BAUDRATE);
  delay(3000);            // Power up delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 100);

  // Initialize Rx Radio and network.
  SPI.begin();
  radio.begin();
  network.begin(chan, node);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setRetries(1, 1);
  radio.setAutoAck(false);

  setLEDS2Color(0, 0, 0, 0);
}

void getNextFrame() {
  Serial.println("Waiting for data...");
  while (!network.available()) {
    network.update();
    delay(1);
    Serial.println("Waiting for data...");
  }
  RF24NetworkHeader header;
  network.read(header, data, BYTES_PER_FRAME);
  Serial.println("Data read!");
}

void loop() {
    getNextFrame();
    if(data[0] == 1){
        Serial.println("data[0] == 1. Turning on lights.");
        setLEDS2Color(255, 165, 0, 32);
    }else{
      Serial.println("data[0] == 0. Turning off lights.");
        setLEDS2Color(0, 0, 0, 0);
    }
}
