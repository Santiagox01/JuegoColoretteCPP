#include "Juego.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <fstream>

Juego::Juego() : mazo(nullptr), mazoSize(0), deckPos(0), jugadores(nullptr), numJugadores(0) {
    std::random_device rd;
    rng.seed(static_cast<unsigned int>(rd()));
}

Juego::~Juego() {
    if (mazo) delete[] mazo;
    if (jugadores) delete[] jugadores;
}

void Juego::inicializarMazo(int maxValuePerColor) {
    mazoSize = MAX_COLORS * maxValuePerColor;
    mazo = new Carta[mazoSize];
    int idx = 0;
    for (int c = 0; c < MAX_COLORS; ++c) {
        for (int v = 1; v <= maxValuePerColor; ++v) {
            mazo[idx++] = Carta(c, v);
        }
    }
    deckPos = 0;
}

void Juego::barajar() {
    for (int i = mazoSize - 1; i > 0; --i) {
        std::uniform_int_distribution<int> dist(0, i);
        int j = dist(rng);
        std::swap(mazo[i], mazo[j]);
    }
}

void Juego::crearJugadores(int n, int initialHandCap) {
    numJugadores = n;
    jugadores = new Jugador[numJugadores];
    for (int i = 0; i < numJugadores; ++i) {
        jugadores[i].~Jugador();
        new (&jugadores[i]) Jugador("Jugador " + std::to_string(i + 1), initialHandCap);
    }
}

void Juego::repartirInicial(int n) {
    for (int r = 0; r < n; ++r) {
        for (int p = 0; p < numJugadores; ++p) {
            Carta c;
            if (robarCarta(c)) {
                jugadores[p].addCartaAMano(c);
            }
        }
    }
}

bool Juego::robarCarta(Carta& out) {
    if (deckPos >= mazoSize) return false;
    out = mazo[deckPos++];
    return true;
}

void Juego::ejecutarJuego() {
    int turno = 0;
    
    while (true) {
        bool alguienConCartas = false;
        
        for (int p = 0; p < numJugadores; ++p) {
            std::cout << "\n--------------------------\n";
            std::cout << jugadores[p].getNombre() << " turno.\n";

            // Robar carta
            Carta robada;
            if (robarCarta(robada)) {
                std::cout << "  Robó: " << robada.toString() << "\n";
                jugadores[p].addCartaAMano(robada);
            } else {
                std::cout << "  Mazo vacío.\n";
            }

            jugadores[p].mostrarMano();

            if (jugadores[p].tieneCartasEnMano()) {
                int cartaIdx;
                
                // Si es el Jugador 1 (humano), pedir input
                if (p == 0) {
                    std::cout << "¿Qué carta quieres jugar? (ingresa el número): ";
                    std::cin >> cartaIdx;
                    
                    // Validar entrada
                    while (cartaIdx < 0 || cartaIdx >= jugadores[p].manoSize) {
                        std::cout << "Opción inválida. Elige entre 0 y " << (jugadores[p].manoSize - 1) << ": ";
                        std::cin >> cartaIdx;
                    }
                } else {
                    // IA: Jugar la carta con mayor valor
                    cartaIdx = 0;
                    int mejorVal = jugadores[p].mano[0].valor;
                    for (int i = 1; i < jugadores[p].manoSize; ++i) {
                        if (jugadores[p].mano[i].valor > mejorVal) {
                            mejorVal = jugadores[p].mano[i].valor;
                            cartaIdx = i;
                        }
                    }
                }

                Carta aJugar = jugadores[p].mano[cartaIdx];
                jugadores[p].jugarCarta(cartaIdx);
                std::cout << "  Jugó: " << aJugar.toString() << " -> a su pila " << COLOR_NAMES[aJugar.color] << "\n";
            } else {
                std::cout << "  No tiene cartas para jugar.\n";
            }

            jugadores[p].mostrarPilas();
            if (jugadores[p].tieneCartasEnMano()) alguienConCartas = true;
        }

        // Condición de parada
        if (deckPos >= mazoSize && !alguienConCartas) break;

        ++turno;
        if (turno > 10000) {
            std::cerr << "Error: demasiados turnos, abortando.\n";
            break;
        }
    }

    // Mostrar puntajes finales
    std::cout << "\n=== Puntajes Finales ===\n";
    for (int p = 0; p < numJugadores; ++p) {
        std::cout << jugadores[p].getNombre() << " puntaje: " << jugadores[p].calcularPuntaje() << "\n";
    }
}

