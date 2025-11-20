#include <iostream>
#include <cstring>
using namespace std;

// Estructura para representar una arista (vuelo)
struct Arista {
    int ValorArista;           // Número de vuelo
    struct Vertice* Destino;   // Puntero al vértice destino
    Arista* SigArista;         // Siguiente arista en la lista
};

// Estructura para representar un vértice (ciudad)
struct Vertice {
    char ValorVertice[50];     // Nombre de la ciudad
    Vertice* SigVertice;       // Siguiente vértice en la lista
    Arista* Ady;               // Lista de adyacencia (vuelos salientes)
};

class GrafoVuelos {
private:
    Vertice* Principio;

public:
    GrafoVuelos() {
        Principio = NULL;
    }

    // a) Insertar un vértice manteniendo orden alfabético
    void insertarVertice(const char* nombreCiudad) {
        // Verificar si ya existe
        Vertice* aux = Principio;
        while (aux != NULL) {
            if (strcmp(aux->ValorVertice, nombreCiudad) == 0) {
                cout << "La ciudad " << nombreCiudad << " ya existe en el grafo." << endl;
                return;
            }
            aux = aux->SigVertice;
        }

        // Crear nuevo vértice
        Vertice* nuevo = new Vertice;
        strcpy(nuevo->ValorVertice, nombreCiudad);
        nuevo->SigVertice = NULL;
        nuevo->Ady = NULL;

        // Caso 1: Lista vacía o insertar al inicio
        if (Principio == NULL || strcmp(nombreCiudad, Principio->ValorVertice) < 0) {
            nuevo->SigVertice = Principio;
            Principio = nuevo;
            cout << "Ciudad " << nombreCiudad << " insertada correctamente." << endl;
            return;
        }

        // Caso 2: Insertar en medio o al final
        Vertice* anterior = Principio;
        Vertice* actual = Principio->SigVertice;

        while (actual != NULL && strcmp(nombreCiudad, actual->ValorVertice) > 0) {
            anterior = actual;
            actual = actual->SigVertice;
        }

        nuevo->SigVertice = actual;
        anterior->SigVertice = nuevo;
        cout << "Ciudad " << nombreCiudad << " insertada correctamente." << endl;
    }

    // b) Insertar una arista (vuelo)
    void insertarVuelo(const char* origen, const char* destino, int numeroVuelo) {
        // Buscar vértice origen
        Vertice* vertOrigen = buscarVertice(origen);
        if (vertOrigen == NULL) {
            cout << "Error: La ciudad origen " << origen << " no existe." << endl;
            return;
        }

        // Buscar vértice destino
        Vertice* vertDestino = buscarVertice(destino);
        if (vertDestino == NULL) {
            cout << "Error: La ciudad destino " << destino << " no existe." << endl;
            return;
        }

        // Verificar que no exista el número de vuelo
        if (existeNumeroVuelo(numeroVuelo)) {
            cout << "Error: El numero de vuelo " << numeroVuelo << " ya existe." << endl;
            return;
        }

        // Crear nueva arista
        Arista* nueva = new Arista;
        nueva->ValorArista = numeroVuelo;
        nueva->Destino = vertDestino;
        nueva->SigArista = vertOrigen->Ady;
        vertOrigen->Ady = nueva;

        cout << "Vuelo " << numeroVuelo << " de " << origen << " a " << destino 
             << " insertado correctamente." << endl;
    }

