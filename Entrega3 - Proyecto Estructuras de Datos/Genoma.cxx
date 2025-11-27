#include "Genoma.h"
#include "winRAR.h"
#include "ArbolHoffman.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <queue>     // Requerido para Dijkstra
#include <cmath>     // Requerido para abs()
#include <limits>    // Requerido para infinito
#include <algorithm> // Requerido para reverse

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
            return; // Agregado return para eficiencia
        }
    }
    // Si llega aqui es que no encontro nada
    std::cout << "Secuencia inválida.\n";
}
//Metodo para contar la cantidad de veces que sale una sub secuencia en las secuencias
void Genoma::ExisteSubsecuencia(std::string &sub) {
    if (conjunto.empty()) {
        std::cout << "No hay secuencias cargadas en memoria.\n";
        return;
    }

    int contador = 0;
    bool existe = false;
    // Correccion de seguridad por si conjunto esta vacio (aunque el if de arriba lo cubre)
    int anchoaux = 0; 
    if(!conjunto.empty()) anchoaux = conjunto.begin()->anchoLinea;

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
    // Optimizacion: referencia constante
    for (const Secuencia& c : conjunto) {
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
        char k = par.first;
        int v = par.second;
        archivoSalida.write(&k, sizeof(char));
        archivoSalida.write(reinterpret_cast<char*>(&v), sizeof(int));
    }
    
    int n_secuencias = conjunto.size();
    archivoSalida.write(reinterpret_cast<char*>(&n_secuencias), sizeof(int));

    winRAR codificador;
    // Optimizacion: iterar sin copia masiva si es posible, pero mantenemos tu logica
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
        
        // Correccion potencial: Si arbol.raiz es null, esto falla.
        // Asumimos que construirHojas funciono bien.
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

// =========================================================
//         COMPONENTE 3: GRAFOS Y DIJKSTRA
// =========================================================

// Alias para evitar structs externas: Pair<Costo, Pair<Fila, Columna>>
using NodoDijkstra = std::pair<double, std::pair<int, int>>;

void Genoma::ruta_mas_corta(std::string descripcion_secuencia, int i, int j, int x, int y) {
    // 1. Buscar la secuencia
    Secuencia* secEncontrada = nullptr;
    // Iteramos por referencia para no copiar
    for (auto& s : conjunto) {
        if (s.Obtenernombre() == descripcion_secuencia) {
            secEncontrada = &s;
            break;
        }
    }

    if (secEncontrada == nullptr) {
        std::cout << "La secuencia " << descripcion_secuencia << " no existe.\n";
        return;
    }

    // 2. Obtener matriz (Grafo implicito)
    std::vector<std::vector<char>> matriz = secEncontrada->obtenerMatriz();
    int filas = matriz.size();
    
    // Validaciones de limites
    if (i < 0 || i >= filas || j < 0 || j >= matriz[i].size()) {
        std::cout << "La base en la posición [" << i << "," << j << "] no existe.\n";
        return;
    }
    if (x < 0 || x >= filas || y < 0 || y >= matriz[x].size()) {
        std::cout << "La base en la posición [" << x << "," << y << "] no existe.\n";
        return;
    }

    // 3. Inicializar Dijkstra
    // Usamos vector de vectores para distancias y padres
    std::vector<std::vector<double>> dist(filas);
    std::vector<std::vector<std::pair<int, int>>> padre(filas);

    // Ajustar tamaños de columnas (pueden ser irregulares)
    for(int k=0; k<filas; k++){
        dist[k].resize(matriz[k].size(), std::numeric_limits<double>::infinity());
        padre[k].resize(matriz[k].size(), {-1, -1});
    }

    // Min-Heap (menor costo arriba)
    std::priority_queue<NodoDijkstra, std::vector<NodoDijkstra>, std::greater<NodoDijkstra>> pq;

    dist[i][j] = 0;
    pq.push({0.0, {i, j}});

    // Vectores de movimiento: Arriba, Abajo, Izq, Der
    int dFila[] = {-1, 1, 0, 0};
    int dCol[] = {0, 0, -1, 1};

    while (!pq.empty()) {
        double costoActual = pq.top().first;
        int f = pq.top().second.first;
        int c = pq.top().second.second;
        pq.pop();

        if (costoActual > dist[f][c]) continue;
        if (f == x && c == y) break; // Encontramos destino

        // Explorar 4 vecinos
        for (int k = 0; k < 4; k++) {
            int nf = f + dFila[k];
            int nc = c + dCol[k];

            // Validar existencia del vecino en la matriz
            if (nf >= 0 && nf < filas && nc >= 0 && nc < matriz[nf].size()) {
                // Formula del enunciado: 1 / (1 + |ASCII_1 - ASCII_2|)
                double peso = 1.0 / (1.0 + std::abs((int)matriz[f][c] - (int)matriz[nf][nc]));
                
                if (dist[f][c] + peso < dist[nf][nc]) {
                    dist[nf][nc] = dist[f][c] + peso;
                    padre[nf][nc] = {f, c};
                    pq.push({dist[nf][nc], {nf, nc}});
                }
            }
        }
    }

    // 4. Resultados
    if (dist[x][y] == std::numeric_limits<double>::infinity()) {
        std::cout << "No existe una ruta entre [" << i << "," << j << "] y [" << x << "," << y << "].\n";
    } else {
        std::cout << "Para la secuencia " << descripcion_secuencia << ", la ruta más corta entre la base en [" 
                << i << "," << j << "] y la base en [" << x << "," << y << "] es:\n";
        
        // Reconstruccion (Backtracking)
        std::vector<char> ruta;
        std::pair<int, int> actual = {x, y};
        while (actual.first != -1) {
            ruta.push_back(matriz[actual.first][actual.second]);
            actual = padre[actual.first][actual.second];
        }
        std::reverse(ruta.begin(), ruta.end()); // Invertir para tener Inicio -> Fin

        for (size_t k = 0; k < ruta.size(); k++) {
            std::cout << ruta[k];
            if (k < ruta.size() - 1) std::cout << ", ";
        }
        std::cout << "\nEl costo total de la ruta es: " << dist[x][y] << "\n";
    }
}

void Genoma::base_remota(std::string descripcion_secuencia, int i, int j) {
    Secuencia* secEncontrada = nullptr;
    for (auto& s : conjunto) {
        if (s.Obtenernombre() == descripcion_secuencia) {
            secEncontrada = &s;
            break;
        }
    }

    if (secEncontrada == nullptr) {
        std::cout << "La secuencia " << descripcion_secuencia << " no existe.\n";
        return;
    }

    std::vector<std::vector<char>> matriz = secEncontrada->obtenerMatriz();
    int filas = matriz.size();

    if (i < 0 || i >= filas || j < 0 || j >= matriz[i].size()) {
        std::cout << "La base en la posición [" << i << "," << j << "] no existe.\n";
        return;
    }
    std::vector<std::vector<double>> dist(filas);
    std::vector<std::vector<std::pair<int, int>>> padre(filas);

    for(int k=0; k<filas; k++){
        dist[k].resize(matriz[k].size(), std::numeric_limits<double>::infinity());
        padre[k].resize(matriz[k].size(), {-1, -1});
    }

    std::priority_queue<NodoDijkstra, std::vector<NodoDijkstra>, std::greater<NodoDijkstra>> pq;
    dist[i][j] = 0;
    pq.push({0.0, {i, j}});

    int dFila[] = {-1, 1, 0, 0};
    int dCol[] = {0, 0, -1, 1};

    while (!pq.empty()) {
        double costoActual = pq.top().first;
        int f = pq.top().second.first;
        int c = pq.top().second.second;
        pq.pop();

        if (costoActual > dist[f][c]) continue;

        for (int k = 0; k < 4; k++) {
            int nf = f + dFila[k];
            int nc = c + dCol[k];
            if (nf >= 0 && nf < filas && nc >= 0 && nc < matriz[nf].size()) {
                double peso = 1.0 / (1.0 + std::abs((int)matriz[f][c] - (int)matriz[nf][nc]));
                if (dist[f][c] + peso < dist[nf][nc]) {
                    dist[nf][nc] = dist[f][c] + peso;
                    padre[nf][nc] = {f, c};
                    pq.push({dist[nf][nc], {nf, nc}});
                }
            }
        }
    }
    char baseOrigen = matriz[i][j];
    double maxDist = -1.0;
    int rx = -1, ry = -1;

    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < matriz[f].size(); c++) {
            if (f == i && c == j) continue;

            if (dist[f][c] != std::numeric_limits<double>::infinity() && matriz[f][c] == baseOrigen) {
                if (dist[f][c] > maxDist) {
                    maxDist = dist[f][c];
                    rx = f;
                    ry = c;
                }
            }
        }
    }

    if (rx != -1) {
        std::cout << "Para la secuencia " << descripcion_secuencia << ", la base remota está ubicada en [" 
                  << rx << "," << ry << "].\n";
        
        std::vector<char> ruta;
        std::pair<int, int> actual = {rx, ry};
        while (actual.first != -1) {
            ruta.push_back(matriz[actual.first][actual.second]);
            actual = padre[actual.first][actual.second];
        }
        std::reverse(ruta.begin(), ruta.end());

        std::cout << "La ruta entre la base en [" << i << "," << j << "] y la base remota en [" 
                  << rx << "," << ry << "] es:\n";
        for (size_t k = 0; k < ruta.size(); k++) {
            std::cout << ruta[k];
            if (k < ruta.size() - 1) std::cout << ", ";
        }
        std::cout << "\nEl costo total de la ruta es: " << maxDist << "\n";
    } else {
        std::cout << "No se encontró ninguna base remota alcanzable idéntica a la original.\n";
    }
}
