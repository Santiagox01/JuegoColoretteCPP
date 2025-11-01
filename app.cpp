#include <iostream>
#include <limits>
#include "Juego.h"
#include "SaveManager.h"

void mostrarMenu() {
    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "        COLORETTE - Menú Principal      \n";
    std::cout << "========================================\n";
    std::cout << "1. Nueva Partida\n";
    std::cout << "2. Cargar Partida\n";
    std::cout << "3. Administrar Partidas Guardadas\n";
    std::cout << "4. Salir\n";
    std::cout << "========================================\n";
    std::cout << "Selecciona una opción: ";
}

void mostrarReglas() {
    std::cout << "\n========================================\n";
    std::cout << "         REGLAS DE COLORETTE            \n";
    std::cout << "========================================\n\n";
    std::cout << "OBJETIVO: Conseguir la mayor puntuación coleccionando cartas de colores.\n\n";
    std::cout << "PREPARACIÓN:\n";
    std::cout << "- Se juega con 3-5 jugadores (todos humanos, turnos locales)\n";
    std::cout << "- Con 3 jugadores, se retira un color completo del mazo\n";
    std::cout << "- Cada jugador toma una carta de color distinto al inicio\n";
    std::cout << "- Se colocan tantas cartas de fila como jugadores haya\n\n";
    std::cout << "TURNO:\n";
    std::cout << "En tu turno puedes:\n";
    std::cout << "1. Robar la carta superior del mazo y colocarla junto a una fila, O\n";
    std::cout << "2. Tomar una fila completa (la carta principal + cartas adicionales)\n\n";
    std::cout << "RONDAS:\n";
    std::cout << "- Cuando todos toman una fila, inicia nueva ronda\n";
    std::cout << "- Hay una carta 'ÚLTIMA RONDA' que marca la penúltima ronda\n\n";
    std::cout << "PUNTUACIÓN FINAL:\n";
    std::cout << "- Cada jugador elige 3 colores como POSITIVOS\n";
    std::cout << "- Los demás colores cuentan como NEGATIVOS\n";
    std::cout << "- Tabla de puntos (Lado Café):\n";
    std::cout << "  1 carta = 1 punto\n";
    std::cout << "  2 cartas = 3 puntos\n";
    std::cout << "  3 cartas = 6 puntos\n";
    std::cout << "  4 cartas = 10 puntos\n";
    std::cout << "  5 cartas = 15 puntos\n";
    std::cout << "  6+ cartas = 21 puntos\n";
    std::cout << "- Cartas +2 suman 2 puntos extra\n\n";
    std::cout << "========================================\n\n";
}

void nuevaPartida() {
    std::cout << "\n=== Nueva Partida ===\n";
    
    int numPlayers;
    std::cout << "Número de jugadores (3-5): ";
    std::cin >> numPlayers;
    
    if (numPlayers < 3) numPlayers = 3;
    if (numPlayers > 5) numPlayers = 5;
    
    std::cout << "Jugando con " << numPlayers << " jugadores (todos humanos).\n";
    
    std::cout << "Nombre para esta partida (sin espacios): ";
    std::string saveName;
    std::cin >> saveName;

    Juego juego;
    juego.inicializarMazo(numPlayers);
    juego.barajar();
    juego.crearJugadores(numPlayers);
    
    mostrarReglas();
    
    std::cout << "Presiona Enter para comenzar...";
    std::cin.ignore();
    std::cin.get();
    
    juego.repartirCartasIniciales();
    juego.ejecutarJuegoConGuardado(saveName);
    
    std::cout << "\n¡Gracias por jugar!\n";
}

void cargarPartida() {
    std::cout << "\n=== Cargar Partida ===\n";
    
    auto partidas = SaveManager::listarPartidas();
    
    if (partidas.empty()) {
        std::cout << "No hay partidas guardadas.\n";
        return;
    }
    
    std::cout << "Partidas disponibles:\n";
    for (size_t i = 0; i < partidas.size(); ++i) {
        std::cout << (i + 1) << ". " << partidas[i] << "\n";
    }
    
    std::cout << "\nSelecciona el número de la partida a cargar (0 para cancelar): ";
    int seleccion;
    std::cin >> seleccion;
    
    if (seleccion <= 0 || seleccion > (int)partidas.size()) {
        std::cout << "Operación cancelada.\n";
        return;
    }
    
    std::string nombrePartida = partidas[seleccion - 1];
    std::string filepath = "saves/" + nombrePartida + ".txt";
    
    Juego juego;
    if (juego.cargarPartida(filepath)) {
        std::cout << "Partida cargada exitosamente.\n";
        std::cout << "\nContinuando partida...\n";
        juego.ejecutarJuegoConGuardado(nombrePartida);
        std::cout << "\n¡Gracias por jugar!\n";
    } else {
        std::cout << "Error al cargar la partida.\n";
    }
}

void administrarPartidas() {
    while (true) {
        std::cout << "\n=== Administrar Partidas ===\n";
        
        auto partidas = SaveManager::listarPartidas();
        
        if (partidas.empty()) {
            std::cout << "No hay partidas guardadas.\n";
            return;
        }
        
        std::cout << "Partidas guardadas:\n";
        for (size_t i = 0; i < partidas.size(); ++i) {
            std::cout << (i + 1) << ". " << partidas[i] << "\n";
        }
        
        std::cout << "\nOpciones:\n";
        std::cout << "1. Eliminar una partida\n";
        std::cout << "2. Volver al menú principal\n";
        std::cout << "Selecciona una opción: ";
        
        int opcion;
        std::cin >> opcion;
        
        if (opcion == 1) {
            std::cout << "Ingresa el número de la partida a eliminar (0 para cancelar): ";
            int seleccion;
            std::cin >> seleccion;
            
            if (seleccion > 0 && seleccion <= (int)partidas.size()) {
                if (SaveManager::eliminarPartida(partidas[seleccion - 1])) {
                    std::cout << "Partida eliminada exitosamente.\n";
                } else {
                    std::cout << "Error al eliminar la partida.\n";
                }
            }
        } else if (opcion == 2) {
            break;
        }
    }
}

int main() {
    std::cout << "=== COLORETTE ===\n";
    std::cout << "Juego de cartas de colores\n";
    std::cout << "Todos los jugadores son humanos (juego local por turnos)\n";
    
    while (true) {
        mostrarMenu();
        
        int opcion;
        std::cin >> opcion;
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Opción inválida. Intenta de nuevo.\n";
            continue;
        }
        
        switch (opcion) {
            case 1:
                nuevaPartida();
                break;
            case 2:
                cargarPartida();
                break;
            case 3:
                administrarPartidas();
                break;
            case 4:
                std::cout << "\n¡Hasta luego!\n";
                return 0;
            default:
                std::cout << "Opción inválida. Intenta de nuevo.\n";
        }
    }
    
    return 0;
}