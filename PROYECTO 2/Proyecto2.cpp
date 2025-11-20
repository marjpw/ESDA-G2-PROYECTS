#include<iostream>
#include<vector>
#include<stdlib.h>
#include<string>

class Depa {
private:
    int clave;
    double extension;
    std::string ubicacion;
    double price;
    bool disponibilidad;

public:
    Depa(int clave, double extension, std::string ubicacion, double price, bool disponibilidad)
        : clave(clave), extension(extension), ubicacion(ubicacion), price(price), disponibilidad(disponibilidad) {}

    int getClave() const { return clave; }
    double getExtension() const { return extension; }
    std::string getUbicacion() const { return ubicacion; }
    double getPrecio() const { return price; }
    bool estaDisponible() const { return disponibilidad; }

    void setDisponible(bool d) { disponibilidad = d; }
    void setPrecio(double p) { price = p; }

    void mostrar() const {
        std::cout << "Clave: " << clave
            << " | Extension: " << extension << " m2"
            << " | Ubicacion: " << ubicacion
            << " | Precio: $" << price
            << " | Disponible: " << (disponibilidad ? "SI" : "NO") << std::endl;
    }
};

bool existClave(const std::vector<Depa>& d, int clave) {
    for (auto& arr : d)
        if (arr.getClave() == clave) return true;
    return false;
}

void registrarDepa(std::vector<Depa>& d, std::string ubi[]) {
    int clave, ubic;
    double ext, precio;
    bool disponible;

    std::cout << "\tDatos del departamento\n";
    std::cout << "Clave: "; std::cin >> clave;
    std::cout << "Extension: "; std::cin >> ext;
    std::cout << "Ubicacion (1=Excelente, 2=Buena, 3=Regular, 4=Mala): ";
    std::cin >> ubic;
    std::cout << "Precio: "; std::cin >> precio;
    std::cout << "Disponibilidad (1=Si, 0=No): "; std::cin >> disponible;

    if (ubic < 1 || ubic > 4) {
        std::cout << "Ubicacion invalida.\n";
        return;
    }

    if (existClave(d, clave)) {
        std::cout << "Error. La clave ya existe.\n";
        return;
    }

    Depa nuevo(clave, ext, ubi[ubic - 1], precio, disponible);

    // INSERTAR ORDENADO POR PRECIO
    auto it = d.begin();
    while (it != d.end() && it->getPrecio() < nuevo.getPrecio())
        ++it;

    d.insert(it, nuevo);

    std::cout << "Departamento agregado exitosamente.\n";
}

void eliminarRegistroDepa(std::vector<Depa>& d) {
    int clave;
    std::cout << "\tEliminar departamento\n";
    std::cout << "Clave: "; std::cin >> clave;

    for (auto i = d.begin(); i != d.end(); i++) {
        if (i->getClave() == clave) {
            d.erase(i);
            std::cout << "Departamento eliminado.\n";
            return;
        }
    }
    std::cout << "No existe departamento con dicha clave.\n";
}

void listarDepaPrecio(std::vector<Depa>& d) {
    double valor;
    std::cout << "Precio maximo: "; std::cin >> valor;
    for (auto& depa : d)
        if (depa.estaDisponible() && depa.getPrecio() <= valor)
            depa.mostrar();
}

void listarDepaExtension(std::vector<Depa>& d) {
    double exte;
    std::cout << "Extension minima: "; std::cin >> exte;
    for (auto& depa : d)
        if (depa.estaDisponible() &&
            depa.getExtension() >= exte &&
            depa.getUbicacion() == "Excelente")
            depa.mostrar();
}

void listarMontoDepaAlquilado(std::vector<Depa>& d) {
    double total = 0;
    for (auto& depa : d)
        if (!depa.estaDisponible()) {
            total += depa.getPrecio();
            depa.mostrar();
        }
    std::cout << "\nMonto total de rentas: $" << total << "\n";
}

void rentarDepa(std::vector<Depa>& d) {
    double extMin, precioMax;
    std::string ubiReq;

    std::cout << "\tRentar departamento\n";
    std::cout << "Extension minima requerida: ";
    std::cin >> extMin;

    std::cout << "Precio maximo que acepta: ";
    std::cin >> precioMax;

    std::cout << "Ubicacion requerida (Excelente/Buena/Regular/Mala): ";
    std::cin >> ubiReq;

    for (auto& depa : d) {
        if (depa.estaDisponible() &&
            depa.getExtension() >= extMin &&
            depa.getPrecio() <= precioMax &&
            depa.getUbicacion() == ubiReq) {

            depa.setDisponible(false);
            std::cout << "\nDepartamento RENTADO exitosamente:\n";
            depa.mostrar();
            return;
        }
    }

    std::cout << "\nNO hay departamentos disponibles con esos requisitos.\n";
}

void actualizarDepaVencido(std::vector<Depa>& d) {
    int clave;
    std::cout << "Clave del departamento a renovar: ";
    std::cin >> clave;

    for (auto& depa : d) {
        if (depa.getClave() == clave) {
            depa.setDisponible(true);
            std::cout << "Contrato actualizado. Ahora está disponible.\n";
            return;
        }
    }
    std::cout << "No se encontro departamento con esa clave.\n";
}

void actualizaMuestraRentaNoAlquilado(std::vector<Depa>& d) {
    double porcentaje;
    std::cout << "Porcentaje de aumento: ";
    std::cin >> porcentaje;

    for (auto& depa : d)
        if (depa.estaDisponible())
            depa.setPrecio(depa.getPrecio() * (1 + porcentaje / 100.0));

    std::cout << "\nPrecios actualizados:\n";
    for (auto& depa : d)
        if (depa.estaDisponible())
            depa.mostrar();
}

int main() {
    std::vector<Depa> departamento;
    int op;
    std::string ubi[] = { "Excelente", "Buena", "Regular", "Mala" };

    do {
        std::cout << "\n\t--- MENU ---\n";
        std::cout << "1. Insertar nuevo departamento\n";
        std::cout << "2. Eliminar departamento\n";
        std::cout << "3. Listar por precio\n";
        std::cout << "4. Listar por extension y ubicacion\n";
        std::cout << "5. Monto total rentas\n";
        std::cout << "6. Rentar departamento\n";
        std::cout << "7. Actualizar contrato vencido\n";
        std::cout << "8. Subir rentas no alquiladas\n";
        std::cout << "0. Salir\n";
        std::cin >> op;

        system("cls");

        switch (op) {
        case 1: registrarDepa(departamento, ubi); break;
        case 2: eliminarRegistroDepa(departamento); break;
        case 3: listarDepaPrecio(departamento); break;
        case 4: listarDepaExtension(departamento); break;
        case 5: listarMontoDepaAlquilado(departamento); break;
        case 6: rentarDepa(departamento); break;
        case 7: actualizarDepaVencido(departamento); break;
        case 8: actualizaMuestraRentaNoAlquilado(departamento); break;
        case 0: return 0;
        default: std::cout << "Opcion incorrecta.\n";
        }

        system("pause");
        system("cls");
    } while (true);
}
