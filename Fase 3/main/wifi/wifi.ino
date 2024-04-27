// Este codigo va en el nodeMCU ESP32 / 12-F
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient mqttClient(espClient);

String ssid = "Xiaomi 11T Pro";
String password = "12345678Steven";

byte cont_intentos = 0;
byte max_intentos = 50;

char *server = "broker.emqx.io";
int port = 1883;

String resultS = "";
String mensajeArduino = "";

void setup()
{
  // Inicia Serial
  Serial.begin(9600);
  // Serial.println("\n");

  wifiInit();
  mqttClient.setServer(server, port);
  mqttClient.setCallback(callback);

  // while(!Serial){;}
}

void loop()
{
  if (!mqttClient.connected())
  {
    reconnect();
  }
  mqttClient.loop();
  // ** Serial conexion Arduino **
  if (Serial.available())
  {

    char valorArduino = Serial.read(); // Lee el carácter recibido
    if (valorArduino != '\n')
    {                                 // Verifica que no sea el fin de la linea
      mensajeArduino += valorArduino; // Agrega el carácter a la cadena
    }
    else
    {
      // Separamos la cadena en cada coma
      String cadena = mensajeArduino;
      String numeroTopic = cadena.substring(0, cadena.indexOf(","));
      String mensaje = cadena.substring(cadena.indexOf(",") + 1, cadena.length());
      char bufferPublish[100]; // Define un buffer para almacenar el dato convertido
      // verificamos el topic
      if (numeroTopic == "1")
      { // temperatura
        String payload = "{\"temperatura\":" + mensaje + "}";
        payload.toCharArray(bufferPublish, 100);
        mqttClient.publish("g4Temperatura", bufferPublish);
      }
      else if (numeroTopic == "2") // humedad
      {
        String payload = "{\"humedad\":" + mensaje + "}";
        payload.toCharArray(bufferPublish, 100);
        mqttClient.publish("g4Humedad", bufferPublish);
      }
      else if (numeroTopic == "3") // CO2
      {
        String payload = "{\"co2\":" + mensaje + "}";
        payload.toCharArray(bufferPublish, 100);
        mqttClient.publish("g4CO2", bufferPublish);
      }
      else if (numeroTopic == "4") // Luz
      {
        String payload = "{\"luz\":" + mensaje + "}";
        payload.toCharArray(bufferPublish, 100);
        mqttClient.publish("g4Luz", bufferPublish);
      }
      else if (numeroTopic == "5") // Movimiento
      {
        String payload = "{\"mov\":" + mensaje + "}";
        payload.toCharArray(bufferPublish, 100);
        mqttClient.publish("g4Movimiento", bufferPublish);
      }else if(numeroTopic == "6"){ // Guardar datos en EEPROM
        String payload = "{\"mensaje\":\"" + mensaje + "\"}";
        payload.toCharArray(bufferPublish, 100);
        mqttClient.publish("g4Eeprom", bufferPublish);
      }else if(numeroTopic == "7"){ // Leer datos de EEPROM
        //es necesario separar el mensaje en los diferentes datos
        String temperatura = mensaje.substring(0, mensaje.indexOf(","));
        mensaje = mensaje.substring(mensaje.indexOf(",") + 1, mensaje.length());
        String humedad = mensaje.substring(0, mensaje.indexOf(","));
        mensaje = mensaje.substring(mensaje.indexOf(",") + 1, mensaje.length());
        String luz = mensaje.substring(0, mensaje.indexOf(","));
        mensaje = mensaje.substring(mensaje.indexOf(",") + 1, mensaje.length());
        String co2 = mensaje.substring(0, mensaje.indexOf(","));
        mensaje = mensaje.substring(mensaje.indexOf(",") + 1, mensaje.length());
        String mov = mensaje;
        String payload = "{\"temperatura\":" + temperatura + ",\"humedad\":" + humedad + ",\"luz\":" + luz + ",\"co2\":" + co2 + ",\"mov\":" + mov + "}";
        payload.toCharArray(bufferPublish, 100);
        mqttClient.publish("g4Eeprom", bufferPublish);
      }else if(numeroTopic == "8"){ // Cambiar estado del motor
        String payload = "{\"motor\":" + mensaje + "}";
        payload.toCharArray(bufferPublish, 100);
        mqttClient.publish("g4Motor", bufferPublish);
      }else if(numeroTopic == "9"){ // Cambiar estado de la puerta
        String payload = "{\"puerta\":" + mensaje + "}";
        payload.toCharArray(bufferPublish, 100);
        mqttClient.publish("g4Servo", bufferPublish);
      }else if(numeroTopic == "10"){ // Cambiar velocidad del motor a lenta
        String payload = "{\"velocidad\":" + mensaje + "}";
        payload.toCharArray(bufferPublish, 100);
        mqttClient.publish("g4Velocidad", bufferPublish);
      }else if(numeroTopic == "11"){ // Cambiar velocidad del motor a rápida
        String payload = "{\"velocidad\":" + mensaje + "}";
        payload.toCharArray(bufferPublish, 100);
        mqttClient.publish("g4Velocidad", bufferPublish);
      }
      mensajeArduino = ""; // Reinicia la cadena para la próxima recepción
    }
  }
}

void wifiInit()
{
  // Conexión WIFI
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED and cont_intentos < max_intentos)
  { // Cuenta hasta 50 si no se puede conectar lo cancela
    cont_intentos++;
    delay(500);
    // Serial.print(".");
  }

  // Serial.println("");

  if (cont_intentos < max_intentos)
  { // Si se conectó
    /* Serial.println("********************************************");
Serial.print("Conectado a la red WiFi: ");
Serial.println(WiFi.SSID());
Serial.print("IP: ");
Serial.println(WiFi.localIP());
Serial.print("macAdress: ");
Serial.println(WiFi.macAddress());
Serial.println("*********************************************");*/
  }
  else
  { // No se conectó
    // Serial.println("------------------------------------");
    // Serial.println("Error de conexion");
    // Serial.println("------------------------------------");
  }
}

void callback(char *topic, byte *payload, unsigned int length)
{
  /*Serial.print("Mensaje recibido - Topic [");
  Serial.print(topic);
  Serial.println("] ");*/
  String resultado = "";
  for (int i = 0; i < length; i++)
  {
    resultado = resultado + (char)payload[i];
  }
  resultado += '\n';
  Serial.write(resultado.c_str());
}

void reconnect()
{
  while (!mqttClient.connected())
  {
    // Serial.print("Intentando conectarse MQTT...");

    if (mqttClient.connect("arduinoClient"))
    {
      // Serial.println("Conectado");
      mqttClient.subscribe("g4inTopic");
    }
    else
    {
      // Serial.print("Fallo, rc=");
      // Serial.print(mqttClient.state());
      // Serial.println(" intentar de nuevo en 5 segundos");
      //  Wait 5 seconds before retrying
      delay(5000);
    }
  }
}