#include "Jugador.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <fstream>

Jugador::Jugador() : nombre("Jugador"), mano(nullptr), manoSize(0), manoCap(0), pilesSum(nullptr) {}

Jugador::Jugador(const std::string& n, int initialHandCap) : nombre(n) {
    manoCap = initialHandCap;
    manoSize = 0;
    mano = new Carta[manoCap];
    pilesSum = new int[MAX_COLORS];
    for (int i = 0; i < MAX_COLORS; ++i) pilesSum[i] = 0;
}

Jugador::~Jugador() {
    if (mano) delete[] mano;
    if (pilesSum) delete[] pilesSum;
}

std::string Jugador::getNombre() const { 
    return nombre; 
}

void Jugador::addCartaAMano(const Carta& c) {
    if (manoSize >= manoCap) {
        int nuevaCap = std::max(1, manoCap * 2);
        Carta* nuevo = new Carta[nuevaCap];
        for (int i = 0; i < manoSize; ++i) nuevo[i] = mano[i];
        delete[] mano;
        mano = nuevo;
        manoCap = nuevaCap;
    }
    mano[manoSize++] = c;
}

bool Jugador::jugarCarta(int idx) {
    if (idx < 0 || idx >= manoSize) return false;
    Carta c = mano[idx];
    pilesSum[c.color] += c.valor;

    for (int i = idx; i + 1 < manoSize; ++i) mano[i] = mano[i + 1];
    --manoSize;
    return true;
}

void Jugador::mostrarMano() const {
    std::cout << nombre << " mano (" << manoSize << "): ";
    for (int i = 0; i < manoSize; ++i) {
        std::cout << "[" << i << ":" << mano[i].toString() << "] ";
    }
    std::cout << "\n";
}

void Jugador::mostrarPilas() const {
    std::cout << nombre << " pilas:\n";
    for (int c = 0; c < MAX_COLORS; ++c) {
        std::cout << "  " << std::setw(9) << COLOR_NAMES[c] << " -> suma: " << pilesSum[c] << "\n";
    }
}

int Jugador::calcularPuntaje() const {
    int s = 0;
    for (int c = 0; c < MAX_COLORS; ++c) s += pilesSum[c];
    return s;
}

bool Jugador::tieneCartasEnMano() const { 
    return manoSize > 0; 
}

void Jugador::serialize(std::ofstream& out) const {
    out << nombre << "\n";
    out << manoSize << "\n";
    for (int i = 0; i < manoSize; ++i) {
        out << mano[i].color << " " << mano[i].valor << "\n";
    }
    for (int c = 0; c < MAX_COLORS; ++c) {
        out << pilesSum[c] << " ";
    }
    out << "\n";
}

void Jugador::deserialize(std::ifstream& in, int initialHandCap) {
    std::getline(in, nombre);
    
    pilesSum = new int[MAX_COLORS];
    
    in >> manoSize;
    in.ignore();
    
    manoCap = std::max(initialHandCap, manoSize);
    mano = new Carta[manoCap];
    
    for (int i = 0; i < manoSize; ++i) {
        int color, valor;
        in >> color >> valor;
        mano[i] = Carta(color, valor);
    }
    in.ignore();
    
    for (int c = 0; c < MAX_COLORS; ++c) {
        in >> pilesSum[c];
    }
    in.ignore();
}
