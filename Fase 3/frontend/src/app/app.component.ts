import { Component } from '@angular/core';
import Swal, {SweetAlertIcon} from 'sweetalert2';

import {
  IMqttMessage,
  IMqttServiceOptions,
  MqttService,
  IPublishOptions,
} from 'ngx-mqtt';
import { IClientSubscribeOptions } from 'mqtt-browser';
import { Subscription } from 'rxjs';

export interface Metrica {
  nombre: string;
  valor: string;
}

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.scss'],
})
export class AppComponent {

  constructor(private _mqttService: MqttService) {
    this.client = this._mqttService;
  }

  private curSubscription: Subscription | undefined;

  connection = {
    // Broker
    hostname: 'broker.emqx.io',
    port: 8083,
    path: '/mqtt',
    clean: true,
    connectTimeout: 4000,
    //reconnectPeriod: 1000*60*60,  // 1 hora
    reconnectPeriod: 4000, 
    // Credenciales de conexion
    clientId: 'mqttx_04be0b77_',
    username: '',
    password: '',
    protocol: 'ws',
  }

  TOPIC_TEMPERATURA = "g4Temperatura";
  TOPIC_SOLICITUD = "g4inTopic";

  subscription = {
    topic: this.TOPIC_TEMPERATURA,
    qos: 0,
  };

  publish = {
    topic: this.TOPIC_SOLICITUD,
    qos: 0,
    payload: '',
  };

  client: MqttService | undefined;
  isConnection = false;
  subscribeSuccess = false;

  sensorValor = 0;
  porcentaje = 0;
  tituloGrafica = '';
  unidadMedida = '';
  escala = {min: 0, max: 0};
  mostrarGrafica = false;
  mostrarTabla = false;
  medidas : Metrica[] = [];
  medidas2 : Metrica[] = [
    {nombre: 'Temperatura', valor: '0'},
    {nombre: 'Humedad', valor: '0'},
    {nombre: 'CO2', valor: '0'},
    {nombre: 'Luz', valor: '0'},
    {nombre: 'Movimiento', valor: '0'}
  ];
  mostrarUnidades = true;
  displayedColumns: string[] = ['nombre', 'valor'];
  jsonObject: any;
  mensaje: string = '';

  // Crear Conexion con el Broker 
  createConnection() {
    try {
      this.client?.connect(this.connection as IMqttServiceOptions)
    } catch (error) {
      console.log('mqtt.connect error', error);
      this.minimalSweetAlert('¡Falló la conexión con el borker!', "error");
    }

    this.client?.onConnect.subscribe(() => {
      this.isConnection = true
      console.log('¡La conexión se realizó correctamente!');
      this.minimalSweetAlert('¡Conexión exitosa!', "success");
    });

    this.client?.onError.subscribe((error: any) => {
      this.isConnection = false
      console.log('La conexión falló', error);
      this.minimalSweetAlert('¡Falló la conexión!', "error");
    });

    this.client?.onMessage.subscribe((packet: any) => {
      console.log(`Mensaje recibido ${packet.payload.toString()} del topic ${packet.topic}`);
      
    })
  }



