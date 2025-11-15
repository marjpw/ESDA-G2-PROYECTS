#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <limits>

using namespace std;

// ---------------------------------------------------------
//          ESTRUCTURA DEL NODO (HIJO - HERMANO)
// ---------------------------------------------------------
struct Nodo {
    int valor;
    Nodo* hijoIzquierdo;
    Nodo* hermanoDerecho;

    Nodo(int val) : valor(val), hijoIzquierdo(nullptr), hermanoDerecho(nullptr) {}
};

//              CLASE ÁRBOL GENERAL (n-ario)
//     Representación Hijo Izquierdo – Hermano Derecho
// ---------------------------------------------------------
class ArbolGeneral {
private:
    Nodo* raiz;

    // --- Búsqueda en árbol general ---
    Nodo* buscar(Nodo* n, int valorBuscado) {
        if (n == nullptr) return nullptr;
        if (n->valor == valorBuscado) return n;

        // Buscar en hijos
        Nodo* encontrado = buscar(n->hijoIzquierdo, valorBuscado);
        if (encontrado != nullptr) return encontrado;

        // Buscar en hermanos
        return buscar(n->hermanoDerecho, valorBuscado);
    }

    //              RECORRIDO PREORDEN CORRECTO
    //         Nodo → Hijos (de izquierda a derecha)
    // ---------------------------------------------------------
    void preorden(Nodo* n) {
        if (n == nullptr) return;

        cout << n->valor << " ";

        Nodo* hijo = n->hijoIzquierdo;
        while (hijo != nullptr) {
            preorden(hijo);
            hijo = hijo->hermanoDerecho;
        }
    }

    //              RECORRIDO INORDEN CORRECTO
    //   Primer hijo → Nodo → Demás hijos (HI-HD n-ario)
    // ---------------------------------------------------------
    void inorden(Nodo* n) {
        if (n == nullptr) return;

        Nodo* hijo = n->hijoIzquierdo;

        // Primer hijo
        if (hijo != nullptr) {
            inorden(hijo);
            hijo = hijo->hermanoDerecho;
        }

        // Nodo
        cout << n->valor << " ";

        // Más hijos
        while (hijo != nullptr) {
            inorden(hijo);
            hijo = hijo->hermanoDerecho;
        }
    }

    //              RECORRIDO POSTORDEN CORRECTO
    //            Hijos → Nodo (n-ario HI-HD)
    // ---------------------------------------------------------
    void postorden(Nodo* n) {
        if (n == nullptr) return;

        Nodo* hijo = n->hijoIzquierdo;

        while (hijo != nullptr) {
            postorden(hijo);
            hijo = hijo->hermanoDerecho;
        }

        cout << n->valor << " ";
    }

    //          MOSTRAR CAMINOS (Raíz a cada hoja)
    // ---------------------------------------------------------
    void mostrarCaminosRecursivo(Nodo* n, vector<int>& camino) {
        if (n == nullptr) return;

        camino.push_back(n->valor);

        if (n->hijoIzquierdo == nullptr) {
            // Imprimir camino actual
            for (size_t i = 0; i < camino.size(); i++) {
                cout << camino[i];
                if (i < camino.size() - 1) cout << " -> ";
            }
            cout << endl;
        } else {
            Nodo* hijo = n->hijoIzquierdo;
            while (hijo != nullptr) {
                mostrarCaminosRecursivo(hijo, camino);
                hijo = hijo->hermanoDerecho;
            }
        }

        camino.pop_back();
    }

    // --- Destructor recursivo ---
    void destruir(Nodo* n) {
        if (n == nullptr) return;
        destruir(n->hijoIzquierdo);
        destruir(n->hermanoDerecho);
        delete n;
    }

public:
    ArbolGeneral() : raiz(nullptr) {}
    ~ArbolGeneral() { destruir(raiz); }

