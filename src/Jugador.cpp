#include "Jugador.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

Jugador::Jugador() : nombre("Jugador"), tieneCartaInicial(false), cartasPlus2(0) {}

Jugador::Jugador(const std::string& n) : nombre(n), tieneCartaInicial(false), cartasPlus2(0) {}

std::string Jugador::getNombre() const { 
    return nombre; 
}

void Jugador::setCartaInicial(const Carta& c) {
    cartaInicial = c;
    tieneCartaInicial = true;
    agregarCarta(c);
}

void Jugador::agregarCarta(const Carta& c) {
    if (c.esPlus2()) {
        cartasPlus2++;
    } else if (!c.esUltimaRonda()) {
        cartasColeccionadas.push_back(c);
    }
}

void Jugador::agregarCartas(const std::vector<Carta>& cartas) {
    for (const auto& c : cartas) {
        agregarCarta(c);
    }
}

int Jugador::contarCartasPorColor(int color) const {
    int count = 0;
    for (const auto& c : cartasColeccionadas) {
        if (c.color == color) {
            count++;
        }
    }
    return count;
}

int calcularPuntosSegunCantidad(int cantidad) {
    switch(cantidad) {
        case 0: return 0;
        case 1: return 1;
        case 2: return 3;
        case 3: return 6;
        case 4: return 10;
        case 5: return 15;
        default: return 21;
    }
}

int Jugador::calcularPuntajeConSeleccion(int col1, int col2, int col3) const {
    int puntaje = 0;
    
    for (int c = 0; c < MAX_COLORS; c++) {
        int cantidad = contarCartasPorColor(c);
        int puntos = calcularPuntosSegunCantidad(cantidad);
        
        if (c == col1 || c == col2 || c == col3) {
            puntaje += puntos;
        } else {
            puntaje -= puntos;
        }
    }
    
    puntaje += (cartasPlus2 * 2);
    
    return puntaje;
}

void Jugador::mostrarCartasColeccionadas() const {
    std::cout << "\n" << nombre << " - Cartas coleccionadas:\n";
    
    for (int c = 0; c < MAX_COLORS; c++) {
        int cantidad = contarCartasPorColor(c);
        std::cout << "  " << std::setw(10) << COLOR_NAMES[c] << ": " << cantidad << " cartas\n";
    }
    
    if (cartasPlus2 > 0) {
        std::cout << "  " << std::setw(10) << "+2" << ": " << cartasPlus2 << " cartas\n";
    }
}

void Jugador::serialize(std::ofstream& out) const {
    out << nombre << "\n";
    out << tieneCartaInicial << "\n";
    if (tieneCartaInicial) {
        out << cartaInicial.color << " " << cartaInicial.valor << " " 
            << cartaInicial.esEspecial << " " << cartaInicial.tipoEspecial << "\n";
    }
    out << cartasPlus2 << "\n";
    out << cartasColeccionadas.size() << "\n";
    for (const auto& c : cartasColeccionadas) {
        out << c.color << " " << c.valor << " " 
            << c.esEspecial << " " << c.tipoEspecial << "\n";
    }
}

void Jugador::deserialize(std::ifstream& in) {
    std::getline(in, nombre);
    in >> tieneCartaInicial;
    in.ignore();
    
    if (tieneCartaInicial) {
        in >> cartaInicial.color >> cartaInicial.valor 
           >> cartaInicial.esEspecial >> cartaInicial.tipoEspecial;
        in.ignore();
    }
    
    in >> cartasPlus2;
    in.ignore();
    
    size_t numCartas;
    in >> numCartas;
    in.ignore();
    
    cartasColeccionadas.clear();
    for (size_t i = 0; i < numCartas; i++) {
        Carta c;
        in >> c.color >> c.valor >> c.esEspecial >> c.tipoEspecial;
        in.ignore();
        cartasColeccionadas.push_back(c);
    }
}
