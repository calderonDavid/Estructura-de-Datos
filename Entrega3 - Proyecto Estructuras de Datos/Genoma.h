#ifndef GENOMA_H
#define GENOMA_H

#include <string>
#include <list>
#include <vector>
#include "Secuencia.h"
#include "Histograma.h"

class Genoma {
public:
//datos minimos 
    std::list<Secuencia> conjunto;
    int cantSecuencias;

    // Constructor
    Genoma(std::list<Secuencia> conjunto = {}, int cantSecuencias = 0);

    // Operaciones
    const std::list<Secuencia>& getConjunto() const;
    void setConjunto(const std::list<Secuencia>& nuevoConjunto);
    int getCantSecuencias() const;
    void setCantSecuencias(int cantidad);
    void ListarSecuencias();

    void ObtenerSecuencia(const std::string& nombre);
    void ExisteSubsecuencia(std::string &sub);
    void EnmascararSubsecuencia(std::string &sub);
    void codificar(std::string& salidaArchivo);
    void decodificar(const std::string& nombreArchivo);
    void ruta_mas_corta(std::string nombreSec, int i, int j, int x, int y);
    void base_remota(std::string nombreSec, int i, int j);
};

#endif
