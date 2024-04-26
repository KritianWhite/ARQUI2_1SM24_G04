
cambiarEstadoMotor(){
    if (estadoMotor == false){
        // Encendemos el motor
        estadoMotor = 1;
        digitalWrite(motor1, HIGH);
        digitalWrite(motor2, LOW);
        //enviamos mensaje de confirmación a broker
        String payload = "8,1";
        enviar(payload);
    } else {
        // Apagamos el motor
        estadoMotor = 0;
        digitalWrite(motor1, LOW);
        digitalWrite(motor2, LOW);
        //enviamos mensaje de confirmación a broker
        String payload = "8,0";
        enviar(payload);
    }
}