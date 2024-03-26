import React from "react";
//import './styles/Navbar.css';

export default function Navbar() {


    return (
        <>
            <nav className="navbar navbar-expand-lg navbar-dark fixed-top bg-dark">
                <a className="navbar-brand">Group 4</a>
                <button className="navbar-toggler" type="button" data-toggle="collapse" data-target="#navbarCollapse"
                    aria-controls="navbarCollapse" aria-expanded="false" aria-label="Toggle navigation">
                    <span className="navbar-toggler-icon"></span>
                </button>

                <div className="collapse navbar-collapse" id="navbarCollapse">
                    <ul className="navbar-nav mr-auto sidenav" id="navAccordion">
                        <li className="nav-item active">
                            <a className="nav-link" href="/">Dashboard <span className="sr-only">(current)</span></a>
                        </li>
                        <li className="nav-item">
                            <a className="nav-link">Item 1</a>
                        </li>
                        <li className="nav-item">
                            <a className="nav-link">Item 2</a>
                        </li>
                    </ul>
                </div>
            </nav>
        </>
    );

} 