#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include <string>
#include <vector>

class SaveManager {
public:
    static std::vector<std::string> listarPartidas();
    static bool eliminarPartida(const std::string& nombre);
    static bool existePartida(const std::string& nombre);
    static std::string generarNombreConFecha();
};

#endif