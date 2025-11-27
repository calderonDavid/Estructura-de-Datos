#ifndef SECUENCIA_H
#define SECUENCIA_H

#include <string>
#include <list>
#include <vector> // Agregado para el soporte de matrices

class Secuencia {
public:
    //Datos minimos
    std::string Nombresecuencia;
    std::list<char> lineas;
    int anchoLinea;
    bool completo;

    // Constructor
    Secuencia(std::string nombre, std::list<char> linea, int ancho, bool complet);

    // Operaciones
    std::string Obtenernombre() const;
    int ObtenerLongitud();
    bool EsCompleta() const;
    int ContarBase(char b);

    // NUEVO METODO: Necesario para Componente 3 (Grafos)
    // Convierte la lista lineal en una matriz 2D [fila][columna]
    std::vector<std::vector<char>> obtenerMatriz() const;
 
};

#endif
