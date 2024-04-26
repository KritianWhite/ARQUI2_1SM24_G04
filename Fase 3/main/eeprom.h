
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

    //enviamos mensaje de confirmación a broker
    String payload = "6,Datos guardados en EEPROM";
    enviar(payload);
}

void leerDatos() {
  float temperatura;
  float humedad;
  float lumens;
  float ppm;
  bool movimiento;

  EEPROM.get(EEPROM_START_TEMPERATURA, temperatura);  //Leer (direccion_inicial, variable)
  EEPROM.get(EEPROM_START_HUMEDAD, humedad);
  EEPROM.get(EEPROM_START_LUZ, lumens);
  EEPROM.get(EEPROM_START_CO2, ppm);
  EEPROM.get(EEPROM_START_PROXIMIDAD, movimiento);

  // Prepare a JSON payload String
  String payload = "7," + String(temperatura) + "," + String(humedad) + "," + String(lumens) + "," + String(ppm) + "," + String(movimiento);
  // Send payload
  enviar(payload);
}
