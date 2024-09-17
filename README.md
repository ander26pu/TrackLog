# TrackLog
- Placa de pruebas: [LILYGO-T18](https://github.com/LilyGO/LILYGO-T-Energy)
- GPS Neo 8MN
- Pantalla OLED 0.96''
- Acelerómetro y giroscopio BMI160
- RTC DS3231
- Módulo micro SD card

## Pines Utilizados

En esta sección se detallan los pines utilizados en el proyecto para los diferentes periféricos conectados al microcontrolador.

| Componente              | Pin de Microcontrolador | Función                          |
|-------------------------|-------------------------|----------------------------------|
| **Pantalla OLED (SSD1306)** | I2C SDA (GPIO 21)        | Comunicación I2C (SDA)           |
|                         | I2C SCL (GPIO 22)        | Comunicación I2C (SCL)           |
| **GPS (UART)**           | RX1 (GPIO 19)            | Recepción de datos (RX)          |
|                         | TX1 (GPIO 18)            | Transmisión de datos (TX)        |
| **Tarjeta SD**           | CS (GPIO 15)             | Chip Select (CS)                 |
|                         | MOSI (GPIO 13)           | Master Out Slave In (MOSI)       |
|                         | MISO (GPIO 12)           | Master In Slave Out (MISO)       |
|                         | SCK (GPIO 14)            | Serial Clock (SCK)               |
| **Acelerómetro/Giroscopio (BMI160)** | I2C SDA (GPIO 21)        | Comunicación I2C (SDA)           |
|                         | I2C SCL (GPIO 22)        | Comunicación I2C (SCL)           |
| **RTC (DS3231)**         | I2C SDA (GPIO 21)        | Comunicación I2C (SDA)           |
|                         | I2C SCL (GPIO 22)        | Comunicación I2C (SCL)           |

### Notas adicionales:

- **I2C:** Varios componentes (OLED, acelerómetro/giroscopio, RTC) comparten el bus I2C. Asegúrate de que cada dispositivo tenga una dirección I2C única.
- **SPI:** La tarjeta SD se comunica a través del bus SPI. El pin CS (Chip Select) se define en el código, y el resto de los pines de SPI son comunes a todos los dispositivos SPI.
- **GPS (UART):** El GPS utiliza UART para la comunicación. En este caso, se ha configurado para utilizar UART1.