    //                  INSERCIÓN HI-HD
    // ---------------------------------------------------------
    void insertar(int valorNuevo, int valorPadre) {
        Nodo* nuevoNodo = new Nodo(valorNuevo);

        // Insertar raíz
        if (valorPadre == 0) {
            if (raiz == nullptr) {
                raiz = nuevoNodo;
                cout << "Nodo " << valorNuevo << " insertado como raíz.\n";
            } else {
                cout << "Error: la raíz ya existe.\n";
                delete nuevoNodo;
            }
            return;
        }

        // Insertar hijo de un padre existente
        if (raiz == nullptr) {
            cout << "Error: inserte primero una raíz.\n";
            delete nuevoNodo;
            return;
        }

        Nodo* padre = buscar(raiz, valorPadre);
        if (padre == nullptr) {
            cout << "No existe el nodo padre " << valorPadre << ".\n";
            delete nuevoNodo;
            return;
        }

        if (padre->hijoIzquierdo == nullptr) {
            padre->hijoIzquierdo = nuevoNodo;
        } else {
            Nodo* temp = padre->hijoIzquierdo;
            while (temp->hermanoDerecho != nullptr) {
                temp = temp->hermanoDerecho;
            }
            temp->hermanoDerecho = nuevoNodo;
        }

        cout << "Nodo " << valorNuevo << " insertado como hijo de " << valorPadre << ".\n";
    }


    //                 RECORRIDOS PÚBLICOS
    // ---------------------------------------------------------
    void recorridoPreorden() {
        if (raiz == nullptr) { cout << "Árbol vacío.\n"; return; }
        cout << "Preorden: ";
        preorden(raiz);
        cout << endl;
    }

    void recorridoInorden() {
        if (raiz == nullptr) { cout << "Árbol vacío.\n"; return; }
        cout << "Inorden: ";
        inorden(raiz);
        cout << endl;
    }

    void recorridoPostorden() {
        if (raiz == nullptr) { cout << "Árbol vacío.\n"; return; }
        cout << "Postorden: ";
        postorden(raiz);
        cout << endl;
    }

    void recorridoPorNiveles() {
        if (raiz == nullptr) { cout << "Árbol vacío.\n"; return; }

        cout << "Por niveles: ";
        queue<Nodo*> cola;
        cola.push(raiz);

        while (!cola.empty()) {
            Nodo* actual = cola.front();
            cola.pop();

            cout << actual->valor << " ";

            Nodo* hijo = actual->hijoIzquierdo;
            while (hijo != nullptr) {
                cola.push(hijo);
                hijo = hijo->hermanoDerecho;
            }
        }
        cout << endl;
    }

    void mostrarCaminos() {
        if (raiz == nullptr) { cout << "Árbol vacío.\n"; return; }
        cout << "Caminos raíz → hojas:\n";
        vector<int> camino;
        mostrarCaminosRecursivo(raiz, camino);
    }
};

//        FUNCIÓN SEGURO PARA LEER ENTEROS
// ---------------------------------------------------------
int leerEntero(string mensaje) {
    int valor;
    cout << mensaje;
    while (!(cin >> valor)) {
        cout << "Entrada invalida. Ingrese un número: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return valor;
}

// ---------------------------------------------------------
int main() {
    ArbolGeneral arbol;
    int opcion = -1;

    while (opcion != 0) {
        cout << "\n----- MENU ARBOL GENERAL (HI-HD) -----\n";
        cout << "1. Insertar nodo\n";
        cout << "2. Recorrido Pre-orden\n";
        cout << "3. Recorrido In-orden\n";
        cout << "4. Recorrido Post-orden\n";
        cout << "5. Recorrido por Niveles\n";
        cout << "6. Mostrar Caminos\n";
        cout << "0. Salir\n";
        cout << "--------------------------------------\n";

        opcion = leerEntero("Opción: ");

        switch (opcion) {
            case 1: {
                int val = leerEntero("Valor del nuevo nodo: ");
                int padre = leerEntero("Valor del nodo padre (0 = raíz): ");
                arbol.insertar(val, padre);
                break;
            }
            case 2: arbol.recorridoPreorden(); break;
            case 3: arbol.recorridoInorden(); break;
            case 4: arbol.recorridoPostorden(); break;
            case 5: arbol.recorridoPorNiveles(); break;
            case 6: arbol.mostrarCaminos(); break;
            case 0: cout << "Saliendo...\n"; break;
            default: cout << "Opción inválida.\n"; break;
        }

        if (opcion != 0) {
            cout << "\nPresione Enter para continuar...";
            cin.get();
        }
    }

    return 0;
}

