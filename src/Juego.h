#ifndef JUEGO_H
#define JUEGO_H

#include "Carta.h"
#include "Jugador.h"
#include <random>
#include <string>
#include <fstream>

class Juego {
private:
    Carta* mazo;
    int mazoSize;
    int deckPos;
    Jugador* jugadores;
    int numJugadores;
    std::mt19937 rng;

public:
    Juego();
    ~Juego();

    void inicializarMazo(int maxValuePerColor);
    void barajar();
    void crearJugadores(int n, int initialHandCap);
    void repartirInicial(int n);
    bool robarCarta(Carta& out);
    void ejecutarJuego();
    void ejecutarJuegoConGuardado(const std::string& saveName);
    void mostrarEstadoMazo() const;
    
    bool guardarPartida(const std::string& filename) const;
    bool cargarPartida(const std::string& filename);
};

#endif
