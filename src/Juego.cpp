#include "Juego.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <fstream>
#include <limits>

Juego::Juego() : ultimaRondaActivada(false), penultimaRonda(false), colorExcluido(-1), partidaCargada(false), proximoJugadorIdx(0) {
    std::random_device rd;
    rng.seed(static_cast<unsigned int>(rd()));
}

void Juego::inicializarMazo(int numJugadores) {
    mazo.clear();
    
    if (numJugadores == 3) {
        std::uniform_int_distribution<int> dist(0, MAX_COLORS - 1);
        colorExcluido = dist(rng);
        std::cout << "\n[3 jugadores] Se excluye del mazo el color: " 
                  << COLOR_NAMES[colorExcluido] << "\n";
    }
    
    for (int c = 0; c < MAX_COLORS; c++) {
        if (c == colorExcluido) continue;
        
        for (int v = 1; v <= 5; v++) {
            mazo.push_back(Carta(c, v));
        }
    }
    
    for (int i = 0; i < 3; i++) {
        mazo.push_back(Carta::crearPlus2());
    }
    
    mazo.push_back(Carta::crearUltimaRonda());
}

void Juego::barajar() {
    std::shuffle(mazo.begin(), mazo.end(), rng);
}

void Juego::crearJugadores(int n) {
    jugadores.clear();
    jugadoresTomaronFila.clear();
    for (int i = 0; i < n; i++) {
        jugadores.push_back(Jugador("Jugador " + std::to_string(i + 1)));
        jugadoresTomaronFila.push_back(false);
    }
}

void Juego::repartirCartasIniciales() {
    std::cout << "\n=== Repartiendo cartas iniciales ===\n";
    std::cout << "Cada jugador debe tomar una carta de color distinto del mazo.\n\n";
    
    std::vector<int> coloresUsados;
    
    for (size_t i = 0; i < jugadores.size(); i++) {
        std::cout << jugadores[i].getNombre() << ", elige un color que nadie haya tomado:\n";
        
        std::cout << "Colores disponibles:\n";
        for (int c = 0; c < MAX_COLORS; c++) {
            if (c == colorExcluido) continue;
            bool usado = std::find(coloresUsados.begin(), coloresUsados.end(), c) != coloresUsados.end();
            if (!usado) {
                std::cout << "  " << c << ". " << COLOR_NAMES[c] << "\n";
            }
        }
        
        int colorElegido;
        std::cout << "Ingresa el número del color: ";
        std::cin >> colorElegido;
        
        while (colorElegido < 0 || colorElegido >= MAX_COLORS || 
               colorElegido == colorExcluido ||
               std::find(coloresUsados.begin(), coloresUsados.end(), colorElegido) != coloresUsados.end()) {
            std::cout << "Color inválido o ya tomado. Intenta de nuevo: ";
            std::cin >> colorElegido;
        }
        
        auto it = std::find_if(mazo.begin(), mazo.end(), [colorElegido](const Carta& c) {
            return c.color == colorElegido && !c.esEspecial;
        });
        
        if (it != mazo.end()) {
            Carta cartaInicial = *it;
            mazo.erase(it);
            jugadores[i].setCartaInicial(cartaInicial);
            coloresUsados.push_back(colorElegido);
            
            std::cout << jugadores[i].getNombre() << " tomó del mazo: " << cartaInicial.toString() << "\n\n";
        } else {
            std::cout << "ERROR: No se encontró carta del color elegido en el mazo.\n";
        }
    }
}

void Juego::inicializarFilas() {
    filas.clear();
    for (size_t i = 0; i < jugadores.size(); i++) {
        if (mazo.empty()) break;
        Carta c = robarCartaDelMazo();
        filas.push_back(Fila(c));
    }
}

