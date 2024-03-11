import React from "react";
//import './styles/Navbar.css';

export default function Navbar() {


    return (
        <>
            <nav className="navbar navbar-expand-lg navbar-dark fixed-top bg-dark">
                <a className="navbar-brand" href="#">Group 4</a>
                <button className="navbar-toggler" type="button" data-toggle="collapse" data-target="#navbarCollapse"
                    aria-controls="navbarCollapse" aria-expanded="false" aria-label="Toggle navigation">
                    <span className="navbar-toggler-icon"></span>
                </button>

                <div className="collapse navbar-collapse" id="navbarCollapse">
                    <ul className="navbar-nav mr-auto sidenav" id="navAccordion">
                        <li className="nav-item active">
                            <a className="nav-link" href="#">Dashboard <span className="sr-only">(current)</span></a>
                        </li>
                        <li className="nav-item">
                            <a className="nav-link" href="#">Item 1</a>
                        </li>
                        <li className="nav-item">
                            <a className="nav-link nav-link-collapse" href="#" id="hasSubItems" data-toggle="collapse"
                                data-target="#collapseSubItems2" aria-controls="collapseSubItems2" aria-expanded="false">Item
                                2</a>
                            <ul className="nav-second-level collapse" id="collapseSubItems2" data-parent="#navAccordion">
                                <li className="nav-item">
                                    <a className="nav-link" href="#">
                                        <span className="nav-link-text">Item 2.1</span>
                                    </a>
                                </li>
                                <li className="nav-item">
                                    <a className="nav-link" href="#">
                                        <span className="nav-link-text">Item 2.2</span>
                                    </a>
                                </li>
                            </ul>
                        </li>
                        <li className="nav-item">
                            <a className="nav-link" href="#">Item 3</a>
                        </li>
                        <li className="nav-item">
                            <a className="nav-link nav-link-collapse" href="#" id="hasSubItems" data-toggle="collapse"
                                data-target="#collapseSubItems4" aria-controls="collapseSubItems4" aria-expanded="false">Item
                                4</a>
                            <ul className="nav-second-level collapse" id="collapseSubItems4" data-parent="#navAccordion">
                                <li className="nav-item">
                                    <a className="nav-link" href="#">
                                        <span className="nav-link-text">Item 4.1</span>
                                    </a>
                                </li>
                                <li className="nav-item">
                                    <a className="nav-link" href="#">
                                        <span className="nav-link-text">Item 4.2</span>
                                    </a>
                                </li>
                                <li className="nav-item">
                                    <a className="nav-link" href="#">
                                        <span className="nav-link-text">Item 4.2</span>
                                    </a>
                                </li>
                            </ul>
                        </li>
                        <li className="nav-item">
                            <a className="nav-link" href="#">Item 5</a>
                        </li>
                    </ul>
                </div>
            </nav>
        </>
    );

} 