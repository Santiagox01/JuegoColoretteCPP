#include "SaveManager.h"
#include <filesystem>
#include <ctime>
#include <sstream>
#include <iomanip>

namespace fs = std::filesystem;

std::vector<std::string> SaveManager::listarPartidas() {
    std::vector<std::string> partidas;
    
    if (!fs::exists("saves")) {
        fs::create_directory("saves");
        return partidas;
    }
    
    for (const auto& entry : fs::directory_iterator("saves")) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            partidas.push_back(entry.path().stem().string());
        }
    }
    
    return partidas;
}

bool SaveManager::eliminarPartida(const std::string& nombre) {
    std::string filepath = "saves/" + nombre + ".txt";
    if (fs::exists(filepath)) {
        return fs::remove(filepath);
    }
    return false;
}

bool SaveManager::existePartida(const std::string& nombre) {
    std::string filepath = "saves/" + nombre + ".txt";
    return fs::exists(filepath);
}

std::string SaveManager::generarNombreConFecha() {
    std::time_t now = std::time(nullptr);
    std::tm* ltm = std::localtime(&now);
    
    std::ostringstream oss;
    oss << "partida_" 
        << std::setfill('0') << std::setw(4) << (1900 + ltm->tm_year)
        << std::setw(2) << (1 + ltm->tm_mon)
        << std::setw(2) << ltm->tm_mday << "_"
        << std::setw(2) << ltm->tm_hour
        << std::setw(2) << ltm->tm_min
        << std::setw(2) << ltm->tm_sec;
    
    return oss.str();
}
