// colorette.cpp
// Referencia: Implementación didáctica de "Colorette" en C++
// - Máximo 7 colores
// - Uso explícito de memoria dinámica (new / delete)
// - Clases: Carta, Jugador, Juego
// Compilar: g++ -std=c++17 colorette.cpp -o colorette

#include <iostream>
#include <string>
#include <random>
#include <algorithm>
#include <ctime>
#include <iomanip>

constexpr int MAX_COLORS = 7;

// Simple listado de nombres de colores (índices 0..6)
const std::string COLOR_NAMES[MAX_COLORS] = {
    "Rojo", "Azul", "Verde", "Amarillo", "Morado", "Naranja", "Rosado"
};

// -------------------------
// Clase Carta
// -------------------------
class Carta {
public:
    int color;   // 0 .. MAX_COLORS-1
    int valor;   // valor numérico de la carta

    Carta() : color(0), valor(0) {}
    Carta(int c, int v) : color(c), valor(v) {}

    std::string toString() const {
        return COLOR_NAMES[color] + " " + std::to_string(valor);
    }
};

// -------------------------
// Clase Jugador
// -------------------------
// Cada jugador mantiene una "mano" (arreglo dinámico de Carta) y
// arrays para sus pilas por color (suma de valores por color).
// -------------------------
class Jugador {
private:
    std::string nombre;

public:
    Carta* mano;       // arreglo dinámico para la mano
    int manoSize;
    int manoCap;       // capacidad actual de la mano (para manejo simple)

    // Para este ejemplo, cada jugador tiene su propia suma de cartas por color.
    // pilesSum[color] = suma de valores que el jugador ha jugado de ese color.
    int* pilesSum;     // arreglo dinámico de tamaño MAX_COLORS

    Jugador() : nombre("Jugador"), mano(nullptr), manoSize(0), manoCap(0), pilesSum(nullptr) {}

    Jugador(const std::string& n, int initialHandCap) : nombre(n) {
        manoCap = initialHandCap;
        manoSize = 0;
        mano = new Carta[manoCap];   // memoria dinámica para la mano
        pilesSum = new int[MAX_COLORS]; // memoria dinámica para las pilas por color
        for (int i = 0; i < MAX_COLORS; ++i) pilesSum[i] = 0;
    }

    ~Jugador() {
        // Liberar memoria asignada a la mano y a pilesSum
        if (mano) delete[] mano;
        if (pilesSum) delete[] pilesSum;
    }

    std::string getNombre() const { return nombre; }

    // Añadir carta a la mano (copiando la carta)
    void addCartaAMano(const Carta& c) {
        if (manoSize >= manoCap) {
            // ampliar la mano: crear un nuevo arreglo con doble capacidad
            int nuevaCap = std::max(1, manoCap * 2);
            Carta* nuevo = new Carta[nuevaCap];
            for (int i = 0; i < manoSize; ++i) nuevo[i] = mano[i];
            delete[] mano; // liberar viejo arreglo
            mano = nuevo;
            manoCap = nuevaCap;
        }
        mano[manoSize++] = c;
    }

    // Jugar la carta en posición idx de la mano: mover valor a pilesSum
    // Retorna true si se jugó correctamente.
    bool jugarCarta(int idx) {
        if (idx < 0 || idx >= manoSize) return false;
        Carta c = mano[idx];
        pilesSum[c.color] += c.valor;

        // eliminar carta de la mano desplazando hacia atrás
        for (int i = idx; i + 1 < manoSize; ++i) mano[i] = mano[i + 1];
        --manoSize;
        return true;
    }

    // Mostrar mano por consola
    void mostrarMano() const {
        std::cout << nombre << " mano (" << manoSize << "): ";
        for (int i = 0; i < manoSize; ++i) {
            std::cout << "[" << i << ":" << mano[i].toString() << "] ";
        }
        std::cout << "\n";
    }

    // Mostrar pilas (sumas por color)
    void mostrarPilas() const {
        std::cout << nombre << " pilas:\n";
        for (int c = 0; c < MAX_COLORS; ++c) {
            std::cout << "  " << std::setw(9) << COLOR_NAMES[c] << " -> suma: " << pilesSum[c] << "\n";
        }
    }

    // Calcular puntaje final simple: suma de todas las pilas
    int calcularPuntaje() const {
        int s = 0;
        for (int c = 0; c < MAX_COLORS; ++c) s += pilesSum[c];
        return s;
    }

    // Comprueba si tiene cartas en mano
    bool tieneCartasEnMano() const { return manoSize > 0; }
};

// -------------------------
// Clase Juego
// -------------------------
// Responsable de crear el mazo dinámicamente, barajear, repartir,
// y ejecutar turnos. El mazo se gestiona como un arreglo dinámico
// de Carta con un índice deckPos que apunta a la siguiente carta.
// -------------------------
class Juego {
private:
    Carta* mazo;      // arreglo dinámico del mazo
    int mazoSize;     // número total de cartas en el mazo
    int deckPos;      // índice de la siguiente carta disponible (0..mazoSize-1)

    Jugador* jugadores; // arreglo dinámico de jugadores
    int numJugadores;

    std::mt19937 rng; // generador aleatorio

public:
    Juego() : mazo(nullptr), mazoSize(0), deckPos(0), jugadores(nullptr), numJugadores(0) {
        std::random_device rd;
        rng.seed(static_cast<unsigned int>(rd()));
    }

