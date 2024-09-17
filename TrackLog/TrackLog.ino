// Script: TrackLog V1.0
// Description: Guarda datos del gps con las aceleraciones con fecha y hora en una micro sd, mostrando el estado en una pantalla OLED
// Autor: @ander26pu
// Fecha: 2024-09-16
// Board: LILYGO-T18

// Description: Guarda datos del gps con las aceleraciones con fecha y hora en una micro sd, mostrando el estado en una pantalla OLED
// Board: LILYGO-T18

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>
#include <SPI.h>  // Biblioteca para comunicación SPI
#include <SD.h>   // Biblioteca para manejar la tarjeta SD
#include <DFRobot_BMI160.h>
#include "RTClib.h"

#define I2C_SDA 21
#define I2C_SCL 22

#define SCREEN_WIDTH 128 // Ancho de la pantalla OLED
#define SCREEN_HEIGHT 64 // Alto de la pantalla OLED

// Definir el pin de reinicio, si no se usa, define como -1
#define OLED_RESET    -1

#define HSPI_MISO   12
#define HSPI_MOSI   13
#define HSPI_SCK    14
#define SD_CS_PIN   15  // Cambia este pin según tu diseño

File myFile;

// Inicializar la pantalla con la dirección I2C predeterminada 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Inicializar la comunicación serial para el GPS
HardwareSerial serialGPS(1); // Usamos UART1 (GPIO 19 y 18 para RX/TX)
TinyGPSPlus gps;             // Instancia del objeto TinyGPSPlus

RTC_DS3231 rtc;

DFRobot_BMI160 bmi160;

const int8_t i2c_addr = 0x69;

void setup() {
  Serial.begin(115200);
  delay(100);
  
  // Inicializa la comunicación I2C
  Wire.begin(I2C_SDA, I2C_SCL);

  if (!rtc.begin()) {
    Serial.println("No se pudo encontrar el RTC DS3231");
    while (1);
  }

  // Inicializa el sensor BMI160
  if (bmi160.softReset() != BMI160_OK) {
    Serial.println("Error en el reinicio del BMI160");
    while(1);
  }
  
  if (bmi160.I2cInit(i2c_addr) != BMI160_OK) {
    Serial.println("Error en la inicialización del BMI160");
    while(1);
  }

  // Inicializa el bus HSPI
  SPI.begin(HSPI_SCK, HSPI_MISO, HSPI_MOSI, SD_CS_PIN);
  
  // Intenta inicializar la tarjeta SD
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("Fallo en la inicialización de la tarjeta SD.");
    while (1);
  } else {
    Serial.println("Tarjeta SD detectada y lista.");
  }

  // Inicializar la pantalla OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Fallo al inicializar la pantalla OLED"));
    for(;;);
  }

  // Inicializar la comunicación serial para el GPS a 9600 baudios
  serialGPS.begin(9600, SERIAL_8N1, 19, 18);

  // Limpiar el buffer de la pantalla y mostrar mensaje de inicio
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,10);
  display.println(F("Esperando GPS..."));
  display.display();
}

void loop() {
  

  // Leer los datos del GPS
  while (serialGPS.available() > 0) {
    gps.encode(serialGPS.read());
  }
  
  // Limpiar la pantalla
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);

  // Verificar si tenemos datos válidos de GPS
  if (gps.location.isValid()) {
    display.println(F("GPS Datos:"));

    // Mostrar la latitud
    display.print(F("Lat: "));
    display.println(gps.location.lat(), 6);

    // Mostrar la longitud
    display.print(F("Lon: "));
    display.println(gps.location.lng(), 6);
    
    // Mostrar velocidad en km/h
    display.print(F("Vel: "));
    display.print(gps.speed.kmph());
    display.println(F(" km/h"));

    // Mostrar número de satélites
    display.print(F("Sats: "));
    display.println(gps.satellites.value());

    // Enviar los datos a la pantalla
    display.display();

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

    // Guardar los datos en la tarjeta SD solo si los datos del GPS son válidos
    myFile = SD.open("/data.txt", FILE_WRITE);
    if (myFile) {
      Serial.println("Escribiendo en data.txt...");

      DateTime now = rtc.now();
      // Escribir fecha y hora
      myFile.print(now.year(), DEC);
      myFile.print('/');
      myFile.print(now.month(), DEC);
      myFile.print('/');
      myFile.print(now.day(), DEC);
      myFile.print(" ");
      myFile.print(now.hour(), DEC);
      myFile.print(':');
      myFile.print(now.minute(), DEC);
      myFile.print(':');
      myFile.print(now.second(), DEC);
      myFile.print("\tLat: ");
      myFile.print(gps.location.lat(), 6);
      myFile.print("\tLon: ");
      myFile.print(gps.location.lng(), 6);
      myFile.print("\tVel: ");
      myFile.print(gps.speed.kmph());
      myFile.print(" km/h");
      
      // Escribir los datos del acelerómetro
      for(i = 3; i < 6; i++) {
        myFile.print("\tAccel: ");
        myFile.print(accelGyro[i] / 16384.0);
      }
      myFile.println();
      
      myFile.close();  // Cierra el archivo para guardar los cambios
      Serial.println("Escritura completada.");
    } else {
      Serial.println("Error al abrir data.txt");
    }
  } else {
    // Si no hay datos válidos, muestra un mensaje en la pantalla OLED
    display.println(F("Esperando señal GPS..."));
    display.display();
  }

  delay(1000);
}
