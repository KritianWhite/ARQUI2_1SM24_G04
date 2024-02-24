#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <DHT.h>

LiquidCrystal lcd(46, 47, 48, 49, 50, 51);
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

enum Estado {
  NINGUNO,
  FOTOCELDA,
  MOSTRARINFO
};

Estado ESTADO;

DHT dht(DHT_PIN, DHT_TYPE);

const int boton1 = 2;
const int boton2 = 3;
const int boton3 = 18;
const int boton4 = 19;

const int boton5 = 20;
const int boton6 = 17;

// Definimos la sensibilidad de la fotocelda
const float sensibilidadFotocelda = 100.0;  // V/lux

// Definimos el factor de corrección
const float factorCorreccion = 1.0;

// Definimos el área sensible a la luz de la fotocelda
const float areaFotocelda = 1.0;  // cm^2

bool puedeLeerFotocelda = true;
long unsigned tiempoFotocelda = 0;
int tiempoEsperaFotocelda = 4000;

const long A = 1000;  //Resistencia en oscuridad en KΩ
const int B = 15;     //Resistencia a la luz (10 Lux) en KΩ
const int Rc = 10;    //Resistencia calibracion en KΩ

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);          //iniciailzamos la comunicación
  pinMode(Trigger, OUTPUT);    //pin como salida
  pinMode(Echo, INPUT);        //pin como entrada
  digitalWrite(Trigger, LOW);  //Inicializamos el pin con 0

  pinMode(boton1, INPUT_PULLUP);  //pin como entrada
  pinMode(boton2, INPUT_PULLUP);  //pin como entrada
  pinMode(boton3, INPUT_PULLUP);  //pin como entrada
  pinMode(boton4, INPUT_PULLUP);  //pin como entrada
  pinMode(boton5, INPUT_PULLUP);  //pin como entrada
  pinMode(boton6, INPUT_PULLUP);  //pin como entrada


  attachInterrupt(digitalPinToInterrupt(boton2), fun2, FALLING);

  attachInterrupt(digitalPinToInterrupt(boton3), fun3, FALLING);

  lcd.begin(16, 2);
  lcd.clear();

  dht.begin();

  writeLine(0, "Welcome");
  writeLine(1, "Developed by G04");
  delay(3000);
  clearLine(0);
  clearLine(1);
  ESTADO = NINGUNO;
}

void loop() {
  // put your main code here, to run repeatedly:

  if (ESTADO == NINGUNO) {
    verificarBotones();
    lcd.setCursor(0, 0);
    lcd.print("Presione un");
    lcd.setCursor(0, 1);
    lcd.print("boton...");
  } else if (ESTADO == FOTOCELDA) {
    fotocelda();
  } else if (ESTADO == MOSTRARINFO){
    mostrarDatosEEPROM();
  }
}

void fotocelda() {
  if (puedeLeerFotocelda) {
    puedeLeerFotocelda = false;
    tiempoFotocelda = millis();

    float illuminance = getLumens();

    // Muestra la lectura del sensor de luz en el LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Lumens: ");
    lcd.print(illuminance);
  }

  if (tiempoFotocelda != 0) {
    if ((millis() - tiempoFotocelda) >= tiempoEsperaFotocelda) {
      // Si el tiempo es >= 5 segundos, cambiamos estado
      Serial.println("Cambio de estado");
      ESTADO = NINGUNO;
      puedeLeerFotocelda = true;
      lcd.clear();
      tiempoFotocelda = 0;
    }
  }
}

void verificarBotones() {
  if (digitalRead(boton4) == LOW) {
    fun4();
  }

  if (digitalRead(boton6) == LOW) {
    fun6();
  }

  if (digitalRead(boton1) == LOW) {
    fun1();
  }

  if (digitalRead(boton5) == LOW) {
    fun5();
  }
}

void fun1() {
  guardarDatos();
}

void fun2() {
  ESTADO = MOSTRARINFO;
}

void fun3() {
  ESTADO = FOTOCELDA;
}

void fun4() {
  lcd.clear();
  float ppm = getPartPerMillon();
  lcd.setCursor(0, 0);
  lcd.print("Calidad de Aire: ");
  lcd.setCursor(0, 1);
  lcd.print(ppm);
  lcd.print(" PPM");
  lcd.println("           ");
  lcd.print("  ");
  delay(4000);  //   esperamos 5 segundos para visualizar la información
  lcd.clear();
}