    ~Juego() {
        // Liberar memoria del mazo y del arreglo de jugadores.
        if (mazo) delete[] mazo;
        if (jugadores) delete[] jugadores;
    }

    // Inicializa el mazo con (MAX_COLORS) colores y valores 1..maxValue
    void inicializarMazo(int maxValuePerColor) {
        mazoSize = MAX_COLORS * maxValuePerColor;
        mazo = new Carta[mazoSize]; // asignación dinámica del mazo
        int idx = 0;
        for (int c = 0; c < MAX_COLORS; ++c) {
            for (int v = 1; v <= maxValuePerColor; ++v) {
                mazo[idx++] = Carta(c, v);
            }
        }
        deckPos = 0;
    }

    // Barajear (Fisher-Yates)
    void barajar() {
        for (int i = mazoSize - 1; i > 0; --i) {
            std::uniform_int_distribution<int> dist(0, i);
            int j = dist(rng);
            std::swap(mazo[i], mazo[j]);
        }
    }

    // Crear jugadores dinámicamente
    void crearJugadores(int n, int initialHandCap) {
        numJugadores = n;
        jugadores = new Jugador[numJugadores]; // arreglo dinámico de Jugador (invoca default ctor)
        for (int i = 0; i < numJugadores; ++i) {
            // Reconstruir cada jugador en su posición con el constructor con parámetros:
            jugadores[i].~Jugador(); // liberar cualquier contenido default (seguro porque era trivial)
            new (&jugadores[i]) Jugador("Jugador " + std::to_string(i + 1), initialHandCap);
        }
    }

    // Repartir n cartas a cada jugador (si hay suficientes cartas)
    void repartirInicial(int n) {
        for (int r = 0; r < n; ++r) {
            for (int p = 0; p < numJugadores; ++p) {
                Carta c;
                if (robarCarta(c)) {
                    jugadores[p].addCartaAMano(c);
                }
            }
        }
    }

    // Robar una carta del mazo: si hay cartas, copia a 'out' y retorna true
    bool robarCarta(Carta& out) {
        if (deckPos >= mazoSize) return false;
        out = mazo[deckPos++];
        return true;
    }

    // Ejecuta el juego con una estrategia simple:
    // Cada turno, el jugador roba (si hay mazo), luego juega la carta de índice 0 (si tiene cartas).
    // Esta es una estrategia automática simple para mostrar flujo. Puedes cambiarla por entrada humana.
    void ejecutarJuego() {
        int turno = 0;
        // Jugar hasta que mazo vacío y todos los jugadores sin cartas
        while (true) {
            bool alguienConCartas = false;
            for (int p = 0; p < numJugadores; ++p) {
                // Turno del jugador p
                std::cout << "--------------------------\n";
                std::cout << jugadores[p].getNombre() << " turno.\n";

                // Robar si queda en el mazo
                Carta robada;
                if (robarCarta(robada)) {
                    std::cout << "  Robó: " << robada.toString() << "\n";
                    jugadores[p].addCartaAMano(robada);
                } else {
                    std::cout << "  Mazo vacío.\n";
                }

                jugadores[p].mostrarMano();

                // Estrategia simple: jugar la carta con mayor valor de la mano
                if (jugadores[p].tieneCartasEnMano()) {
                    int mejorIdx = 0;
                    int mejorVal = jugadores[p].mano[0].valor;
                    for (int i = 1; i < jugadores[p].manoSize; ++i) {
                        if (jugadores[p].mano[i].valor > mejorVal) {
                            mejorVal = jugadores[p].mano[i].valor;
                            mejorIdx = i;
                        }
                    }
                    Carta aJugar = jugadores[p].mano[mejorIdx];
                    jugadores[p].jugarCarta(mejorIdx);
                    std::cout << "  Jugó: " << aJugar.toString() << " -> a su pila " << COLOR_NAMES[aJugar.color] << "\n";
                } else {
                    std::cout << "  No tiene cartas para jugar.\n";
                }

                jugadores[p].mostrarPilas();
                if (jugadores[p].tieneCartasEnMano()) alguienConCartas = true;
            }

            // condición de parada: mazo vacío y nadie tiene cartas
            if (deckPos >= mazoSize && !alguienConCartas) break;

            ++turno;
            if (turno > 10000) { // salvaguarda contra loops infinitos
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

    // Muestra estado del mazo (cuántas cartas quedan)
    void mostrarEstadoMazo() const {
        std::cout << "Cartas en mazo restantes: " << (mazoSize - deckPos) << " / " << mazoSize << "\n";
    }
};

// -------------------------
// Función main
// -------------------------
int main() {
    std::cout << "=== Colorette (versión didáctica con memoria dinámica) ===\n";

    int numPlayers;
    std::cout << "Número de jugadores (2..6): ";
    std::cin >> numPlayers;
    if (numPlayers < 2) numPlayers = 2;
    if (numPlayers > 6) numPlayers = 6;

    // Parámetros del mazo: por color 1..maxValue (ej. 1..9)
    int maxValuePerColor = 9;

    Juego juego;
    juego.inicializarMazo(maxValuePerColor);
    juego.barajar();

    // Crear jugadores con capacidad inicial de mano (ej. 5)
    juego.crearJugadores(numPlayers, 5);

    // Repartir 5 cartas iniciales cada jugador
    juego.repartirInicial(5);

    juego.mostrarEstadoMazo();
    juego.ejecutarJuego();

    std::cout << "Fin del juego. Gracias por jugar.\n";
    return 0;
}