    // c) Eliminar un vértice y sus aristas
    void eliminarVertice(const char* nombreCiudad) {
        if (Principio == NULL) {
            cout << "El grafo esta vacio." << endl;
            return;
        }

        // Buscar y eliminar el vértice
        Vertice* anterior = NULL;
        Vertice* actual = Principio;

        while (actual != NULL && strcmp(actual->ValorVertice, nombreCiudad) != 0) {
            anterior = actual;
            actual = actual->SigVertice;
        }

        if (actual == NULL) {
            cout << "La ciudad " << nombreCiudad << " no existe." << endl;
            return;
        }

        // Eliminar lista de adyacencia del vértice
        Arista* aristaAux = actual->Ady;
        while (aristaAux != NULL) {
            Arista* temp = aristaAux;
            aristaAux = aristaAux->SigArista;
            delete temp;
        }

        // Eliminar aristas que apuntan a este vértice
        eliminarAristasHaciaVertice(actual);

        // Eliminar el vértice de la lista
        if (anterior == NULL) {
            Principio = actual->SigVertice;
        } else {
            anterior->SigVertice = actual->SigVertice;
        }

        delete actual;
        cout << "Ciudad " << nombreCiudad << " eliminada correctamente." << endl;
    }

    // d) Eliminar un vuelo dado su número
    void eliminarVuelo(int numeroVuelo) {
        Vertice* vert = Principio;
        bool encontrado = false;

        while (vert != NULL) {
            Arista* anterior = NULL;
            Arista* actual = vert->Ady;

            while (actual != NULL) {
                if (actual->ValorArista == numeroVuelo) {
                    if (anterior == NULL) {
                        vert->Ady = actual->SigArista;
                    } else {
                        anterior->SigArista = actual->SigArista;
                    }
                    delete actual;
                    cout << "Vuelo " << numeroVuelo << " eliminado correctamente." << endl;
                    encontrado = true;
                    return;
                }
                anterior = actual;
                actual = actual->SigArista;
            }
            vert = vert->SigVertice;
        }

        if (!encontrado) {
            cout << "El vuelo " << numeroVuelo << " no existe." << endl;
        }
    }

    // e) Mostrar todos los vuelos
    void mostrarVuelos() {
        if (Principio == NULL) {
            cout << "El grafo está vacío." << endl;
            return;
        }

        cout << "\n=== LISTADO DE VUELOS ===" << endl;
        cout << "Num. Vuelo | Ciudad Origen -> Ciudad Destino" << endl;
        cout << "----------------------------------------------" << endl;

        Vertice* vert = Principio;
        while (vert != NULL) {
            Arista* arista = vert->Ady;
            while (arista != NULL) {
                cout << "   " << arista->ValorArista << "      | " 
                     << vert->ValorVertice << " -> " 
                     << arista->Destino->ValorVertice << endl;
                arista = arista->SigArista;
            }
            vert = vert->SigVertice;
        }
        cout << endl;
    }

    // Función auxiliar: mostrar estructura del grafo
    void mostrarGrafo() {
        if (Principio == NULL) {
            cout << "El grafo esta vacio." << endl;
            return;
        }

        cout << "\n=== ESTRUCTURA DEL GRAFO ===" << endl;
        Vertice* vert = Principio;
        while (vert != NULL) {
            cout << vert->ValorVertice << " -> ";
            Arista* arista = vert->Ady;
            if (arista == NULL) {
                cout << "NULL";
            }
            while (arista != NULL) {
                cout << "[Vuelo:" << arista->ValorArista << " -> " 
                     << arista->Destino->ValorVertice << "]";
                if (arista->SigArista != NULL) {
                    cout << " -> ";
                }
                arista = arista->SigArista;
            }
            cout << endl;
            vert = vert->SigVertice;
        }
        cout << endl;
    }

    ~GrafoVuelos() {
        while (Principio != NULL) {
            Vertice* temp = Principio;
            Principio = Principio->SigVertice;

            // Liberar lista de adyacencia
            while (temp->Ady != NULL) {
                Arista* tempArista = temp->Ady;
                temp->Ady = temp->Ady->SigArista;
                delete tempArista;
            }
            delete temp;
        }
    }

private:
    // Buscar un vértice por nombre
    Vertice* buscarVertice(const char* nombreCiudad) {
        Vertice* aux = Principio;
        while (aux != NULL) {
            if (strcmp(aux->ValorVertice, nombreCiudad) == 0) {
                return aux;
            }
            aux = aux->SigVertice;
        }
        return NULL;
    }

