#ifndef ARCHIVO_H
#define ARCHIVO_H

#include <string>
#include <list>
#include "Secuencia.h"
#include "Genoma.h"

class Archivo {
public:
    std::string ruta;

    // Constructor
    Archivo();

    // Operaciones
    Genoma leer(const std::string &ruta);
    void guardar(const Genoma& genoma, std::string nombreArchivoGuardado);
};

#endif
