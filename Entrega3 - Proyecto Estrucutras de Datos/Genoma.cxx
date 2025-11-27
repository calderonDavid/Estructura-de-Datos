#include "Genoma.h"
#include "winRAR.h"
#include "ArbolHoffman.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <queue>
#include <cmath>
#include <limits>
#include <algorithm>
#include <iomanip>


// Estructura auxiliar para Dijkstra
struct Estado {
    int x, y;
    float costo;
    
    bool operator>(const Estado& other) const {
        return costo > other.costo;
    }
};
// Constructor
Genoma::Genoma(std::list<Secuencia> conjunto, int cantSecuencias) 
    : conjunto(conjunto), cantSecuencias(cantSecuencias) {}

// Operaciones
const std::list<Secuencia>& Genoma::getConjunto() const {
    return conjunto;
}
//getters y setters..
void Genoma::setConjunto(const std::list<Secuencia>& nuevoConjunto) {
    conjunto = nuevoConjunto;
}

int Genoma::getCantSecuencias() const {
    return cantSecuencias;
}

void Genoma::setCantSecuencias(int cantidad) {
    cantSecuencias = cantidad;
}
//Listar las secuencias que tiene el genoma
void Genoma::ListarSecuencias() {
	//evalua si hay secuencias cargadas 
    if (conjunto.empty()) {
        std::cout << "No hay secuencias cargadas en memoria.\n";
    } else {
        std::cout << "Hay " << conjunto.size() << " secuencias cargadas en memoria:\n";
        //iterador para ir por cada secuencia del genoma
        std::list<Secuencia>::iterator it;
        for (it = conjunto.begin(); it != conjunto.end(); it++) {
        //dependiendo si esta completo o no, se imprime.. (nombre, tamaño)
            if (it->EsCompleta()) {
                std::cout << "Secuencia " << it->Obtenernombre()
                          << " contiene " << it->lineas.size() << " bases.\n";
            } else {
                std::cout << "Secuencia " << it->Obtenernombre()
                          << " contiene al menos " << it->lineas.size()<< " bases.\n";
            }
        }
    }
}
//Metodo para usar histograma, recibimos la secuencia 
void Genoma::ObtenerSecuencia(const std::string& nombre) {
    Histograma h;
    //iterador para ir por secuencias del genoma
    std::list<Secuencia>::iterator it;
    for (it = conjunto.begin(); it != conjunto.end(); it++) {
        if (it->Obtenernombre() == nombre) {
        //buscamos que la secuencia exista en genoma
            h.ordenSegunTabla(*it);
        }
    }
}
//Metodo para contar la cantidad de veces que sale una sub secuencia en las secuencias
void Genoma::ExisteSubsecuencia(std::string &sub) {
    if (conjunto.empty()) {
        std::cout << "No hay secuencias cargadas en memoria.\n";
        return;
    }

    int contador = 0;
    bool existe = false;
    int anchoaux= conjunto.begin()->anchoLinea;
    std::list<Secuencia>:: iterator it;
    for ( it = conjunto.begin(); it != conjunto.end(); it++) {
    // se pasa de char a string para manejarlo mas facil..
        std::string secuencia_string(it->lineas.begin(), it->lineas.end());
        int contInt = 0 ;
        for (int i = 0; i + sub.size() <= secuencia_string.size(); i++) {
        	//se recorre de tal forma que toma una posicion evalua, y sigue a la siguiente posicion...
        	//se evalua si apartir de esa posicion existe un substring que cumpla el mismo tamaño y sea igual..

            contInt++;
            if (secuencia_string.substr(i, sub.size()) == sub && contInt!= anchoaux) {
                contador++;
                existe = true;
            }
            if (contInt == anchoaux) {
                contInt= 0;
            }
        }
    }
    // se imprime con base en si se encontro o no la sub secuencia
    if (existe) {
        std::cout << "La subsecuencia dada se repite " << contador 
                  << " veces dentro de las secuencias cargadas.\n";
    } else {
        std::cout << "La subsecuencia dada no existe dentro de las secuencias cargadas.\n";
    }
}
//metodo para enmascarar una subsecuencia dada Funciona de forma general igual a "ExisteSubsecuencia"
void Genoma::EnmascararSubsecuencia(std::string &sub) {
    if (conjunto.empty()) {
        std::cout << "No hay secuencias cargadas en memoria.\n";
        return;
    }

    bool encontrada = false;
	std::list<Secuencia>:: iterator it;
    for (it = conjunto.begin(); it != conjunto.end(); it++) {
    	// se pasa de char a string para manejarlo mas facil..
        std::string secuencia_string(it->lineas.begin(), it->lineas.end());
	
	//se recorre de tal forma que toma una posicion evalua, y sigue a la siguiente posicion...
        //se evalua si apartir de esa posicion existe un substring que cumpla el mismo tamaño y sea igual..
        for (int i = 0; i + sub.size() <= secuencia_string.size(); i++) {
            if (secuencia_string.substr(i, sub.size()) == sub) {
                encontrada = true;
                //desde la posicion que encontro la subsecuencua, pasar las bases a "x"
                for (int j = 0; j < sub.size(); j++) {
                    secuencia_string[i + j] = 'X';
                }
            }
        }
	//Codigo generado por IA
        // actualizar lineas en la secuencia
        //se limpia la secuencia anterior (sin enmascarar)
        it->lineas.clear();
        //se inserta la string que se divide en char.. con las sub secuencias ya enmascaradas
        it->lineas.insert(it->lineas.begin(), secuencia_string.begin(), secuencia_string.end());
    }
    //se imprime con base en si se pudo enmascarar o no 
    if (encontrada) {
        std::cout << "La subsecuencia se ha enmascarado correctamente.\n";
    } else {
        std::cout << "La subsecuencia dada no existe dentro de las secuencias cargadas.\n";
    }
}

