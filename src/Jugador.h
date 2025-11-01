#ifndef JUGADOR_H
#define JUGADOR_H

#include "Carta.h"
#include <string>
#include <fstream>
#include <vector>

class Jugador {
private:
    std::string nombre;
    std::vector<Carta> cartasColeccionadas;
    Carta cartaInicial;
    bool tieneCartaInicial;
    int cartasPlus2;

public:
    Jugador();
    Jugador(const std::string& n);
    
    std::string getNombre() const;
    void setCartaInicial(const Carta& c);
    void agregarCarta(const Carta& c);
    void agregarCartas(const std::vector<Carta>& cartas);
    
    int contarCartasPorColor(int color) const;
    int calcularPuntajeConSeleccion(int col1, int col2, int col3) const;
    void mostrarCartasColeccionadas() const;
    
    void serialize(std::ofstream& out) const;
    void deserialize(std::ifstream& in);
};

#endif