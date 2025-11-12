#include <iostream>
#include <string>
#include <limits>

using namespace std;

// ------------------------- ESTRUCTURAS -------------------------

struct Persona {
    string apellidosNombres;
    char sexo; // 'M' o 'F'
    int edad;
};

struct NodoPersona {
    Persona info;
    NodoPersona* sig;

    NodoPersona(const Persona& p) : info(p), sig(nullptr) {}
};

// Nodo de la lista de ingresos (lista simple)
struct NodoIngreso {
    int codigo;                 // 1..6
    string descripcion;         // texto del rango
    NodoPersona* ultimo;        // puntero al ULTIMO de la lista circular
    NodoIngreso* sig;

    NodoIngreso(int c, const string& d)
        : codigo(c), descripcion(d), ultimo(nullptr), sig(nullptr) {}
};

// ------------------ FUNCIONES AUXILIARES -----------------------

int leerEntero(const string& mensaje, int minV, int maxV) {
    int x;
    while (true) {
        cout << mensaje;
        if (cin >> x && x >= minV && x <= maxV) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return x;
        }
        cout << "Valor invalido. Intente de nuevo.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

char leerSexo() {
    char s;
    while (true) {
        cout << "Sexo (M/F): ";
        cin >> s;
        s = toupper(s);
        if (s == 'M' || s == 'F') {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return s;
        }
        cout << "Sexo invalido. Intente de nuevo.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// Devuelve el intervalo de edad 1..6
int intervaloEdad(int edad) {
    if (edad < 20) return 1;
    if (edad <= 29) return 2;
    if (edad <= 39) return 3;
    if (edad <= 49) return 4;
    if (edad <= 59) return 5;
    return 6;
}

// ------------------ MANEJO LISTA DE INGRESOS -------------------

NodoIngreso* crearListaIngresos() {
    // Crea la lista simple con 6 nodos ya enlazados
    NodoIngreso* cabeza = new NodoIngreso(1, "1 - menor de 2000");
    NodoIngreso* n2 = new NodoIngreso(2, "2 - de 2000 a 2999");
    NodoIngreso* n3 = new NodoIngreso(3, "3 - de 3000 a 3999");
    NodoIngreso* n4 = new NodoIngreso(4, "4 - de 4000 a 4999");
    NodoIngreso* n5 = new NodoIngreso(5, "5 - de 5000 a 9999");
    NodoIngreso* n6 = new NodoIngreso(6, "6 - mayor de 10000");

    cabeza->sig = n2;
    n2->sig = n3;
    n3->sig = n4;
    n4->sig = n5;
    n5->sig = n6;
    n6->sig = nullptr;

    return cabeza;
}

NodoIngreso* buscarIngreso(NodoIngreso* cabeza, int codigo) {
    NodoIngreso* p = cabeza;
    while (p != nullptr) {
        if (p->codigo == codigo) return p;
        p = p->sig;
    }
    return nullptr;
}

// --------------- MANEJO LISTA CIRCULAR DE PERSONAS -------------
// last = puntero al ultimo nodo de la lista circular

void insertarOrdenado(NodoPersona*& last, const Persona& per) {
    NodoPersona* nuevo = new NodoPersona(per);

    if (last == nullptr) {
        // Primera persona del ingreso
        last = nuevo;
        last->sig = last;
        return;
    }

    NodoPersona* primero = last->sig;

    // Si el nuevo es >= alfabeticamente que el ultimo, va al final
    if (per.apellidosNombres >= last->info.apellidosNombres) {
        nuevo->sig = last->sig;
        last->sig = nuevo;
        last = nuevo; // ahora es el ultimo
        return;
    }

    // Si el nuevo es <= que el primero, se inserta al inicio
    if (per.apellidosNombres <= primero->info.apellidosNombres) {
        nuevo->sig = primero;
        last->sig = nuevo; // el ultimo no cambia
        return;
    }

    // Insercion en el medio
    NodoPersona* actual = primero;
    while (actual->sig != primero &&
           actual->sig->info.apellidosNombres < per.apellidosNombres) {
        actual = actual->sig;
    }

    nuevo->sig = actual->sig;
    actual->sig = nuevo;
}

bool eliminarPorNombre(NodoPersona*& last, const string& nombre) {
    if (last == nullptr) return false;

    NodoPersona* actual = last->sig; // primero
    NodoPersona* previo = last;

    do {
        if (actual->info.apellidosNombres == nombre) {
            // Un solo nodo
            if (actual == last && actual == last->sig) {
                delete actual;
                last = nullptr;
                return true;
            }

            // Elimina primero
            if (actual == last->sig) {
                previo->sig = actual->sig;
                last->sig = actual->sig;
                delete actual;
                return true;
            }

            // Elimina ultimo
            if (actual == last) {
                previo->sig = actual->sig;
                last = previo;
                delete actual;
                return true;
            }

            // Elimina nodo intermedio
            previo->sig = actual->sig;
            delete actual;
            return true;
        }

        previo = actual;
        actual = actual->sig;
    } while (actual != last->sig);

    return false; // no encontrado
}

void mostrarLista(NodoPersona* last) {
    if (last == nullptr) {
        cout << "No hay personas en este ingreso.\n";
        return;
    }
    NodoPersona* actual = last->sig; // primero
    do {
        cout << "Nombre : " << actual->info.apellidosNombres << "\n";
        cout << "Sexo   : " << actual->info.sexo << "\n";
        cout << "Edad   : " << actual->info.edad << "\n";
        cout << "-----------------------------\n";
        actual = actual->sig;
    } while (actual != last->sig);
}

void mostrarPorSexo(NodoPersona* last, char sexo) {
    if (last == nullptr) {
        cout << "No hay personas en este ingreso.\n";
        return;
    }
    bool encontrado = false;
    NodoPersona* actual = last->sig;
    do {
        if (actual->info.sexo == sexo) {
            cout << "Nombre : " << actual->info.apellidosNombres << "\n";
            cout << "Sexo   : " << actual->info.sexo << "\n";
            cout << "Edad   : " << actual->info.edad << "\n";
            cout << "-----------------------------\n";
            encontrado = true;
        }
        actual = actual->sig;
    } while (actual != last->sig);

    if (!encontrado)
        cout << "No hay personas con ese sexo en este ingreso.\n";
}

void mostrarPorSexoYEdad(NodoPersona* last, char sexo, int intervalo) {
    if (last == nullptr) {
        cout << "No hay personas en este ingreso.\n";
        return;
    }
    bool encontrado = false;
    NodoPersona* actual = last->sig;
    do {
        if (actual->info.sexo == sexo &&
            intervaloEdad(actual->info.edad) == intervalo) {
            cout << "Nombre : " << actual->info.apellidosNombres << "\n";
            cout << "Sexo   : " << actual->info.sexo << "\n";
            cout << "Edad   : " << actual->info.edad << "\n";
            cout << "-----------------------------\n";
            encontrado = true;
        }
        actual = actual->sig;
    } while (actual != last->sig);

    if (!encontrado)
        cout << "No hay personas que cumplan esos criterios.\n";
}

// ----------------------------- MAIN ----------------------------

int main() {
    NodoIngreso* listaIngresos = crearListaIngresos();

    int opcion;
    do {
        cout << "\n===== TERCER PROYECTO E.D. (LISTAS) =====\n";
        cout << "1. Insertar persona\n";
        cout << "2. Eliminar persona\n";
        cout << "3. Mostrar personas de un ingreso\n";
        cout << "4. Mostrar personas de un ingreso por sexo\n";
        cout << "5. Mostrar personas por sexo, ingreso e intervalo de edad\n";
        cout << "0. Salir\n";
        cout << "Opcion: ";
        cin >> opcion;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (opcion == 1) {
            // (a) Insertar
            Persona p;
            cout << "Ingrese Apellidos y Nombres: ";
            getline(cin, p.apellidosNombres);
            p.sexo = leerSexo();
            p.edad = leerEntero("Edad: ", 0, 120);
            int codIng = leerEntero("Codigo de Ingreso (1..6): ", 1, 6);

            NodoIngreso* ing = buscarIngreso(listaIngresos, codIng);
            if (!ing) {
                cout << "Ingreso inexistente.\n";
            } else {
                insertarOrdenado(ing->ultimo, p);
                cout << "Persona insertada correctamente.\n";
            }

        } else if (opcion == 2) {
            // (b) Eliminar
            int codIng = leerEntero("Codigo de Ingreso (1..6): ", 1, 6);
            NodoIngreso* ing = buscarIngreso(listaIngresos, codIng);
            if (!ing) {
                cout << "Ingreso inexistente.\n";
            } else {
                string nombre;
                cout << "Apellidos y Nombres de la persona a eliminar: ";
                getline(cin, nombre);
                if (eliminarPorNombre(ing->ultimo, nombre))
                    cout << "Persona eliminada.\n";
                else
                    cout << "Persona no encontrada.\n";
            }

        } else if (opcion == 3) {
            // (c) Mostrar por ingreso
            int codIng = leerEntero("Codigo de Ingreso (1..6): ", 1, 6);
            NodoIngreso* ing = buscarIngreso(listaIngresos, codIng);
            if (!ing) {
                cout << "Ingreso inexistente.\n";
            } else {
                cout << "\nPersonas con Ingreso " << ing->descripcion << ":\n";
                cout << "----------------------------------------\n";
                mostrarLista(ing->ultimo);
            }

        } else if (opcion == 4) {
            // (d) Mostrar por sexo e ingreso
            int codIng = leerEntero("Codigo de Ingreso (1..6): ", 1, 6);
            char sexo = leerSexo();
            NodoIngreso* ing = buscarIngreso(listaIngresos, codIng);
            if (!ing) {
                cout << "Ingreso inexistente.\n";
            } else {
                cout << "\nPersonas con Ingreso " << ing->descripcion
                     << " y sexo " << sexo << ":\n";
                cout << "----------------------------------------\n";
                mostrarPorSexo(ing->ultimo, sexo);
            }

        } else if (opcion == 5) {
            // (e) Mostrar por sexo, ingreso e intervalo de edad
            int codIng = leerEntero("Codigo de Ingreso (1..6): ", 1, 6);
            char sexo = leerSexo();
            cout << "Intervalo de edad:\n";
            cout << "1: menor de 20\n";
            cout << "2: 20 a 29\n";
            cout << "3: 30 a 39\n";
            cout << "4: 40 a 49\n";
            cout << "5: 50 a 59\n";
            cout << "6: mayor de 60\n";
            int inter = leerEntero("Opcion (1..6): ", 1, 6);

            NodoIngreso* ing = buscarIngreso(listaIngresos, codIng);
            if (!ing) {
                cout << "Ingreso inexistente.\n";
            } else {
                cout << "\nPersonas con Ingreso " << ing->descripcion
                     << ", sexo " << sexo
                     << " e intervalo de edad " << inter << ":\n";
                cout << "----------------------------------------\n";
                mostrarPorSexoYEdad(ing->ultimo, sexo, inter);
            }
        }

    } while (opcion != 0);

    cout << "Fin del programa.\n";
    return 0;
}
