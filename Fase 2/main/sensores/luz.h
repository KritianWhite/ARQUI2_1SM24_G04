float getLumens() {
  int V = analogRead(analogFotocelda);

  return ((long)(1024 - V) * A * 10) / ((long)B * Rc * V);
}