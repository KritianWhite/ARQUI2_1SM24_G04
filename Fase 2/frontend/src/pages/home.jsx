import React, { useState, useEffect } from "react";
import mqtt from "mqtt";

import Navbar from "../components/Navbar";
import Velocimetro from "../components/velocimetro";
import './styles/home.css';

export default function Home() {
    const [encendido, setEncendido] = useState(false); // Estado para controlar si el ventilador está encendido
    const [unidad, setUnidad] = useState(""); // Estado para controlar la unidad de medida del velocímetro
    const [titulo, setTitulo] = useState(""); // Estado para controlar el título del velocímetro
    const [porcentaje, setPorcentaje] = useState(0); // Estado para controlar el porcentaje del velocímetro

    const clientId = "emqx_react_" + Math.random().toString(16).substring(2, 8);
    const username = "";
    const password = "";

    const handleVentilador = () => {
        setEncendido(!encendido); // Cambia el estado (true -> false, false -> true)
    };

    const handleClick = async () => {
        // Conectar al broker MQTT
        const client = mqtt.connect("ws://broker.emqx.io:1883", {
            clientId,
            username,
            password,
            // ...other options
        });

        client.on('connect', () => {
            console.log('Conectado al broker MQTT');

            // Publicar mensaje al tópico específico
            client.publish(MQTT_TOPIC, MQTT_MESSAGE, {}, (error) => {
                if (error) {
                    console.error('Error al publicar mensaje', error);
                }
            });

            // Cerrar la conexión
            client.end();
        });
    }

    return (
        <>
            <div className="container-nav">
                <Navbar />
            </div>
            <div className="container-content">
                <div className="home-title">
                    <h1>Dashboard</h1>
                </div>
                <div className="botones">
                    <button className="boton" onClick={handleClick}>Temperatura</button>
                    <button className="boton">Humedad</button>
                    <button className="boton">CO2</button>
                    <button className="boton">Iluminación</button>
                    <button className="boton">Movimiento</button>
                    <button className="boton">Guardar datos</button>
                    <button className="boton">Ver datos</button>
                    <button className="boton" onClick={handleVentilador}>{encendido ? 'Apagar ventilador' : 'Encender ventilador'}</button>
                </div>
                <div id="card-group">
                    <Velocimetro unidad={unidad} titulo={titulo} porcentaje={porcentaje} />
                </div>
            </div>
        </>
    );
}