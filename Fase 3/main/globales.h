#define Echo 5
#define Trigger 6

#define analogMq135 A0

#define analogFotocelda A1

#define pinBombilla A2

#define DHT_TYPE DHT11
#define DHT_PIN 7

#define EEPROM_START_TEMPERATURA 0
#define EEPROM_START_HUMEDAD 10
#define EEPROM_START_LUZ 20
#define EEPROM_START_CO2 30
#define EEPROM_START_PROXIMIDAD 40

#define MQ135_RL 20.0 // Resistencia de carga (kΩ)
#define MQ135_RO 76.63 // Resistencia del sensor en aire limpio (kΩ)
#define MQ135_PARA 116.6020682 // Parámetro para calcular gases diversos
#define MQ135_PARB 2.769034857 // Parámetro para calcular gases diversos

String codigo = "";

enum Estado
{
  NINGUNO,
  FOTOCELDA,
  MOSTRARINFO
};

Estado ESTADO;

DHT dht(DHT_PIN, DHT_TYPE);
Servo servoPuerta;

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

// definimos los pines del ventilador
#define motor1 9
#define motor2 8
#define pinVelocidad 10 // pin para controlar la velocidad del ventilador
#define velocidad1 115 // velocidad lenta
#define velocidad2 255 // velocidad rápida


bool estadoMotor = false;
bool estadoPuerta = false;
bool estadoBombilla = true;


// Conexión a la red wifi: nombre de la red y contraseña
#define WIFI_AP "Xiaomi 11T Pro"
#define WIFI_PASSWORD "12345678Steven"

// Nombre o IP del servidor mosquitto
char server[50] = "broker.emqx.io";
