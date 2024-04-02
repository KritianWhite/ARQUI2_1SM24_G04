
cambiarEstadoMotor(){
    if (estadoMotor == false){
        // Encendemos el motor
        estadoMotor = 1;
        digitalWrite(motor1, HIGH);
        digitalWrite(motor2, LOW);
        //enviamos mensaje de confirmación a broker
        String payload = "{\"motor\": \"1\"}";
        client.publish("g4Motor", payload.c_str());
    } else {
        // Apagamos el motor
        estadoMotor = 0;
        digitalWrite(motor1, LOW);
        digitalWrite(motor2, LOW);
        //enviamos mensaje de confirmación a broker
        String payload = "{\"motor\": \"0\"}";
        client.publish("g4Motor", payload.c_str());
    }
}