void Juego::mostrarFilas() const {
    std::cout << "\n--- FILAS DISPONIBLES ---\n";
    for (size_t i = 0; i < filas.size(); i++) {
        std::cout << "Fila " << i << ": " << filas[i].cartaPrincipal.toString();
        if (!filas[i].cartasAdicionales.empty()) {
            std::cout << " + [";
            for (size_t j = 0; j < filas[i].cartasAdicionales.size(); j++) {
                if (j > 0) std::cout << ", ";
                std::cout << filas[i].cartasAdicionales[j].toString();
            }
            std::cout << "]";
        }
        std::cout << " (" << filas[i].totalCartas() << "/" << MAX_CARTAS_POR_FILA << " cartas)";
        if (filas[i].estaLlena()) {
            std::cout << " [LLENA]";
        }
        std::cout << "\n";
    }
    std::cout << "-------------------------\n";
}

void Juego::mostrarEstadoJuego() const {
    std::cout << "\n=== ESTADO DEL JUEGO ===\n";
    std::cout << "Cartas restantes en mazo: " << mazo.size() << "\n";
    if (penultimaRonda) {
        std::cout << "*** PENÚLTIMA RONDA ***\n";
    } else if (ultimaRondaActivada) {
        std::cout << "*** ÚLTIMA RONDA ***\n";
    }
    mostrarFilas();
}

Carta Juego::robarCartaDelMazo() {
    if (mazo.empty()) {
        return Carta();
    }
    Carta c = mazo.back();
    mazo.pop_back();
    return c;
}

bool Juego::todasFilasLlenas() const {
    for (const auto& fila : filas) {
        if (!fila.estaLlena()) {
            return false;
        }
    }
    return true;
}

