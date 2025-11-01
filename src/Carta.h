#ifndef CARTA_H
#define CARTA_H

#include <string>

constexpr int MAX_COLORS = 7;
constexpr int CARTA_ESPECIAL_ULTIMA_RONDA = -1;
constexpr int CARTA_ESPECIAL_PLUS2 = -2;

extern const std::string COLOR_NAMES[MAX_COLORS];

class Carta {
public:
    int color;
    int valor;
    bool esEspecial;
    int tipoEspecial;

    Carta();
    Carta(int c, int v);
    static Carta crearUltimaRonda();
    static Carta crearPlus2();
    std::string toString() const;
    bool esUltimaRonda() const;
    bool esPlus2() const;
};

#endif