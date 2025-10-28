#include "Carta.h"

const std::string COLOR_NAMES[MAX_COLORS] = {
    "Rojo", "Azul", "Verde", "Amarillo", "Morado", "Naranja", "Rosado"
};

Carta::Carta() : color(0), valor(0) {}

Carta::Carta(int c, int v) : color(c), valor(v) {}

std::string Carta::toString() const {
    return COLOR_NAMES[color] + " " + std::to_string(valor);
}