void Juego::realizarTurno(int jugadorIdx, bool& tomoFila) {
    std::cout << "\n========================================\n";
    std::cout << "Turno de: " << jugadores[jugadorIdx].getNombre() << "\n";
    std::cout << "========================================\n";
    
    mostrarEstadoJuego();
    
    bool debeTomarFila = todasFilasLlenas() || mazo.empty();
    
    if (debeTomarFila) {
        std::cout << "\n*** Todas las filas están llenas o el mazo está vacío ***\n";
        std::cout << "DEBES tomar una fila.\n\n";
        
        if (filas.empty()) {
            std::cout << "\nNo hay filas disponibles. Fin del turno.\n";
            tomoFila = true;
            return;
        }
        
        mostrarFilas();
        std::cout << "\n¿Qué fila quieres tomar? (0-" << (filas.size() - 1) << "): ";
        int filaIdx;
        std::cin >> filaIdx;
        
        while (filaIdx < 0 || filaIdx >= (int)filas.size()) {
            std::cout << "Fila inválida. Elige entre 0 y " << (filas.size() - 1) << ": ";
            std::cin >> filaIdx;
        }
        
        Fila filaElegida = filas[filaIdx];
        
        std::cout << "\n" << jugadores[jugadorIdx].getNombre() << " tomó la fila " << filaIdx << ":\n";
        std::cout << "  - " << filaElegida.cartaPrincipal.toString() << "\n";
        jugadores[jugadorIdx].agregarCarta(filaElegida.cartaPrincipal);
        
        for (const auto& c : filaElegida.cartasAdicionales) {
            std::cout << "  - " << c.toString() << "\n";
            jugadores[jugadorIdx].agregarCarta(c);
        }
        
        filas.erase(filas.begin() + filaIdx);
        tomoFila = true;
        
        std::cout << "\nFilas restantes: " << filas.size() << "\n";
        return;
    }
    
    std::cout << "\nOpciones:\n";
    std::cout << "1. Robar carta del mazo y colocarla junto a una fila\n";
    std::cout << "2. Tomar una fila completa\n";
    std::cout << "Elige opción: ";
    
    int opcion;
    std::cin >> opcion;
    
    while (opcion != 1 && opcion != 2) {
        std::cout << "Opción inválida. Elige 1 o 2: ";
        std::cin >> opcion;
    }
    
    if (opcion == 1) {
        Carta cartaRobada = robarCartaDelMazo();
        std::cout << "\nRobaste: " << cartaRobada.toString() << "\n";
        
        if (cartaRobada.esUltimaRonda()) {
            std::cout << "\n*** ¡Carta de ÚLTIMA RONDA revelada! ***\n";
            std::cout << "Esta es la penúltima ronda. La siguiente será la última.\n";
            penultimaRonda = true;
        }
        
        mostrarFilas();
        
        std::vector<int> filasDisponibles;
        for (size_t i = 0; i < filas.size(); i++) {
            if (!filas[i].estaLlena()) {
                filasDisponibles.push_back(i);
            }
        }
        
        if (filasDisponibles.empty()) {
            std::cout << "\n¡Todas las filas están llenas! No puedes colocar la carta.\n";
            std::cout << "La carta " << cartaRobada.toString() << " se descarta.\n";
            tomoFila = false;
            return;
        }
        
        std::cout << "\nFilas disponibles (no llenas): ";
        for (size_t i = 0; i < filasDisponibles.size(); i++) {
            if (i > 0) std::cout << ", ";
            std::cout << filasDisponibles[i];
        }
        std::cout << "\n";
        
        std::cout << "¿Junto a qué fila quieres colocar esta carta?: ";
        int filaIdx;
        std::cin >> filaIdx;
        
        while (std::find(filasDisponibles.begin(), filasDisponibles.end(), filaIdx) == filasDisponibles.end()) {
            std::cout << "Fila inválida o llena. Elige una fila disponible: ";
            std::cin >> filaIdx;
        }
        
        filas[filaIdx].cartasAdicionales.push_back(cartaRobada);
        std::cout << "\nCarta colocada junto a la fila " << filaIdx << ".\n";
        
        mostrarFilas();
        tomoFila = false;
    }
    else if (opcion == 2) {
        if (filas.empty()) {
            std::cout << "\nNo hay filas disponibles para tomar.\n";
            tomoFila = true;
            return;
        }
        
        mostrarFilas();
        std::cout << "\n¿Qué fila quieres tomar? (0-" << (filas.size() - 1) << "): ";
        int filaIdx;
        std::cin >> filaIdx;
        
        while (filaIdx < 0 || filaIdx >= (int)filas.size()) {
            std::cout << "Fila inválida. Elige entre 0 y " << (filas.size() - 1) << ": ";
            std::cin >> filaIdx;
        }
        
        Fila filaElegida = filas[filaIdx];
        
        std::cout << "\n" << jugadores[jugadorIdx].getNombre() << " tomó la fila " << filaIdx << ":\n";
        std::cout << "  - " << filaElegida.cartaPrincipal.toString() << "\n";
        jugadores[jugadorIdx].agregarCarta(filaElegida.cartaPrincipal);
        
        for (const auto& c : filaElegida.cartasAdicionales) {
            std::cout << "  - " << c.toString() << "\n";
            jugadores[jugadorIdx].agregarCarta(c);
        }
        
        filas.erase(filas.begin() + filaIdx);
        tomoFila = true;
        
        std::cout << "\nFilas restantes: " << filas.size() << "\n";
    }
}

void Juego::ejecutarRonda() {
    std::cout << "\n\n========================================\n";
    std::cout << "===== NUEVA RONDA =====\n";
    std::cout << "========================================\n";
    
    if (!partidaCargada) {
        inicializarFilas();
        for (size_t i = 0; i < jugadoresTomaronFila.size(); i++) {
            jugadoresTomaronFila[i] = false;
        }
        proximoJugadorIdx = 0;
    } else {
        std::cout << "*** Reanudando partida guardada ***\n";
        std::cout << "Próximo turno: " << jugadores[proximoJugadorIdx].getNombre() << "\n";
        partidaCargada = false;
    }
    
    mostrarEstadoJuego();
    
    int jugadoresPendientes = 0;
    for (bool tomo : jugadoresTomaronFila) {
        if (!tomo) jugadoresPendientes++;
    }
    
    while (jugadoresPendientes > 0 && !filas.empty()) {
        for (size_t i = 0; i < jugadores.size(); i++) {
            size_t jugadorIdx = (proximoJugadorIdx + i) % jugadores.size();
            if (jugadoresTomaronFila[jugadorIdx]) continue;
            
            bool tomoFila = false;
            realizarTurno(jugadorIdx, tomoFila);
            
            proximoJugadorIdx = (jugadorIdx + 1) % jugadores.size();
            
            if (tomoFila) {
                jugadoresTomaronFila[jugadorIdx] = true;
                jugadoresPendientes--;
                std::cout << "\n" << jugadores[jugadorIdx].getNombre() << " terminó su participación en esta ronda.\n";
                std::cout << "Jugadores restantes en la ronda: " << jugadoresPendientes << "\n";
            }
            
            if (filas.empty() || jugadoresPendientes == 0) {
                break;
            }
        }
    }
    
    std::cout << "\n=== Fin de la ronda ===\n";
}

