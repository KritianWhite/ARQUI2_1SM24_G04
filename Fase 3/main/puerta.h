void abrirPuerta(){
    servoPuerta.write(80);
    enviar("9,1");
}

void cerrarPuerta(){
    servoPuerta.write(3);
    enviar("9,0");
}

void cambiarEstadoPuerta(){
    if (estadoPuerta == false){
        // Abrimos la puerta
        estadoPuerta = 1;
        abrirPuerta();
    } else {
        // Cerramos la puerta
        estadoPuerta = 0;
        cerrarPuerta();
    }
}