void Genoma::codificar(std::string& salidaArchivo){
    if (conjunto.empty()) {
        std::cout << "No hay secuencias cargadas en memoria." << std::endl;
        return;
    }

    std::map<char, int> frecuencia;
    for (Secuencia c : conjunto) {
        for (char base : c.lineas) {
            frecuencia[base]++;
        }
    }
    ArbolHoffman arbol;
    arbol.construirHojas(frecuencia);
    
    std::map<char, std::string> ASCIIV2;
    arbol.codigosRegistrados(ASCIIV2);
    
    std::ofstream archivoSalida(salidaArchivo + ".fabin", std::ios::binary);
    if (!archivoSalida.is_open()) {
        std::cerr << "No se pueden guardar las secuencias cargadas en " << salidaArchivo << std::endl;
        return;
    }
    
    // Escribir cabecera (con tipos simplificados)
    short n_lineas = frecuencia.size();
    archivoSalida.write(reinterpret_cast<char*>(&n_lineas), sizeof(short));
    for (auto par : frecuencia) {
        archivoSalida.write(&par.first, sizeof(char));
        archivoSalida.write(reinterpret_cast<char*>(&par.second), sizeof(int));
    }
    
    int n_secuencias = conjunto.size();
    archivoSalida.write(reinterpret_cast<char*>(&n_secuencias), sizeof(int));

    winRAR codificador;
    for (Secuencia c : conjunto) {
        short longNombre = c.Nombresecuencia.length();
        archivoSalida.write(reinterpret_cast<char*>(&longNombre), sizeof(short));
        archivoSalida.write(c.Nombresecuencia.c_str(), longNombre);
        
        int long_linea = c.lineas.size();
        archivoSalida.write(reinterpret_cast<char*>(&long_linea), sizeof(int));
        
        short ancho = c.anchoLinea;
        archivoSalida.write(reinterpret_cast<char*>(&ancho), sizeof(short));
        
        std::string bases(c.lineas.begin(), c.lineas.end());
        std::vector<unsigned char> v_datos = codificador.codifi(bases, ASCIIV2);
        
        int tamano_codificado = v_datos.size();
        archivoSalida.write(reinterpret_cast<char*>(&tamano_codificado), sizeof(int));
        archivoSalida.write(reinterpret_cast<char*>(v_datos.data()), v_datos.size());
    }
    
    archivoSalida.close();
    std::cout << "Secuencias codificadas y almacenadas en " << salidaArchivo << std::endl;
}


