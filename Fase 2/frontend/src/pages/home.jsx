import React from "react";
import Navbar from "../components/Navbar";
import Velocimetro from "../components/velocimetro";
import Barras from "../components/linear";
import './styles/home.css';

export default function Home() {


    return (
        <>
            <div className="container-nav">
                <Navbar />
            </div>
            <div className="container-content">
                <div className="home-title">
                    <h1>Dashboard</h1>
                </div>
                <div id="card-group">
                    <Velocimetro unidad={" Lúmenes"} titulo={"Iluminación"} porcentaje={0.26}/>
                    <Velocimetro unidad={" ppm"} titulo={"Cálidad del aire"} porcentaje={0.45}/>
                    <Velocimetro unidad={"%"} titulo={"Humedad"} porcentaje={0.75}/>
                    <Barras titulo={"Temperatura"} label={"Temperatura °C"}/>
                    <Barras titulo={"Movimiento"} label={"Distancia cm"}/>
                </div>
            </div>
        </>
    );
}