void fun5() {

  float humedad = dht.readHumidity();
  float temperatura = dht.readTemperature();

  if (isnan(humedad) || isnan(temperatura)) {
    Serial.println("No se reconocen datos en el sensor DHT11");
    return;
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Humedad: ");
  lcd.print(humedad);
  lcd.print("%");
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(temperatura);
  lcd.print(" 'C");
  delay(5000);
  lcd.clear();
}

void fun6() {
  bool movimiento = getMovimiento();
  lcd.clear();
  lcd.setCursor(0, 0);
  if (movimiento) {
    lcd.print("Hay Movimiento");
  } else {
    lcd.print("    No hay ");
    lcd.setCursor(0, 1);
    lcd.print("  movimiento");
  }
  delay(4000);  //Hacemos una pausa de 100ms
  lcd.clear();
}

void writeLine(int row, String message) {
  if (row == 0 || row == 1) {
    clearLine(row);
    lcd.setCursor(0, row);

    for (size_t i = 0; i < message.length(); i++) {
      lcd.write(message.charAt(i));  // Escribir un carácter a la vez
    }
  }
}

void clearLine(int row) {
  lcd.setCursor(0, row);
  lcd.print("                 ");
}

void guardarDatos() {

  float lumens = getLumens();
  bool movimiento = getMovimiento();
  float humedad = dht.readHumidity();
  float temperatura = dht.readTemperature();
  float ppm = getPartPerMillon();

  EEPROM.put(EEPROM_START_TEMPERATURA, temperatura);  //Escribir (direccion_inicial, valor)
  EEPROM.put(EEPROM_START_HUMEDAD, humedad);
  EEPROM.put(EEPROM_START_LUZ, lumens);
  EEPROM.put(EEPROM_START_CO2, ppm);
  EEPROM.put(EEPROM_START_PROXIMIDAD, movimiento);

  lcd.clear();
  writeLine(0,"Datos Guardados");
  writeLine(1," Exitosamente");
  delay(2000);
  lcd.clear();
}

void mostrarDatosEEPROM(){

    float lumens;
    bool movimiento;
    float temperatura;
    float humedad;
    float ppm;

    EEPROM.get(EEPROM_START_TEMPERATURA, temperatura); 
    EEPROM.get(EEPROM_START_LUZ, lumens); 
    EEPROM.get(EEPROM_START_PROXIMIDAD, movimiento); 
    EEPROM.get(EEPROM_START_HUMEDAD, humedad); 
    EEPROM.get(EEPROM_START_CO2, ppm);

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Temperatura:");
    lcd.setCursor(0,1);
    lcd.print(temperatura);
    lcd.print(" 'C");
    delay(2500);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Humedad:");
    lcd.setCursor(0,1);
    lcd.print(humedad);
    lcd.print(" %");
    delay(2500);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Calidad de Aire:");
    lcd.setCursor(0,1);
    lcd.print(ppm);
    lcd.print(" PPM");
    delay(2500);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Cantidad de luz:");
    lcd.setCursor(0,1);
    lcd.print(lumens);
    lcd.print(" Lumens");
    delay(2500);
    lcd.clear();
    lcd.setCursor(0,0);
    if(movimiento){
      lcd.print("Hubo Movimiento");
    }else{
      lcd.print("  No Hubo");
      lcd.setCursor(0,1);
      lcd.print("Movimiento");
    }
    delay(2500);
    lcd.clear();
    ESTADO = NINGUNO;
}

float getLumens() {
  int V = analogRead(analogFotocelda);

  return ((long)(1024 - V) * A * 10) / ((long)B * Rc * V);
}

bool getMovimiento() {
  long t;  //timepo que demora en llegar el eco
  long d;  //distancia en centimetros

  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);  //Enviamos un pulso de 10us
  digitalWrite(Trigger, LOW);

  t = pulseIn(Echo, HIGH);  //obtenemos el ancho del pulso
  d = t / 59;               //escalamos el tiempo a una distancia en cm

  Serial.print("Distancia: ");
  Serial.print(d);  //Enviamos serialmente el valor de la distancia
  Serial.print("cm");
  Serial.println();

  if(d<100){
    return true;
  }
  return false;
}

float getPartPerMillon(){
  int val = analogRead(analogMq135);


  float A = 116.6020682;
  float B = -2.769034857;

  // Resistencia en aire limpio
  float R0 = 76.63;

  // Calcula la resistencia del sensor
  float Rs = (1023.0 / val) - 1.0;

  // Calcula la concentración de CO2 en PPM
  float ppm = A * pow(Rs / R0, B);

  return ppm;
}