void Genoma::decodificar(const std::string& nombreArchivo) {

    std::ifstream archivoEntrada(nombreArchivo+".fabin", std::ios::binary);
    if (!archivoEntrada.is_open()) {
        std::cerr << "No se pueden cargar las secuencias desde " << nombreArchivo << std::endl;
        return;
    }
    
    conjunto.clear();
    
    std::map<char, int> frecuencia;
    short n_lineas;
    archivoEntrada.read(reinterpret_cast<char*>(&n_lineas), sizeof(short));
    
    for (int i = 0; i < n_lineas; ++i) {
        char base;
        int freq;
        archivoEntrada.read(&base, sizeof(char));
        archivoEntrada.read(reinterpret_cast<char*>(&freq), sizeof(int));
        frecuencia[base] = freq;
    }
    
    ArbolHoffman arbol;
    arbol.construirHojas(frecuencia);
    if (arbol.raiz == nullptr) {
        std::cerr << "Error: árbol de Hoffman vacío tras construir hojas.\n";
        // no return for now, pero cuidado si decodifi espera arbol no nulo
    }

    int n_conjunto;
    archivoEntrada.read(reinterpret_cast<char*>(&n_conjunto), sizeof(int));
    
    winRAR decodificador;
    
    for (int i = 0; i < n_conjunto; ++i) {
        short nombre_len;
        archivoEntrada.read(reinterpret_cast<char*>(&nombre_len), sizeof(short));
        
        std::string nombre_secuencia(nombre_len, '\0');
        archivoEntrada.read(&nombre_secuencia[0], nombre_len);

        int secuencia_len_original;
        archivoEntrada.read(reinterpret_cast<char*>(&secuencia_len_original), sizeof(int));
        
        short ancho;
        archivoEntrada.read(reinterpret_cast<char*>(&ancho), sizeof(short));

        int tamano_codificado;
        archivoEntrada.read(reinterpret_cast<char*>(&tamano_codificado), sizeof(int));

        std::vector<unsigned char> v_datos(tamano_codificado);
        archivoEntrada.read(reinterpret_cast<char*>(v_datos.data()), tamano_codificado);
        
        std::string conjuntoDecodificado = decodificador.decodifi(v_datos, secuencia_len_original, arbol.raiz);
        
        std::list<char> lineas(conjuntoDecodificado.begin(), conjuntoDecodificado.end());
        
        bool completa = true;
        for(char base : lineas) { if (base == '-') { completa = false; break; } }
        
        Secuencia s(nombre_secuencia, lineas, ancho, completa);
        conjunto.push_back(s);
    }
    archivoEntrada.close();
    std::cout << "Secuencias decodificadas desde " << nombreArchivo << ".fabin y cargadas en memoria." << std::endl;
}
float calcularPeso(char a, char b) {
    return 1.0f / (1.0f + std::abs((int)a - (int)b));
}