void Juego::mostrarEstadoMazo() const {
    std::cout << "Cartas en mazo restantes: " << (mazoSize - deckPos) << " / " << mazoSize << "\n";
}

void Juego::ejecutarJuegoConGuardado(const std::string& saveName) {
    int turno = 0;
    
    while (true) {
        bool alguienConCartas = false;
        
        for (int p = 0; p < numJugadores; ++p) {
            std::cout << "\n--------------------------\n";
            std::cout << jugadores[p].getNombre() << " turno.\n";

            if (p == 0 && jugadores[p].tieneCartasEnMano()) {
                std::cout << "¿Quieres guardar antes de continuar? (1=sí, 0=no): ";
                int quiereGuardar;
                std::cin >> quiereGuardar;
                
                if (quiereGuardar == 1) {
                    if (guardarPartida("saves/" + saveName + ".txt")) {
                        std::cout << "Partida guardada exitosamente.\n";
                    } else {
                        std::cout << "Error al guardar la partida.\n";
                    }
                    std::cout << "¿Continuar jugando? (1=sí, 0=no): ";
                    int continuar;
                    std::cin >> continuar;
                    if (!continuar) return;
                }
            }

            Carta robada;
            if (robarCarta(robada)) {
                std::cout << "  Robó: " << robada.toString() << "\n";
                jugadores[p].addCartaAMano(robada);
            } else {
                std::cout << "  Mazo vacío.\n";
            }

            jugadores[p].mostrarMano();

            if (jugadores[p].tieneCartasEnMano()) {
                int cartaIdx;
                
                if (p == 0) {
                    std::cout << "¿Qué carta quieres jugar? (ingresa el número): ";
                    std::cin >> cartaIdx;
                    
                    while (cartaIdx < 0 || cartaIdx >= jugadores[p].manoSize) {
                        std::cout << "Opción inválida. Elige entre 0 y " << (jugadores[p].manoSize - 1) << ": ";
                        std::cin >> cartaIdx;
                    }
                } else {
                    cartaIdx = 0;
                    int mejorVal = jugadores[p].mano[0].valor;
                    for (int i = 1; i < jugadores[p].manoSize; ++i) {
                        if (jugadores[p].mano[i].valor > mejorVal) {
                            mejorVal = jugadores[p].mano[i].valor;
                            cartaIdx = i;
                        }
                    }
                }

                Carta aJugar = jugadores[p].mano[cartaIdx];
                jugadores[p].jugarCarta(cartaIdx);
                std::cout << "  Jugó: " << aJugar.toString() << " -> a su pila " << COLOR_NAMES[aJugar.color] << "\n";
            } else {
                std::cout << "  No tiene cartas para jugar.\n";
            }

            jugadores[p].mostrarPilas();
            if (jugadores[p].tieneCartasEnMano()) alguienConCartas = true;
        }

        if (deckPos >= mazoSize && !alguienConCartas) break;

        ++turno;
        if (turno > 10000) {
            std::cerr << "Error: demasiados turnos, abortando.\n";
            break;
        }
    }

    std::cout << "\n=== Puntajes Finales ===\n";
    for (int p = 0; p < numJugadores; ++p) {
        std::cout << jugadores[p].getNombre() << " puntaje: " << jugadores[p].calcularPuntaje() << "\n";
    }
}

bool Juego::guardarPartida(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out) return false;
    
    out << "COLORETTE_V1\n";
    out << mazoSize << " " << deckPos << "\n";
    
    for (int i = 0; i < mazoSize; ++i) {
        out << mazo[i].color << " " << mazo[i].valor << "\n";
    }
    
    out << numJugadores << "\n";
    for (int i = 0; i < numJugadores; ++i) {
        jugadores[i].serialize(out);
    }
    
    out.close();
    return true;
}

bool Juego::cargarPartida(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) return false;
    
    std::string header;
    std::getline(in, header);
    if (header != "COLORETTE_V1") return false;
    
    in >> mazoSize >> deckPos;
    in.ignore();
    
    if (mazo) delete[] mazo;
    mazo = new Carta[mazoSize];
    
    for (int i = 0; i < mazoSize; ++i) {
        int color, valor;
        in >> color >> valor;
        mazo[i] = Carta(color, valor);
    }
    in.ignore();
    
    in >> numJugadores;
    in.ignore();
    
    if (jugadores) delete[] jugadores;
    jugadores = new Jugador[numJugadores];
    
    for (int i = 0; i < numJugadores; ++i) {
        jugadores[i].deserialize(in, 10);
    }
    
    in.close();
    return true;
}
