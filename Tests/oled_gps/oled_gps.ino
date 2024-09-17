// Script: Test OLED 0.96'' with GPS Neo 8MN
// Description: Basic demo for position readings from GPS NEO 8MN and show on OLED screen
// Autor: @ander26pu
// Fecha: 2024-09-16
// Board: LILYGO-T18

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>

#define SCREEN_WIDTH 128 // Ancho de la pantalla OLED
#define SCREEN_HEIGHT 64 // Alto de la pantalla OLED

// Definir el pin de reinicio, si no se usa, define como -1
#define OLED_RESET    -1

// Inicializar la pantalla con la dirección I2C predeterminada 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Inicializar la comunicación serial para el GPS
HardwareSerial serialGPS(1); // Usamos UART1 (GPIO 16 y 17........... para RX/TX)
TinyGPSPlus gps;             // Instancia del objeto TinyGPSPlus

void setup() {
  // Inicializar la comunicación serial para depuración
  Serial.begin(115200);

  // Inicializar la pantalla OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Fallo al inicializar la pantalla OLED"));
    for(;;); // Detener ejecución si falla
  }

  // Inicializar la comunicación serial para el GPS a 9600 baudios
  serialGPS.begin(9600, SERIAL_8N1, 19, 18);
  // Limpiar el buffer de la pantalla
  display.clearDisplay();

  // Mostrar mensaje de inicio
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
  } else {
    display.println(F("Esperando señal GPS..."));
  }

  // Enviar los datos a la pantalla
  display.display();

  // Añadir un pequeño retardo para evitar sobrecarga en la pantalla
  delay(1000);
}
