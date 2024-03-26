#include <WiFiEsp.h>
#include <WiFiEspClient.h>
#include <PubSubClient.h>
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(2, 3);  // RX, TX
#endif
#include <EEPROM.h>
#include <DHT.h>
#include "globales.h"
#include "./sensores/co2.h"
#include "./sensores/movimiento.h"
#include "./sensores/tempHum.h"
#include "./sensores/luz.h"

//Conexión a la red wifi: nombre de la red y contraseña
#define WIFI_AP "ARRIS-1C3D"
#define WIFI_PASSWORD "50A5DC411C3D"

//Nombre o IP del servidor mosquitto
char server[50] = "broker.emqx.io";

//Inicializamos el objeto de cliente esp
WiFiEspClient espClient;

//Iniciamos el objeto subscriptor del cliente
//con el objeto del cliente
PubSubClient client(espClient);

//Contador para el envio de datos
unsigned long lastSend;

int status = WL_IDLE_STATUS;

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensaje recibido [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  if (String(topic) == "g4inTopic") {
    String message = String((char*)payload);
    if (message == "1") {
      sendDataTemp();
    } else if (message == "2") {
      sendDataHum();
    } else if (message == "3") {
      sendDataCO2();
    } else if (message == "4") {
      sendDataLuz();
    } else if (message == "5") {
      sendDataMov();
    } else if (message == "6") {
      sendDataTopic(" se recibió guardar eeeprom");
    } else if (message == "7") {
      sendDataTopic(" se recibió guardar eeeprom");
    } else if (message == "8") {
      sendDataTopic(" se recibio encender ventilador");
    } else if (message == "9") {
      sendDataTopic(" se recibio apagar ventilador");
    }
  }
}

void reconnectClient() {
  //Creamos un loop en donde intentamos hacer la conexión
  while (!client.connected()) {
    //Creamos una nueva cadena de conexión para el servidor
    //e intentamos realizar la conexión nueva
    //si requiere usuario y contraseña la enviamos connect(clientId, username, password)
    String clientId = "mqttx_28c75910";
    if (client.connect(clientId.c_str())) {
      client.publish("g4outTopic", "conexion exitosa");
      suscribirse("g4inTopic");
      Serial.println("Suscripción realizada");
    } else {
      Serial.print("[FALLO] [ rc = ");
      Serial.print(client.state());
      Serial.println(" : reintentando en 5 segundos]");
      delay(5000);
    }
  }
}


void setup() {
  //Inicializamos la comunicación serial para el log
  Serial.begin(115200);
  pinMode(Trigger, OUTPUT);    //pin como salida
  pinMode(Echo, INPUT);        //pin como entrada
  digitalWrite(Trigger, LOW);  //Inicializamos el pin con 0
  //Colocamos la referencia del servidor y el puerto
  client.setServer(server, 1883);
  client.setCallback(callback);
  client.setKeepAlive(60);
  dht.begin();
  //Iniciamos la conexión a la red WiFi
  InitWiFi();
  lastSend = 0;
}

void loop() {
  //Validamos si el modulo WiFi aun esta conectado a la red
  status = WiFi.status();
    if(status != WL_CONNECTED) {
        //Si falla la conexión, reconectamos el modulo
        reconnectWifi();
    }

  //Validamos si esta la conexión del servidor
  if (!client.connected()) {
    //Si falla reintentamos la conexión
    reconnectClient();
  }

  client.loop();

  if (Serial.available()) {
    String input = Serial.readString();
    if (input == "1") {
      sendDataTemp();
    } else if (input == "2") {
        sendDataHum();
    }else if (input == "3") {
        sendDataCO2();
    }else if (input == "4") {
        sendDataLuz();
    }else if (input == "5") {
        sendDataMov();
    }else if (input == "6") {
      sendDataTopic(" se recibió guardar eeeprom");
    }else if (input == "7") {
      sendDataTopic(" se recibió guardar eeeprom");
    }else if (input == "8") {
      sendDataTopic(" se recibio encender ventilador");
    }else if (input == "9") {
      sendDataTopic(" se recibio apagar ventilador");
    }
  }
}

//Función para enviar datos de temperatura
void sendDataTemp() {
  // Prepare a JSON payload String
  String payload = "{\"temperatura\": " + String(getTemperature()) + "}";
  // Send payload
  char attributes[100];
  payload.toCharArray(attributes, 100);
  client.publish("g4Temperatura", attributes);
  Serial.println(attributes);
}

//Función para enviar datos de humedad
void sendDataHum() {
  // Prepare a JSON payload String
  String payload = "{\"humedad\": " + String(getHumidity()) + "}";
  // Send payload
  char attributes[100];
  payload.toCharArray(attributes, 100);
  client.publish("g4Humedad", attributes);
  Serial.println(attributes);
}

//Función para enviar datos de CO2
void sendDataCO2() {
  // Prepare a JSON payload String
  String payload = "{\"co2\": " + String(getPartPerMillon()) + "}";
  // Send payload
  char attributes[100];
  payload.toCharArray(attributes, 100);
  client.publish("g4CO2", attributes);
  Serial.println(attributes);
}

//Función para enviar datos de luz
void sendDataLuz() {
  // Prepare a JSON payload String
  String payload = "{\"luz\": " + String(getLumens()) + "}";
  // Send payload
  char attributes[100];
  payload.toCharArray(attributes, 100);
  client.publish("g4Luz", attributes);
  Serial.println(attributes);
}

//Función para enviar datos de movimiento
void sendDataMov() {
  // Prepare a JSON payload String
  String payload = "{\"mov\": " + String(getMovimiento()) + "}";
  // Send payload
  char attributes[100];
  payload.toCharArray(attributes, 100);
  client.publish("g4Movimiento", attributes);
  Serial.println(attributes);
}

//Función para enviar datos a un topic
void sendDataTopic(String data) {
  Serial.println("sendDataTopic");
  // Prepare a JSON payload String
  String payload = data;

  // Send payload
  char attributes[100];
  payload.toCharArray(attributes, 100);
  client.publish("g4outTopic", attributes);
  Serial.println(attributes);
}

//Inicializamos la conexión a la red wifi
void InitWiFi() {
  //Inicializamos el puerto serial
  Serial1.begin(115200);
  //Iniciamos la conexión wifi
  WiFi.init(&Serial1);
  //Verificamos si se pudo realizar la conexión al wifi
  //si obtenemos un error, lo mostramos por log y denememos el programa
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("El modulo WiFi no esta presente");
    while (true)
      ;
  }
  reconnectWifi();
}

void reconnectWifi() {
  Serial.println("Iniciar conección a la red WIFI");
  while (status != WL_CONNECTED) {
    Serial.print("Intentando conectarse a WPA SSID: ");
    Serial.println(WIFI_AP);
    //Conectar a red WPA/WPA2
    status = WiFi.begin(WIFI_AP, WIFI_PASSWORD);
    delay(500);
  }
  Serial.println("Conectado a la red WIFI");
}

// Función para suscribirse a un topic
void suscribirse(const char* topic) {
  client.subscribe(topic);
  Serial.print("Suscrito a: ");
  Serial.println(topic);
}