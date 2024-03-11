import React, { useState, useEffect } from "react";
import GaugeChart from 'react-gauge-chart'
import './styles/general.css';

export default function Velocimetro({ unidad, titulo, porcentaje }) {

  const chartStyle = {
    height: 0,
  }

  useEffect(() => {

  }, []);

  return (
    <>
      <div className="card-border">
        <div className="card-main-content">

          <GaugeChart id="gauge-chart8"
            style={chartStyle}
            nrOfLevels={4}
            colors={['#5BE12C', '#F5CD19', '#EA4228']}
            arcWidth={0.20}
            percent={porcentaje}
            formatTextValue={value => value + unidad} />

        </div>
        <div className="card-info">
          <p>{titulo}</p>
        </div>
      </div>
    </>
  );
}
