#define Echo 5
#define Trigger 6

#define analogMq135 A0

#define analogFotocelda A1

#define DHT_TYPE DHT11
#define DHT_PIN 7

#define EEPROM_START_TEMPERATURA 0
#define EEPROM_START_HUMEDAD 10
#define EEPROM_START_LUZ 20
#define EEPROM_START_CO2 30
#define EEPROM_START_PROXIMIDAD 40

enum Estado
{
  NINGUNO,
  FOTOCELDA,
  MOSTRARINFO
};

Estado ESTADO;

DHT dht(DHT_PIN, DHT_TYPE);

// Definimos la sensibilidad de la fotocelda
const float sensibilidadFotocelda = 100.0; // V/lux

// Definimos el factor de corrección
const float factorCorreccion = 1.0;

// Definimos el área sensible a la luz de la fotocelda
const float areaFotocelda = 1.0; // cm^2

bool puedeLeerFotocelda = true;
long unsigned tiempoFotocelda = 0;
int tiempoEsperaFotocelda = 4000;

const long A = 1000; // Resistencia en oscuridad en KΩ
const int B = 15;    // Resistencia a la luz (10 Lux) en KΩ
const int Rc = 10;   // Resistencia calibracion en KΩ

// definimos los pines del motor
#define motor1 9
#define motor2 8
bool estadoMotor = false;

// Conexión a la red wifi: nombre de la red y contraseña
#define WIFI_AP "Xiaomi 11T Pro"
#define WIFI_PASSWORD "12345678Steven"

// Nombre o IP del servidor mosquitto
char server[50] = "broker.emqx.io";

// Inicializamos el objeto de cliente esp
WiFiEspClient espClient;

// Iniciamos el objeto subscriptor del cliente
// con el objeto del cliente
PubSubClient client(espClient);

// Contador para el envio de datos
unsigned long lastSend;

int status = WL_IDLE_STATUS;