// Script: Test - Micro SD card module
// Description: Basic detection, readings and writings on micro SD card
// Autor: @ander26pu
// Fecha: 2024-09-16
// Board: LILYGO-T18

#include <SPI.h>  // Biblioteca para comunicación SPI
#include <SD.h>   // Biblioteca para manejar la tarjeta SD

#define HSPI_MISO   12
#define HSPI_MOSI   13
#define HSPI_SCK    14
#define SD_CS_PIN   15  // Cambia este pin según tu diseño

File myFile;

void setup() {
  Serial.begin(115200);
  
  // Inicializa el bus HSPI
  SPI.begin(HSPI_SCK, HSPI_MISO, HSPI_MOSI, SD_CS_PIN);
  
  // Intenta inicializar la tarjeta SD
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("Fallo en la inicialización de la tarjeta SD.");
    while (1); // Si falla, queda en un bucle infinito
  } else {
    Serial.println("Tarjeta SD detectada y lista.");
  }

  // Abre o crea un archivo en la tarjeta SD
  myFile = SD.open("/test.txt", FILE_WRITE);
  
  // Si el archivo se abrió correctamente, escribe en él
  if (myFile) {
    Serial.println("Escribiendo en test.txt...");
    myFile.println("Prueba de escritura en tarjeta SD con ESP32");
    myFile.close();  // Cierra el archivo para guardar los cambios
    Serial.println("Escritura completada.");
  } else {
    Serial.println("Error al abrir test.txt");
  }

  // Ahora intenta leer el archivo
  myFile = SD.open("/test.txt");
  if (myFile) {
    Serial.println("Leyendo test.txt:");
    while (myFile.available()) {
      Serial.write(myFile.read());  // Muestra el contenido del archivo
    }
    myFile.close();  // Cierra el archivo después de leer
  } else {
    Serial.println("Error al abrir test.txt para lectura.");
  }

  // Elimina el archivo como parte del test
  if (SD.remove("/test.txt")) {
    Serial.println("test.txt eliminado correctamente.");
  } else {
    Serial.println("No se pudo eliminar test.txt.");
  }
}

void loop() {
  // No se requiere realizar nada en el loop para esta prueba
}