void Juego::calcularYMostrarPuntajesFinales() {
    std::cout << "\n\n========================================\n";
    std::cout << "===== FIN DEL JUEGO =====\n";
    std::cout << "========================================\n\n";
    
    std::cout << "=== Cartas coleccionadas por cada jugador ===\n";
    for (auto& jugador : jugadores) {
        jugador.mostrarCartasColeccionadas();
    }
    
    std::cout << "\n\n=== SELECCIÓN DE COLORES POSITIVOS ===\n";
    std::cout << "Cada jugador debe elegir 3 colores que contarán como puntos POSITIVOS.\n";
    std::cout << "Los demás colores contarán como puntos NEGATIVOS.\n\n";
    
    std::vector<int> puntajes;
    
    for (auto& jugador : jugadores) {
        std::cout << "\n" << jugador.getNombre() << ", elige tus 3 colores positivos:\n";
        std::cout << "Colores disponibles:\n";
        for (int c = 0; c < MAX_COLORS; c++) {
            if (c == colorExcluido) continue;
            int cantidad = jugador.contarCartasPorColor(c);
            std::cout << "  " << c << ". " << COLOR_NAMES[c] << " (" << cantidad << " cartas)\n";
        }
        
        int col1, col2, col3;
        std::cout << "Primer color (número): ";
        std::cin >> col1;
        std::cout << "Segundo color (número): ";
        std::cin >> col2;
        std::cout << "Tercer color (número): ";
        std::cin >> col3;
        
        int puntaje = jugador.calcularPuntajeConSeleccion(col1, col2, col3);
        puntajes.push_back(puntaje);
        
        std::cout << "\n" << jugador.getNombre() << " eligió: " 
                  << COLOR_NAMES[col1] << ", " 
                  << COLOR_NAMES[col2] << ", " 
                  << COLOR_NAMES[col3] << "\n";
        std::cout << "Puntaje final: " << puntaje << " puntos\n";
    }
    
    std::cout << "\n\n========================================\n";
    std::cout << "===== PUNTAJES FINALES =====\n";
    std::cout << "========================================\n";
    
    int mejorPuntaje = std::numeric_limits<int>::min();
    std::string ganador;
    
    for (size_t i = 0; i < jugadores.size(); i++) {
        std::cout << jugadores[i].getNombre() << ": " << puntajes[i] << " puntos\n";
        
        if (puntajes[i] > mejorPuntaje) {
            mejorPuntaje = puntajes[i];
            ganador = jugadores[i].getNombre();
        }
    }
    
    std::cout << "\n\n*** ¡GANADOR: " << ganador << " con " << mejorPuntaje << " puntos! ***\n\n";
}

void Juego::ejecutarJuego() {
    while (!ultimaRondaActivada) {
        if (penultimaRonda) {
            std::cout << "\n\n*** ESTA ES LA ÚLTIMA RONDA ***\n\n";
            ultimaRondaActivada = true;
        }
        
        ejecutarRonda();
        
        if (mazo.empty()) {
            std::cout << "\nEl mazo se agotó. El juego termina.\n";
            break;
        }
    }
    
    calcularYMostrarPuntajesFinales();
}