  // Suscribirse a un Topic
  doSubscribe(topic: string) { 
    const qos = 0;
    console.log('Subscribiendo al Topic:', topic)
    this.curSubscription = this.client?.observe(topic, { qos } as IClientSubscribeOptions).subscribe((message: IMqttMessage) => {
      
      console.log('Suscripcion al Topic - Respuesta:', message.payload.toString())
      this.jsonObject = JSON.parse(message.payload.toString());
      if(this.jsonObject.temperatura !== undefined ){
        
        this.sensorValor =  this.jsonObject.temperatura;
        this.tituloGrafica = 'Temperatura';
        this.unidadMedida = ' C°';
        this.escala.min = 0;
        this.escala.max = 100;
        this.porcentaje = (parseFloat(this.sensorValor.toString()) * 100)/parseFloat(this.escala.max.toString());
        this.mostrarGrafica = true;


      }else if(this.jsonObject.humedad !== undefined){

        this.sensorValor = this.jsonObject.humedad;
        this.tituloGrafica = 'Porcentaje de Humedad';
        this.unidadMedida = ' %';
        this.escala.min = 0;
        this.escala.max = 100;
        this.porcentaje = (parseFloat(this.sensorValor.toString()));
        this.mostrarGrafica = true;

      }else if(this.jsonObject.co2 !== undefined){

        this.sensorValor = this.jsonObject.co2;
        this.tituloGrafica = 'Dióxido de Carbono (CO2)';
        this.unidadMedida = ' ppm';
        this.escala.min = 0;
        this.escala.max = 5000;
        this.porcentaje = (parseFloat(this.sensorValor.toString()) * 100)/parseFloat(this.escala.max.toString());
        this.mostrarGrafica = true;

      }else if(this.jsonObject.luz !== undefined){

        this.sensorValor = this.jsonObject.luz;
        this.tituloGrafica = 'Luz'
        this.unidadMedida = ' Lumens';
        this.escala.min = 0;
        this.escala.max = 1000;
        this.porcentaje = (parseFloat(this.sensorValor.toString()) * 100)/parseFloat(this.escala.max.toString());
        this.mostrarGrafica = true;

      }else if(this.jsonObject.mov !== undefined){
        if(this.jsonObject.mov === 1){
          this.unidadMedida = "Si hay movimiento";
        }else{
          this.unidadMedida = "No hay movimiento";
        }
        this.tituloGrafica = 'Movimiento'
        this.escala.min = 0;
        this.escala.max = 1;
        this.mostrarUnidades = false;
        this.sensorValor = this.jsonObject.mov;
        this.porcentaje = (parseFloat(this.sensorValor.toString()) * 100)/parseFloat(this.escala.max.toString());
        this.mostrarGrafica = true;

      }else{

        this.sensorValor = 0;
        this.tituloGrafica = ''
        this.unidadMedida = '';
        this.escala.min = 0;
        this.escala.max = 0;
        this.porcentaje = (parseFloat(this.sensorValor.toString()) * 100)/parseFloat(this.escala.max.toString());
        this.mostrarGrafica = true;

      }
      console.log('Valor del sensor:', this.sensorValor)
      this.doUnSubscribe();
    })
  }

  // Desuscribirse de un Topic
  doUnSubscribe() {
    this.curSubscription?.unsubscribe()
    this.subscribeSuccess = false
  }

  // Publicar un Topic
  doPublish(action: string, topic2: string) {
    this.mostrarGrafica = false;
    this.mostrarUnidades = true;
    this.mostrarTabla = false;
    console.log("action", action)
    this.publish.payload = action

    const { topic, qos, payload } = this.publish
    console.log(this.publish)

    this.client?.unsafePublish(topic, payload, { qos } as IPublishOptions)
    this.doSubscribe(topic2)
  }

  guardarEEPROM() {
    this.mostrarGrafica = false;
    this.mostrarUnidades = true;
    this.mostrarTabla = false;
    console.log("action", "6")
    this.publish.payload = "6"
    const { topic, qos, payload } = this.publish
    console.log(this.publish)
    this.client?.unsafePublish(topic, payload, { qos } as IPublishOptions)
    this.subscribeGuardarEEPROM();
  }

  subscribeGuardarEEPROM() { 
    const qos = 0;
    const topic = "g4Eeprom";
    console.log('Subscribiendo al Topic:', topic)
    this.curSubscription = this.client?.observe(topic, { qos } as IClientSubscribeOptions).subscribe((message: IMqttMessage) => {
      
      console.log('Suscripcion al Topic - Respuesta:', message.payload.toString())
      this.jsonObject = JSON.parse(message.payload.toString());
      if(this.jsonObject.mensaje !== undefined){
        this.mensaje = this.jsonObject.mensaje;
        this.minimalSweetAlert("¡Datos guardados exitosamente!", "success");
      }else{
        this.minimalSweetAlert('Error al guardar los datos', "error");
      }
      this.doUnSubscribe();
    })
  }

