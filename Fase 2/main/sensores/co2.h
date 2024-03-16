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