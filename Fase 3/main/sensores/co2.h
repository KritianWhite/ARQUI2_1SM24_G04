float getPartPerMillon(){
  int sensorValue = analogRead(analogMq135);
  float Rs = MQ135_RL * (1023.0 / sensorValue - 1.0); // Calcular la resistencia del sensor

  // Calcular las partes por millón de gases diversos
  float ppm = MQ135_PARA * pow(Rs / MQ135_RO, -MQ135_PARB);

  return ppm;
}