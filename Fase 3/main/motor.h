
cambiarEstadoMotor(){
    if (estadoMotor == false){
        // Encendemos el motor
        estadoMotor = 1;
        digitalWrite(motor1, HIGH);
        digitalWrite(motor2, LOW);
        analogWrite(pinVelocidad, velocidad1);
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

//funcion para setear velocidad lenta del motor
void setVelocidadLenta(){
    if(estadoMotor == true){
        //si el motor está encendido, seteamos la velocidad lenta
        analogWrite(pinVelocidad, velocidad1);
        String payload = "10,1";
        enviar(payload);
    }else{
        //enviamos mensaje de que el motor no está encendido
        String payload = "10,0";
        enviar(payload);
    }
}

//funcion para setear velocidad rápida del motor
void setVelocidadRapida(){
    if(estadoMotor == true){
        //si el motor está encendido, seteamos la velocidad rápida
        analogWrite(pinVelocidad, velocidad2);
        String payload = "11,2";
        enviar(payload);
    }else{
        //enviamos mensaje de que el motor no está encendido
        String payload = "11,0";
        enviar(payload);
    }
}