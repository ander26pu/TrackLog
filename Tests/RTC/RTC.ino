// Script: Test RTC DS3231
// Description: Basic demo for date and time readings from DS3231
// Autor: @ander26pu
// Fecha: 2024-09-16
// Board: LILYGO-T18
#include <Wire.h>
#include "RTClib.h"

#define I2C_SDA 21
#define I2C_SCL 22
RTC_DS3231 rtc;

void setup() {
  Serial.begin(115200);

  // Inicializa la comunicación I2C
  Wire.begin(I2C_SDA, I2C_SCL);

  if (!rtc.begin()) {
    Serial.println("No se pudo encontrar el RTC DS3231");
    while (1);
  }

  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void loop() {
  DateTime now = rtc.now();

  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  delay(1000); // Espera un segundo antes de repetir
}