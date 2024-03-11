import { Line } from 'react-chartjs-2';
import {
    Chart as ChartJS,
    CategoryScale,
    LinearScale,
    PointElement,
    LineElement,
    Title,
    Tooltip,
    Legend,
    Filler,
} from 'chart.js';

import './styles/general.css';

ChartJS.register(
    CategoryScale,
    LinearScale,
    PointElement,
    LineElement,
    Title,
    Tooltip,
    Legend,
    Filler
);

export default function LinesChart({titulo, data, label}) {

    var beneficios = [0, 56, 20, 36, 80, 40, 30, -20, 25, 30, 12, 60];
    var meses = ["Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio", "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"];

    var midata = {
        labels: meses,
        datasets: [ // Cada una de las líneas del gráfico
            {
                label: label,
                data: beneficios,
                tension: 0.5,
                fill: true,
                borderColor: '#5BE12C',
                backgroundColor: '#5BE12C',
                pointRadius: 5,
                pointBorderColor: '#5BE12C',
                pointBackgroundColor: '#5BE12C',
            },
        ],
    };

    var misoptions = {
        scales: {
            y: {
                ticks: { color: 'white' },
                min: 0
            },
            x: {
                ticks: { color: 'white' }
            }
        },
        label: { color: 'white' },
    };

    return (
        <>
            <div className="card-border">
                <div className="card-main-content">
                    <Line data={midata} options={misoptions} />
                </div>
                <div className="card-info">
                    <p> {titulo} </p>
                </div>
            </div>
        </>
    );
}