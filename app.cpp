#include <iostream>
#include <limits>
#include "src/Juego.h"
#include "src/SaveManager.h"

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

void nuevaPartida() {
    std::cout << "\n=== Nueva Partida ===\n";
    std::cout << "Tú eres el Jugador 1. Jugarás contra oponentes automáticos o jugadores locales.\n\n";

    int numPlayers;
    std::cout << "Número de jugadores (3-5): ";
    std::cin >> numPlayers;
    
    if (numPlayers < 3) numPlayers = 3;
    if (numPlayers > 5) numPlayers = 5;
    
    std::cout << "Jugando con " << numPlayers << " jugadores.\n";
    
    std::cout << "Nombre para esta partida (sin espacios): ";
    std::string saveName;
    std::cin >> saveName;
    
    int maxValuePerColor = 9;

    Juego juego;
    juego.inicializarMazo(maxValuePerColor);
    juego.barajar();
    juego.crearJugadores(numPlayers, 5);
    juego.repartirInicial(5);

    juego.mostrarEstadoMazo();
    std::cout << "\nAl inicio de cada turno tuyo, se te preguntará si quieres guardar la partida.\n";
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
        std::cout << "Al inicio de cada turno tuyo, se te preguntará si quieres guardar la partida.\n";
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
