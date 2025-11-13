#include <iostream>
#include <string>
#include <algorithm> // para std::min

using namespace std;

/* =========  MODELOS  ========= */

struct Producto {
    string codigo;
    string descripcion;
    double precioUnitario;
    string fechaIngreso;
    int stock;
};

struct OrdenVenta {
    int codigoOrden;
    string fechaOrden;
    string codigoProducto;
    int cantidad;
};

struct VentaRealizada {
    int codigoOrden;
    string fechaOrden;
    string codigoProducto;
    int cantidadVendida;
    double montoTotal;
};

/* =========  PILA DE PRODUCTOS (UEPS)  ========= */

struct NodoPila {
    Producto dato;
    NodoPila* sig;
};

class PilaProductos {
private:
    NodoPila* tope;
public:
    PilaProductos() : tope(nullptr) {}

    bool estaVacia() const {
        return tope == nullptr;
    }

    void push(const Producto& p) {
        NodoPila* nuevo = new NodoPila{p, tope};
        tope = nuevo;
    }

    bool pop(Producto &salida) {
        if (estaVacia()) return false;
        NodoPila* aux = tope;
        salida = aux->dato;
        tope = aux->sig;
        delete aux;
        return true;
    }

    bool peek(Producto &salida) const {
        if (estaVacia()) return false;
        salida = tope->dato;
        return true;
    }

    void mostrar() const {
        cout << "\n--- Productos en la pila (tope -> fondo) ---\n";
        NodoPila* aux = tope;
        while (aux) {
            const Producto &p = aux->dato;
            cout << "Codigo: " << p.codigo
                 << " | Desc: " << p.descripcion
                 << " | Precio: " << p.precioUnitario
                 << " | Fecha Ing.: " << p.fechaIngreso
                 << " | Stock: " << p.stock << '\n';
            aux = aux->sig;
        }
        cout << "-------------------------------------------\n";
    }

    // Muestra solo productos con precio > precioMin
    void mostrarMayoresA(double precioMin) const {
        cout << "\nProductos con precio mayor a " << precioMin << ":\n";
        NodoPila* aux = tope;
        bool hay = false;
        while (aux) {
            const Producto &p = aux->dato;
            if (p.precioUnitario > precioMin) {
                hay = true;
                cout << "Codigo: " << p.codigo
                     << " | Desc: " << p.descripcion
                     << " | Precio: " << p.precioUnitario
                     << " | Stock: " << p.stock << '\n';
            }
            aux = aux->sig;
        }
        if (!hay) cout << "No hay productos que superen ese precio.\n";
    }

    // Venta UEPS de un producto, modificando la pila.
    // Devuelve cantidad efectivamente vendida y monto total.
    void venderProductoUEPS(const string &codProd, int cantidadSolicitada,
                            int &cantidadVendida, double &montoTotal) {
        cantidadVendida = 0;
        montoTotal = 0.0;

        PilaProductos aux; // pila auxiliar para reconstruir
        Producto p;

        while (!estaVacia() && cantidadVendida < cantidadSolicitada) {
            pop(p);
            if (p.codigo == codProd && p.stock > 0) {
                int posible = min(p.stock, cantidadSolicitada - cantidadVendida);
                p.stock -= posible;
                cantidadVendida += posible;
                montoTotal += posible * p.precioUnitario;
            }
            // Si aún tiene stock, lo regresamos luego
            if (p.stock > 0)
                aux.push(p);
        }

        // Regresar todo a la pila original para mantener orden UEPS
        Producto tmp;
        while (!aux.estaVacia()) {
            aux.pop(tmp);
            push(tmp);
        }
    }
};

/* =========  COLA GENERICA PARA ORDENES Y VENTAS  ========= */

template <typename T>
struct NodoCola {
    T dato;
    NodoCola<T>* sig;
};

template <typename T>
class Cola {
private:
    NodoCola<T>* frente;
    NodoCola<T>* fin;
public:
    Cola() : frente(nullptr), fin(nullptr) {}

    bool estaVacia() const { return frente == nullptr; }

