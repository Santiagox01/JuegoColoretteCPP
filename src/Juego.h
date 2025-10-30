#ifndef JUEGO_H
#define JUEGO_H

#include "Carta.h"
#include "Jugador.h"
#include <random>
#include <string>
#include <fstream>
#include <vector>

constexpr int MAX_CARTAS_POR_FILA = 3;

struct Fila {
    Carta cartaPrincipal;
    std::vector<Carta> cartasAdicionales;
    
    Fila() {}
    Fila(const Carta& c) : cartaPrincipal(c) {}
    
    bool estaLlena() const {
        return cartasAdicionales.size() >= (MAX_CARTAS_POR_FILA - 1);
    }
    
    int totalCartas() const {
        return 1 + cartasAdicionales.size();
    }
};

class Juego {
private:
    std::vector<Carta> mazo;
    std::vector<Jugador> jugadores;
    std::vector<Fila> filas;
    std::vector<bool> jugadoresTomaronFila;
    std::mt19937 rng;
    bool ultimaRondaActivada;
    bool penultimaRonda;
    int colorExcluido;
    bool partidaCargada;
    int proximoJugadorIdx;

public:
    Juego();
    
    void inicializarMazo(int numJugadores);
    void barajar();
    void crearJugadores(int n);
    void repartirCartasIniciales();
    void inicializarFilas();
    
    void ejecutarJuego();
    void ejecutarJuegoConGuardado(const std::string& saveName);
    void mostrarFilas() const;
    void mostrarEstadoJuego() const;
    
    bool guardarPartida(const std::string& filename) const;
    bool cargarPartida(const std::string& filename);
    
private:
    void ejecutarRonda();
    void realizarTurno(int jugadorIdx, bool& tomoFila);
    void calcularYMostrarPuntajesFinales();
    Carta robarCartaDelMazo();
    bool todasFilasLlenas() const;
};

#endif