    // Verificar si existe un número de vuelo
    bool existeNumeroVuelo(int numeroVuelo) {
        Vertice* vert = Principio;
        while (vert != NULL) {
            Arista* arista = vert->Ady;
            while (arista != NULL) {
                if (arista->ValorArista == numeroVuelo) {
                    return true;
                }
                arista = arista->SigArista;
            }
            vert = vert->SigVertice;
        }
        return false;
    }

    // Eliminar aristas que tienen como destino el vértice dado
    void eliminarAristasHaciaVertice(Vertice* verticeDestino) {
        Vertice* vert = Principio;
        while (vert != NULL) {
            Arista* anterior = NULL;
            Arista* actual = vert->Ady;

            while (actual != NULL) {
                if (actual->Destino == verticeDestino) {
                    if (anterior == NULL) {
                        vert->Ady = actual->SigArista;
                    } else {
                        anterior->SigArista = actual->SigArista;
                    }
                    Arista* temp = actual;
                    actual = actual->SigArista;
                    delete temp;
                } else {
                    anterior = actual;
                    actual = actual->SigArista;
                }
            }
            vert = vert->SigVertice;
        }
    }
};

int main() {
    GrafoVuelos grafo;
    int opcion;

    do {
        cout << "\n========== SISTEMA DE VUELOS ==========" << endl;
        cout << "1. Insertar ciudad (vertice)" << endl;
        cout << "2. Insertar vuelo (arista)" << endl;
        cout << "3. Eliminar ciudad" << endl;
        cout << "4. Eliminar vuelo" << endl;
        cout << "5. Mostrar vuelos" << endl;
        cout << "6. Mostrar estructura del grafo" << endl;
        cout << "7. Cargar ejemplo del proyecto" << endl;
        cout << "0. Salir" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1: {
                char ciudad[50];
                cout << "Ingrese nombre de la ciudad: ";
                cin.ignore();
                cin.getline(ciudad, 50);
                grafo.insertarVertice(ciudad);
                break;
            }
            case 2: {
                char origen[50], destino[50];
                int numVuelo;
                cout << "Ingrese ciudad origen: ";
                cin.ignore();
                cin.getline(origen, 50);
                cout << "Ingrese ciudad destino: ";
                cin.getline(destino, 50);
                cout << "Ingrese numero de vuelo: ";
                cin >> numVuelo;
                grafo.insertarVuelo(origen, destino, numVuelo);
                break;
            }
            case 3: {
                char ciudad[50];
                cout << "Ingrese nombre de la ciudad a eliminar: ";
                cin.ignore();
                cin.getline(ciudad, 50);
                grafo.eliminarVertice(ciudad);
                break;
            }
            case 4: {
                int numVuelo;
                cout << "Ingrese número de vuelo a eliminar: ";
                cin >> numVuelo;
                grafo.eliminarVuelo(numVuelo);
                break;
            }
            case 5:
                grafo.mostrarVuelos();
                break;
            case 6:
                grafo.mostrarGrafo();
                break;
            case 7: {
                // Cargar ejemplo del PDF
                cout << "\nCargando ejemplo del proyecto..." << endl;
                grafo.insertarVertice("Boston");
                grafo.insertarVertice("Chicago");
                grafo.insertarVertice("Denver");
                grafo.insertarVertice("Miami");
                grafo.insertarVertice("Reno");
                grafo.insertarVuelo("Denver", "Chicago", 201);
                grafo.insertarVuelo("Denver", "Boston", 402);
                grafo.insertarVuelo("Chicago", "Denver", 305);
                grafo.insertarVuelo("Chicago", "Reno", 308);
                grafo.insertarVuelo("Miami", "Denver", 203);
                grafo.insertarVuelo("Miami", "Reno", 301);
                grafo.insertarVuelo("Reno", "Denver", 204);
                
                cout << "Ejemplo cargado exitosamente." << endl;
                break;
            }
            case 0:
                cout << "Saliendo del programa..." << endl;
                break;
            default:
                cout << "Opcion invalida." << endl;
        }
    } while (opcion != 0);

    return 0;
}