    void encolar(const T& x) {
        NodoCola<T>* nuevo = new NodoCola<T>{x, nullptr};
        if (estaVacia()) {
            frente = fin = nuevo;
        } else {
            fin->sig = nuevo;
            fin = nuevo;
        }
    }

    bool desencolar(T &salida) {
        if (estaVacia()) return false;
        NodoCola<T>* aux = frente;
        salida = aux->dato;
        frente = aux->sig;
        if (!frente) fin = nullptr;
        delete aux;
        return true;
    }

    // Extraer un elemento por clave (por ejemplo, orden urgente)
    bool extraerPor(int clave, bool (*esClave)(const T&, int), T &salida) {
        if (estaVacia()) return false;

        Cola<T> aux;
        bool encontrado = false;

        T x;
        while (desencolar(x)) {
            if (!encontrado && esClave(x, clave)) {
                salida = x;
                encontrado = true;
            } else {
                aux.encolar(x);
            }
        }

        // restaurar la cola original
        while (!aux.estaVacia()) {
            aux.desencolar(x);
            encolar(x);
        }

        return encontrado;
    }
};

/* =========  FUNCIONES DE REPORTE (USAN COPIA DE LA COLA)  ========= */

double calcularVentasPorFecha(Cola<VentaRealizada> ventas, const string &fechaBuscada) {
    double total = 0.0;
    VentaRealizada v;
    while (!ventas.estaVacia()) {
        ventas.desencolar(v);
        if (v.fechaOrden == fechaBuscada)
            total += v.montoTotal;
    }
    return total;
}

double calcularVentasPorProducto(Cola<VentaRealizada> ventas, const string &codigoProd) {
    double total = 0.0;
    VentaRealizada v;
    while (!ventas.estaVacia()) {
        ventas.desencolar(v);
        if (v.codigoProducto == codigoProd)
            total += v.montoTotal;
    }
    return total;
}

/* =========  PROGRAMA PRINCIPAL  ========= */

bool esMismaOrden(const OrdenVenta &o, int codigo) {
    return o.codigoOrden == codigo;
}

