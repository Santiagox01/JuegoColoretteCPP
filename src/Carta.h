#ifndef CARTA_H
#define CARTA_H

#include <string>

constexpr int MAX_COLORS = 7;

extern const std::string COLOR_NAMES[MAX_COLORS];

class Carta {
public:
    int color;
    int valor;

    Carta();
    Carta(int c, int v);
    std::string toString() const;
};

#endif
