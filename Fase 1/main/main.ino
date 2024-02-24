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

enum Estado {
  NINGUNO
};

Estado ESTADO;

DHT dht(DHT_PIN, DHT_TYPE);

const int boton1 = 2;
const int boton2 = 3;
const int boton3 = 18;
const int boton4 = 19;

const int boton5 = 20;
const int boton6 = 17;

int lecturaMQ135 = 0;

// Definimos la sensibilidad de la fotocelda
const float sensibilidadFotocelda = 100.0;  // V/lux

// Definimos el factor de corrección
const float factorCorreccion = 1.0;

// Definimos el área sensible a la luz de la fotocelda
const float areaFotocelda = 1.0;  // cm^2

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

  if(ESTADO == NINGUNO){
    verificarBotones();
    lcd.setCursor(0, 0);
    lcd.print("Presione un");
    lcd.setCursor(0, 1);
    lcd.print("boton...");
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
  Serial.println("Boton 1");
  delay(1000);
}

void fun2() {
  Serial.println("Boton 2");
  delay(1000);
}

void fun3() {
  // Leemos el valor del pin analógico
  int valorLeido = analogRead(analogFotocelda);

  // Convertimos el valor leído a un voltaje
  float voltaje = (valorLeido * 5.0) / 1023.0;

  // Calculamos la iluminancia en lux
  float lux = (voltaje / sensibilidadFotocelda) * factorCorreccion;

  // Convertimos lux a lumen
  float lumen = lux * areaFotocelda;

  // Imprimimos los resultados en la consola serial
  Serial.print("Valor leído: ");
  Serial.println(valorLeido);
  Serial.print("Voltaje: ");
  Serial.println(voltaje);
  Serial.print("Iluminancia (lux): ");
  Serial.println(lux);
  Serial.print("Iluminancia (lumen): ");
  Serial.println(lumen);

  // Esperamos un segundo antes de la siguiente medición
  delay(1000);
}

void fun4() {
  lcd.clear();
  lecturaMQ135 = analogRead(0);  //   se lee el input analogo
  lcd.setCursor(0, 0);
  lcd.print("Calidad de Aire: ");
  lcd.setCursor(0, 1);
  lcd.print(lecturaMQ135, DEC);
  lcd.print(" PPM");
  lcd.println("           ");
  lcd.print("  ");
  delay(5000);  //   esperamos 5 segundos para visualizar la información
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
  lcd.setCursor(0,0);
  lcd.print("Humedad: ");
  lcd.print(humedad);
  lcd.print("%");
  lcd.setCursor(0,1);
  lcd.print("Temp: ");
  lcd.print(temperatura);
  lcd.print(" 'C");
  delay(5000);
  lcd.clear();
}

void fun6() {
  Serial.println("Boton 6");
  delay(1000);
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
