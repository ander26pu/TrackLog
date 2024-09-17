// Script: Test - BMI160 sensor
// Description: Basic demo for accelerometer readings from BMI160
// Autor: @ander26pu
// Fecha: 2024-09-16
// Board: LILYGO-T18
#include <Wire.h>
#include <DFRobot_BMI160.h>

DFRobot_BMI160 bmi160;

// Pines I2C para el ESP32 (ajusta si es necesario)
#define SDA_PIN 21
#define SCL_PIN 22

const int8_t i2c_addr = 0x69;

void setup() {
  // Inicializa la comunicación serie
  Serial.begin(115200);
  delay(100);

  // Configura los pines I2C
  Wire.begin(SDA_PIN, SCL_PIN);
  
  // Reinicia el sensor
  if (bmi160.softReset() != BMI160_OK) {
    Serial.println("Error en el reinicio del BMI160");
    while(1);
  }
  
  // Inicializa el BMI160 en la dirección I2C especificada
  if (bmi160.I2cInit(i2c_addr) != BMI160_OK) {
    Serial.println("Error en la inicialización del BMI160");
    while(1);
  }
}

void loop() {
  int i = 0;
  int rslt;
  int16_t accelGyro[6] = {0}; 
  
  // Obtén los datos de acelerómetro y giroscopio
  rslt = bmi160.getAccelGyroData(accelGyro);
  if(rslt == 0) {
    for(i = 0; i < 6; i++) {
      if (i < 3) {
        // Los primeros tres son datos del giroscopio
        Serial.print("Gyro: ");
        Serial.print(accelGyro[i] * 3.14 / 180.0); // Convertir a radianes
        Serial.print("\t");
      } else {
        // Los últimos tres son datos del acelerómetro
        Serial.print("Accel: ");
        Serial.print(accelGyro[i] / 16384.0); // Escala del acelerómetro
        Serial.print("\t");
      }
    }
    Serial.println();
  } else {
    Serial.println("Error en la lectura de datos");
  }
  delay(100);
}