void Genoma::ruta_mas_corta(std::string descripcion_secuencia, int i_orig, int j_orig, int i_dest, int j_dest) {
    // 1. Buscar la secuencia
    auto it = conjunto.begin();
    bool encontrada = false;
    while (it != conjunto.end()) {
        if (it->Obtenernombre() == descripcion_secuencia) {
            encontrada = true;
            break;
        }
        it++;
    }

    if (!encontrada) {
        std::cout << "La secuencia " << descripcion_secuencia << " no existe.\n";
        return;
    }

    // 2. Preparar datos para el grafo
    // Convertimos la lista a vector para acceso rápido O(1)
    std::vector<char> bases(it->lineas.begin(), it->lineas.end());
    int ancho = it->anchoLinea;
    int totalBases = bases.size();
    
    // Calculamos altura (filas)
    int alto = (totalBases + ancho - 1) / ancho; // División techo

    // 3. Validar coordenadas
    if (i_orig < 0 || i_orig >= alto || j_orig < 0 || j_orig >= ancho || (i_orig * ancho + j_orig) >= totalBases) {
        std::cout << "La base en la posición [" << i_orig << "," << j_orig << "] no existe.\n";
        return;
    }
    if (i_dest < 0 || i_dest >= alto || j_dest < 0 || j_dest >= ancho || (i_dest * ancho + j_dest) >= totalBases) {
        std::cout << "La base en la posición [" << i_dest << "," << j_dest << "] no existe.\n";
        return;
    }

    // 4. Dijkstra
    // Matriz de distancias inicializada en infinito
    std::vector<std::vector<float>> dist(alto, std::vector<float>(ancho, std::numeric_limits<float>::infinity()));
    // Matriz de padres para reconstruir la ruta: guarda coordenadas {i, j} previas
    std::vector<std::vector<std::pair<int, int>>> parent(alto, std::vector<std::pair<int, int>>(ancho, {-1, -1}));
    
    std::priority_queue<Estado, std::vector<Estado>, std::greater<Estado>> pq;

    dist[i_orig][j_orig] = 0;
    pq.push({i_orig, j_orig, 0});

    // Movimientos posibles: Arriba, Abajo, Izquierda, Derecha
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};

    while (!pq.empty()) {
        Estado actual = pq.top();
        pq.pop();

        int ux = actual.x;
        int uy = actual.y;

        // Si llegamos al destino, podemos detenernos (opcional, pero eficiente)
        if (ux == i_dest && uy == j_dest) break;

        if (actual.costo > dist[ux][uy]) continue;

        char baseActual = bases[ux * ancho + uy];

        // Revisar vecinos
        for (int k = 0; k < 4; k++) {
            int vx = ux + dx[k];
            int vy = uy + dy[k];

            // Validar límites y que la posición exista en el vector lineal
            if (vx >= 0 && vx < alto && vy >= 0 && vy < ancho) {
                int indiceVecino = vx * ancho + vy;
                if (indiceVecino < totalBases) {
                    char baseVecina = bases[indiceVecino];
                    float peso = calcularPeso(baseActual, baseVecina);
                    
                    if (dist[ux][uy] + peso < dist[vx][vy]) {
                        dist[vx][vy] = dist[ux][uy] + peso;
                        parent[vx][vy] = {ux, uy};
                        pq.push({vx, vy, dist[vx][vy]});
                    }
                }
            }
        }
    }

    // 5. Resultados
    if (dist[i_dest][j_dest] == std::numeric_limits<float>::infinity()) {
        std::cout << "No existe ruta entre [" << i_orig << "," << j_orig << "] y [" << i_dest << "," << j_dest << "].\n";
    } else {
        std::cout << "Para la secuencia " << descripcion_secuencia << ", la ruta más corta entre la base "
                  << bases[i_orig * ancho + j_orig] << " en [" << i_orig << "," << j_orig << "] y la base "
                  << bases[i_dest * ancho + j_dest] << " en [" << i_dest << "," << j_dest << "] es:\n";
        
        // Reconstruir camino
        std::vector<char> camino;
        int currX = i_dest;
        int currY = j_dest;
        
        while (currX != -1 && currY != -1) {
            camino.push_back(bases[currX * ancho + currY]);
            std::pair<int, int> p = parent[currX][currY];
            // Verificar si llegamos al inicio
            if (currX == i_orig && currY == j_orig) break;
            currX = p.first;
            currY = p.second;
        }
        std::reverse(camino.begin(), camino.end());

        // Imprimir camino
        for (size_t k = 0; k < camino.size(); k++) {
            std::cout << camino[k];
            if (k < camino.size() - 1) std::cout << " -> ";
        }
        std::cout << "\nEl costo total de la ruta es: " << dist[i_dest][j_dest] << "\n";
    }
}