int main() {
    PilaProductos pilaProductos;
    Cola<OrdenVenta> colaOrdenes;
    Cola<VentaRealizada> colaVentas;

    double gananciaTotal = 0.0;
    int ultimoCodigoOrden = 0;

    int opcion;
    do {
        cout << "\n===== MENU TIENDA DE REPUESTOS (Pilas y Colas) =====\n";
        cout << "1. Registrar producto (push a pila UEPS)\n";
        cout << "2. Registrar orden de venta (encolar)\n";
        cout << "3. Atender siguiente orden de venta (de la cola)\n";
        cout << "4. Atender orden de venta URGENTE (por codigo de orden)\n";
        cout << "5. Reporte: ventas por fecha\n";
        cout << "6. Reporte: ventas por producto\n";
        cout << "7. Mostrar productos que exceden un precio\n";
        cout << "8. Mostrar ganancia total de la tienda\n";
        cout << "9. Mostrar todos los productos (debug)\n";
        cout << "0. Salir\n";
        cout << "Seleccione opcion: ";
        cin >> opcion;
        cin.ignore(); // limpiar salto de linea

        if (opcion == 1) {
            Producto p;
            cout << "Codigo del producto: ";
            getline(cin, p.codigo);
            cout << "Descripcion: ";
            getline(cin, p.descripcion);
            cout << "Precio unitario: ";
            cin >> p.precioUnitario;
            cin.ignore();
            cout << "Fecha de ingreso (dd/mm/aaaa): ";
            getline(cin, p.fechaIngreso);
            cout << "Stock (unidades): ";
            cin >> p.stock;
            cin.ignore();

            pilaProductos.push(p);
            cout << "Producto registrado en la pila (UEPS).\n";

        } else if (opcion == 2) {
            OrdenVenta o;
            o.codigoOrden = ++ultimoCodigoOrden;
            cout << "Codigo de orden asignado: " << o.codigoOrden << '\n';
            cout << "Fecha de la orden (dd/mm/aaaa): ";
            getline(cin, o.fechaOrden);
            cout << "Codigo del producto: ";
            getline(cin, o.codigoProducto);
            cout << "Cantidad solicitada: ";
            cin >> o.cantidad;
            cin.ignore();

            colaOrdenes.encolar(o);
            cout << "Orden de venta encolada.\n";

        } else if (opcion == 3) {
            OrdenVenta o;
            if (!colaOrdenes.desencolar(o)) {
                cout << "No hay ordenes en la cola.\n";
            } else {
                cout << "\nAtendiendo orden " << o.codigoOrden << "...\n";
                int cantVendida;
                double monto;
                pilaProductos.venderProductoUEPS(o.codigoProducto, o.cantidad,
                                                 cantVendida, monto);

                if (cantVendida == 0) {
                    cout << "No se pudo atender la orden. No hay stock para el producto.\n";
                } else if (cantVendida < o.cantidad) {
                    cout << "Orden atendida parcialmente. Vendidas " << cantVendida
                         << " de " << o.cantidad << " unidades.\n";
                } else {
                    cout << "Orden atendida completamente. Unidades vendidas: "
                         << cantVendida << '\n';
                }

                if (cantVendida > 0) {
                    VentaRealizada v;
                    v.codigoOrden = o.codigoOrden;
                    v.fechaOrden = o.fechaOrden;
                    v.codigoProducto = o.codigoProducto;
                    v.cantidadVendida = cantVendida;
                    v.montoTotal = monto;

                    colaVentas.encolar(v);
                    gananciaTotal += monto;
                    cout << "Monto de la venta: " << monto << '\n';
                }
            }

        } else if (opcion == 4) {
            int codUrgente;
            cout << "Ingrese codigo de la orden URGENTE: ";
            cin >> codUrgente;
            cin.ignore();

            OrdenVenta o;
            if (!colaOrdenes.extraerPor(codUrgente, esMismaOrden, o)) {
                cout << "No se encontro la orden en la cola.\n";
            } else {
                cout << "\nAtendiendo ORDEN URGENTE " << o.codigoOrden << "...\n";
                int cantVendida;
                double monto;
                pilaProductos.venderProductoUEPS(o.codigoProducto, o.cantidad,
                                                 cantVendida, monto);

                if (cantVendida == 0) {
                    cout << "No se pudo atender la orden. No hay stock para el producto.\n";
                } else if (cantVendida < o.cantidad) {
                    cout << "Orden atendida parcialmente. Vendidas " << cantVendida
                         << " de " << o.cantidad << " unidades.\n";
                } else {
                    cout << "Orden atendida completamente. Unidades vendidas: "
                         << cantVendida << '\n';
                }

                if (cantVendida > 0) {
                    VentaRealizada v;
                    v.codigoOrden = o.codigoOrden;
                    v.fechaOrden = o.fechaOrden;
                    v.codigoProducto = o.codigoProducto;
                    v.cantidadVendida = cantVendida;
                    v.montoTotal = monto;

                    colaVentas.encolar(v);
                    gananciaTotal += monto;
                    cout << "Monto de la venta: " << monto << '\n';
                }
            }

        } else if (opcion == 5) {
            string fecha;
            cout << "Ingrese la fecha a consultar (dd/mm/aaaa): ";
            getline(cin, fecha);
            double total = calcularVentasPorFecha(colaVentas, fecha);
            cout << "Ventas en la fecha " << fecha << ": " << total << '\n';

        } else if (opcion == 6) {
            string codProd;
            cout << "Ingrese codigo de producto: ";
            getline(cin, codProd);
            double total = calcularVentasPorProducto(colaVentas, codProd);
            cout << "Ventas para el producto " << codProd << ": " << total << '\n';

        } else if (opcion == 7) {
            double precio;
            cout << "Ingrese precio minimo: ";
            cin >> precio;
            cin.ignore();
            pilaProductos.mostrarMayoresA(precio);

        } else if (opcion == 8) {
            cout << "Ganancia total de la tienda: " << gananciaTotal << '\n';

        } else if (opcion == 9) {
            pilaProductos.mostrar();
        }

    } while (opcion != 0);

    cout << "Programa finalizado.\n";
    return 0;
}
