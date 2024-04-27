#include <EEPROM.h>
#include <DHT.h>
#include <Servo.h>
#include "globales.h"
#include "serial.h"
#include "./sensores/co2.h"
#include "./sensores/movimiento.h"
#include "./sensores/tempHum.h"
#include "./sensores/luz.h"
#include "eeprom.h"
#include "motor.h"
#include "puerta.h"
#include "rele.h"


void setup() {
  // Inicializamos la comunicación serial para el log
  Serial.begin(9600);
  Serial2.begin(9600);
  pinMode(pinBombilla,OUTPUT);
  digitalWrite(pinBombilla, HIGH);
  pinMode(Trigger, OUTPUT);    // pin como salida
  pinMode(Echo, INPUT);        // pin como entrada
  pinMode(motor1, OUTPUT);     // pin como salida
  pinMode(motor2, OUTPUT);     // pin como salida
  pinMode(pinVelocidad, OUTPUT); // pin como salida
  digitalWrite(Trigger, LOW);  // Inicializamos el pin con 0
  pinMode(analogMq135, INPUT); // pin como entrada
  dht.begin();
  servoPuerta.attach(4);
  servoPuerta.write(3);
  estadoPuerta = 0;
  estadoMotor = 0;
  estadoBombilla = 0;
  digitalWrite(motor1, LOW);
  digitalWrite(motor2, LOW);
}

void loop() {

  if (Serial2.available()) {
    char c = Serial2.read();
    if (c != '\n') {  // Verifica que no sea el fin de la linea
      codigo += c;    // Agrega el carácter a la cadena
    } else {
      Serial.println("el valor recibido desde el nodemcu es: " + codigo);
      if (codigo == "1") {
        sendDataTemp();
      } else if (codigo == "2") {
        sendDataHum();
      } else if (codigo == "3") {
        sendDataCO2();
      } else if (codigo == "4") {
        sendDataLuz();
      } else if (codigo == "5") {
        sendDataMov();
      } else if (codigo == "6") {
        guardarDatos();
      } else if (codigo == "7") {
        leerDatos();
      } else if (codigo == "8") {
        cambiarEstadoMotor();
      } else if (codigo == "9") {
        cambiarEstadoPuerta();
      } else if (codigo == "10") {
        setVelocidadLenta();
      } else if (codigo == "11") {
        setVelocidadRapida();
      } else if (codigo == "12") {
        cambiarEstadoBombilla();
      }
      codigo = "";  // Reinicia la cadena para la próxima recepción
    }
  }
}

// Función para enviar datos de temperatura
void sendDataTemp() {
  // Prepare a JSON payload String
  String payload = "1," + String(getTemperature());
  enviar(payload);
}

// Función para enviar datos de humedad
void sendDataHum() {
  // Prepare a JSON payload String
  String payload = "2," + String(getHumidity());
  enviar(payload);
}

// Función para enviar datos de CO2
void sendDataCO2() {
  // Prepare a JSON payload String
  String payload = "3," + String(getPartPerMillon());
  enviar(payload);
}

// Función para enviar datos de luz
void sendDataLuz() {
  // Prepare a JSON payload String
  String payload = "4," + String(getLumens());
  enviar(payload);
}

// Función para enviar datos de movimiento
void sendDataMov() {
  // Prepare a JSON payload String
  String payload = "5," + String(getMovimiento());
  enviar(payload);
}