void Genoma::base_remota(std::string descripcion_secuencia, int i_orig, int j_orig) {
    // 1. Buscar la secuencia
    auto it = conjunto.begin();
    bool encontrada = false;
    while (it != conjunto.end()) {
        if (it->Obtenernombre() == descripcion_secuencia) {
            encontrada = true;
            break;
        }
        it++;
    }

    if (!encontrada) {
        std::cout << "La secuencia " << descripcion_secuencia << " no existe.\n";
        return;
    }

    // 2. Preparar datos
    std::vector<char> bases(it->lineas.begin(), it->lineas.end());
    int ancho = it->anchoLinea;
    int totalBases = bases.size();
    int alto = (totalBases + ancho - 1) / ancho;

    // 3. Validar origen
    if (i_orig < 0 || i_orig >= alto || j_orig < 0 || j_orig >= ancho || (i_orig * ancho + j_orig) >= totalBases) {
        std::cout << "La base en la posición [" << i_orig << "," << j_orig << "] no existe.\n";
        return;
    }

    char baseOrigen = bases[i_orig * ancho + j_orig];

    // 4. Dijkstra completo (hacia todos los nodos)
    std::vector<std::vector<float>> dist(alto, std::vector<float>(ancho, std::numeric_limits<float>::infinity()));
    std::vector<std::vector<std::pair<int, int>>> parent(alto, std::vector<std::pair<int, int>>(ancho, {-1, -1}));
    std::priority_queue<Estado, std::vector<Estado>, std::greater<Estado>> pq;

    dist[i_orig][j_orig] = 0;
    pq.push({i_orig, j_orig, 0});

    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};

    while (!pq.empty()) {
        Estado actual = pq.top();
        pq.pop();

        if (actual.costo > dist[actual.x][actual.y]) continue;

        char baseActual = bases[actual.x * ancho + actual.y];

        for (int k = 0; k < 4; k++) {
            int vx = actual.x + dx[k];
            int vy = actual.y + dy[k];

            if (vx >= 0 && vx < alto && vy >= 0 && vy < ancho) {
                int idxVecino = vx * ancho + vy;
                if (idxVecino < totalBases) {
                    char baseVecina = bases[idxVecino];
                    float peso = calcularPeso(baseActual, baseVecina);
                    
                    if (dist[actual.x][actual.y] + peso < dist[vx][vy]) {
                        dist[vx][vy] = dist[actual.x][actual.y] + peso;
                        parent[vx][vy] = {actual.x, actual.y};
                        pq.push({vx, vy, dist[vx][vy]});
                    }
                }
            }
        }
    }

    // 5. Buscar la base remota (Misma base, mayor distancia)
    float maxDist = -1.0f;
    int maxI = -1, maxJ = -1;

    for (int i = 0; i < alto; i++) {
        for (int j = 0; j < ancho; j++) {
            int idx = i * ancho + j;
            if (idx < totalBases) {
                // Debe ser la misma letra Y la distancia no debe ser infinito (alcanzable) Y mayor a la actual
                if (bases[idx] == baseOrigen && dist[i][j] != std::numeric_limits<float>::infinity()) {
                    if (dist[i][j] > maxDist) {
                        maxDist = dist[i][j];
                        maxI = i;
                        maxJ = j;
                    }
                }
            }
        }
    }

    // 6. Resultados
    if (maxI == -1) {
        // Esto solo pasa si no hay otras bases iguales o no son alcanzables
        std::cout << "No se encontró ninguna otra base '" << baseOrigen << "' alcanzable.\n";
    } else {
        std::cout << "Para la secuencia " << descripcion_secuencia << ", la base remota está ubicada en ["
                  << maxI << "," << maxJ << "].\n";
        std::cout << "La ruta entre la base en [" << i_orig << "," << j_orig << "] y la base remota en ["
                  << maxI << "," << maxJ << "] es:\n";

        // Reconstruir camino
        std::vector<char> camino;
        int currX = maxI;
        int currY = maxJ;
        
        while (currX != -1 && currY != -1) {
            camino.push_back(bases[currX * ancho + currY]);
            std::pair<int, int> p = parent[currX][currY];
            if (currX == i_orig && currY == j_orig) break;
            currX = p.first;
            currY = p.second;
        }
        std::reverse(camino.begin(), camino.end());

        for (size_t k = 0; k < camino.size(); k++) {
            std::cout << camino[k];
            if (k < camino.size() - 1) std::cout << " -> ";
        }
        std::cout << "\nEl costo total de la ruta es: " << maxDist << "\n";
    }
}
