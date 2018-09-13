#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include "SSD1306.h"
#include "images.h"

// TTGO boards
#define SCK     5    // GPIO5  -- SCK
#define MISO    19   // GPIO19 -- MISO
#define MOSI    27   // GPIO27 -- MOSI
#define SS      18   // GPIO18 -- CS
#define RST     14   // GPIO12 -- RESET 
#define DI0     26   // GPIO26 -- IRQ(Interrupt Request)

#define BAND    868E6

// #define OLED 0

#define PA_OUTPUT_PA_BOOST_PIN     1

unsigned int counter = 0;

SSD1306 display(0x3c, 21, 22);
String rssi = "RSSI --";
String packSize = "--";
String packet ;



void setup() {
  pinMode(2, OUTPUT);
  pinMode(16, OUTPUT);

  digitalWrite(16, LOW);    // set GPIO16 low to reset OLED
  delay(50);
  digitalWrite(16, HIGH); // while OLED is running, must set GPIO16 in high

  Serial.begin(115200);
  Serial.println();
  Serial.println("LoRa Sender Test");


  SPI.begin(SCK, MISO, MOSI, SS);

  LoRa.setPins(SS, RST, DI0);
  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  LoRa.setTxPower(20, PA_OUTPUT_PA_BOOST_PIN);
  LoRa.setSpreadingFactor(9);
  Serial.println("init ok");
#ifdef OLED
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  delay(1500);
#endif
}

void loop() {

  for (long freq = 750E6; freq < 999e6; freq = freq + 5e5) {
    LoRa.setFrequency(freq);
#ifdef OLED
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 0, "Frequency: ");
    display.drawString(90, 0, String(freq / 1e6));
    Serial.println(String(counter));
    display.display();
#endif
    // send packet
    LoRa.beginPacket();
    LoRa.print("hello");
    LoRa.print(counter);
    LoRa.endPacket();
  }
}