  ventilador(){
    this.mostrarGrafica = false;
    this.mostrarUnidades = true;
    this.mostrarTabla = false;
    console.log("action", "8")
    this.publish.payload = "8"
    const { topic, qos, payload } = this.publish
    console.log(this.publish)
    this.client?.unsafePublish(topic, payload, { qos } as IPublishOptions)
    this.subscribeVentilador();
  }

  subscribeVentilador() {
    const qos = 0;
    const topic = "g4Motor";
    console.log('Subscribiendo al Topic:', topic)
    this.curSubscription = this.client?.observe(topic, { qos } as IClientSubscribeOptions).subscribe((message: IMqttMessage) => {
      
      console.log('Suscripcion al Topic - Respuesta:', message.payload.toString())
      this.jsonObject = JSON.parse(message.payload.toString());
      if(this.jsonObject.motor !== undefined){
        if(this.jsonObject.motor == "1"){
          this.minimalSweetAlert('Ventilador encendido', "info");
        }else{
          this.minimalSweetAlert('Ventilador apagado', "info");
        }
      }else{
        this.minimalSweetAlert('Error al encender/apagar el ventilador', "error");
      }
      this.doUnSubscribe();
    })
  }

  leerEEPROM() {
    this.mostrarGrafica = false;
    this.mostrarUnidades = true;
    this.mostrarTabla = false;
    this.medidas = [];
    console.log("action", "7")
    this.publish.payload = "7"
    const { topic, qos, payload } = this.publish
    console.log(this.publish)
    this.client?.unsafePublish(topic, payload, { qos } as IPublishOptions)
    this.subscribeLeerEEPROM();
  }

  subscribeLeerEEPROM() { 
    const qos = 0;
    const topic = "g4Eeprom";
    console.log('Subscribiendo al Topic:', topic)
    this.curSubscription = this.client?.observe(topic, { qos } as IClientSubscribeOptions).subscribe((message: IMqttMessage) => {
      
      console.log('Suscripcion al Topic - Respuesta:', message.payload.toString())
      this.jsonObject = JSON.parse(message.payload.toString());
      this.medidas.push(
        {
          nombre: "Temperatura",
          valor : (this.jsonObject.temperatura).toString() + " C°"
        }
      );
      this.medidas.push(
        {
          nombre: "Humedad",
          valor : (this.jsonObject.humedad).toString() + " %"
        }
      );
      this.medidas.push(
        {
          nombre: "CO2",
          valor : (this.jsonObject.co2).toString() + " ppm"
        }
      );
      this.medidas.push(
        {
          nombre: "Luz",
          valor : (this.jsonObject.luz).toString() + " Lumens"
        }
      );
      if(this.jsonObject.mov === 1){
        this.medidas.push(
          {
            nombre: "Movimiento",
            valor : "Si hay movimiento"
          }
        );
      }else{
        this.medidas.push(
          {
            nombre: "Movimiento",
            valor : "No hay movimiento"
          }
        );
      }
      this.doUnSubscribe();
      this.mostrarTabla = true;
      this.mostrarGrafica = false;
    })
  }

  // Terminar Conexion con el Broker 
  destroyConnection() {
    try {
      this.client?.disconnect(true)
      this.isConnection = false
      this.mostrarTabla = false;
      console.log('¡Desconectado exitosamente!')
      this.minimalSweetAlert('¡Desconexión exitosa!', "success");
    } catch (error: any) {
      console.log('Falló la desconexión', error.toString())
      this.minimalSweetAlert('¡Falló la desconexión!', "error");
    }
  }

  accionLuz(){
    console.log("Se encendió el foco");
    
  }

  accionPuerta(){
    console.log("Se abrió la puerta");
  }


  // Alertas SweetAlert
  minimalSweetAlert(mensaje: string, icon_type: SweetAlertIcon) {
    const Toast = Swal.mixin({
      toast: true,
      position: "top-end",
      showConfirmButton: false,
      timer: 3000,
      timerProgressBar: true,
      didOpen: (toast) => {
        toast.onmouseenter = Swal.stopTimer;
        toast.onmouseleave = Swal.resumeTimer;
      }
    });
    Toast.fire({
      icon: icon_type,
      title: mensaje
    });
  }
}