void Juego::ejecutarJuegoConGuardado(const std::string& saveName) {
    while (!ultimaRondaActivada) {
        if (penultimaRonda) {
            std::cout << "\n\n*** ESTA ES LA ÚLTIMA RONDA ***\n\n";
            ultimaRondaActivada = true;
        }
        
        ejecutarRonda();
        
        if (guardarPartida("saves/" + saveName + ".txt")) {
            std::cout << "\n[Auto-guardado realizado]\n";
        } else {
            std::cout << "\n[Error en auto-guardado]\n";
        }
        
        if (mazo.empty()) {
            std::cout << "\nEl mazo se agotó. El juego termina.\n";
            break;
        }
    }
    
    calcularYMostrarPuntajesFinales();
}

bool Juego::guardarPartida(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out) return false;
    
    out << "COLORETTE_V2\n";
    out << mazo.size() << "\n";
    for (const auto& c : mazo) {
        out << c.color << " " << c.valor << " " << c.esEspecial << " " << c.tipoEspecial << "\n";
    }
    
    out << jugadores.size() << "\n";
    for (const auto& j : jugadores) {
        j.serialize(out);
    }
    
    out << filas.size() << "\n";
    for (const auto& f : filas) {
        out << f.cartaPrincipal.color << " " << f.cartaPrincipal.valor << " " 
            << f.cartaPrincipal.esEspecial << " " << f.cartaPrincipal.tipoEspecial << "\n";
        out << f.cartasAdicionales.size() << "\n";
        for (const auto& c : f.cartasAdicionales) {
            out << c.color << " " << c.valor << " " << c.esEspecial << " " << c.tipoEspecial << "\n";
        }
    }
    
    out << jugadoresTomaronFila.size() << "\n";
    for (bool tomo : jugadoresTomaronFila) {
        out << tomo << " ";
    }
    out << "\n";
    
    out << ultimaRondaActivada << " " << penultimaRonda << " " << colorExcluido << " " << proximoJugadorIdx << "\n";
    
    out.close();
    return true;
}

bool Juego::cargarPartida(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) return false;
    
    std::string header;
    std::getline(in, header);
    if (header != "COLORETTE_V2") return false;
    
    size_t mazoSize;
    in >> mazoSize;
    in.ignore();
    
    mazo.clear();
    for (size_t i = 0; i < mazoSize; i++) {
        Carta c;
        in >> c.color >> c.valor >> c.esEspecial >> c.tipoEspecial;
        in.ignore();
        mazo.push_back(c);
    }
    
    size_t numJugadores;
    in >> numJugadores;
    in.ignore();
    
    jugadores.clear();
    for (size_t i = 0; i < numJugadores; i++) {
        Jugador j;
        j.deserialize(in);
        jugadores.push_back(j);
    }
    
    size_t numFilas;
    in >> numFilas;
    in.ignore();
    
    filas.clear();
    for (size_t i = 0; i < numFilas; i++) {
        Carta principal;
        in >> principal.color >> principal.valor >> principal.esEspecial >> principal.tipoEspecial;
        in.ignore();
        
        Fila f(principal);
        
        size_t numAdicionales;
        in >> numAdicionales;
        in.ignore();
        
        for (size_t j = 0; j < numAdicionales; j++) {
            Carta c;
            in >> c.color >> c.valor >> c.esEspecial >> c.tipoEspecial;
            in.ignore();
            f.cartasAdicionales.push_back(c);
        }
        
        filas.push_back(f);
    }
    
    size_t numEstados;
    in >> numEstados;
    in.ignore();
    
    jugadoresTomaronFila.clear();
    for (size_t i = 0; i < numEstados; i++) {
        bool tomo;
        in >> tomo;
        jugadoresTomaronFila.push_back(tomo);
    }
    in.ignore();
    
    in >> ultimaRondaActivada >> penultimaRonda >> colorExcluido >> proximoJugadorIdx;
    in.ignore();
    
    partidaCargada = true;
    
    in.close();
    return true;
}