#include "Carta.h"

const std::string COLOR_NAMES[MAX_COLORS] = {
    "Rojo", "Azul", "Verde", "Amarillo", "Morado", "Naranja", "Rosado"
};

Carta::Carta() : color(0), valor(0), esEspecial(false), tipoEspecial(0) {}

Carta::Carta(int c, int v) : color(c), valor(v), esEspecial(false), tipoEspecial(0) {}

Carta Carta::crearUltimaRonda() {
    Carta c;
    c.esEspecial = true;
    c.tipoEspecial = CARTA_ESPECIAL_ULTIMA_RONDA;
    c.color = -1;
    c.valor = 0;
    return c;
}

Carta Carta::crearPlus2() {
    Carta c;
    c.esEspecial = true;
    c.tipoEspecial = CARTA_ESPECIAL_PLUS2;
    c.color = -1;
    c.valor = 2;
    return c;
}

std::string Carta::toString() const {
    if (esEspecial) {
        if (tipoEspecial == CARTA_ESPECIAL_ULTIMA_RONDA) {
            return "**ÚLTIMA RONDA**";
        } else if (tipoEspecial == CARTA_ESPECIAL_PLUS2) {
            return "+2 PUNTOS";
        }
    }
    return COLOR_NAMES[color] + " " + std::to_string(valor);
}

bool Carta::esUltimaRonda() const {
    return esEspecial && tipoEspecial == CARTA_ESPECIAL_ULTIMA_RONDA;
}

bool Carta::esPlus2() const {
    return esEspecial && tipoEspecial == CARTA_ESPECIAL_PLUS2;
}
