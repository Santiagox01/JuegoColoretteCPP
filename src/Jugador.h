#ifndef JUGADOR_H
#define JUGADOR_H

#include "Carta.h"
#include <string>
#include <fstream>

class Jugador {
private:
    std::string nombre;

public:
    Carta* mano;
    int manoSize;
    int manoCap;
    int* pilesSum;

    Jugador();
    Jugador(const std::string& n, int initialHandCap);
    ~Jugador();

    std::string getNombre() const;
    void addCartaAMano(const Carta& c);
    bool jugarCarta(int idx);
    void mostrarMano() const;
    void mostrarPilas() const;
    int calcularPuntaje() const;
    bool tieneCartasEnMano() const;
    
    void serialize(std::ofstream& out) const;
    void deserialize(std::ifstream& in, int initialHandCap);
};

#endif
