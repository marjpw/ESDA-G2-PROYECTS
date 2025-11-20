#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <sstream>
#include <cmath> 
#include <stdexcept>
#include <iomanip> 

// --- Clase para representar cada Nodo del Árbol ---
class Nodo {
public:
    std::string dato;
    Nodo* izquierda;
    Nodo* derecha;

    // Constructor para un nodo hoja (operando)
    Nodo(std::string valor) : dato(valor), izquierda(NULL), derecha(NULL) {}

    // Constructor para un nodo interno (operador)
    Nodo(std::string valor, Nodo* izq, Nodo* der) : dato(valor), izquierda(izq), derecha(der) {}
};

// --- Clase para manejar el Árbol de Expresión ---
class ArbolExpresion {
private:
    Nodo* raiz;

    // Función recursiva para evaluar el árbol
    double evaluarRecursivo(Nodo* nodo) {
        if (nodo == NULL) {
            return 0; // No debería pasar en una expresión válida
        }

        // Si es una hoja, es un operando. Convertimos a double y retornamos.
        if (nodo->izquierda == NULL && nodo->derecha == NULL) {
            return std::stod(nodo->dato);
        }

        // Si no es hoja, es un operador. Evaluamos hijos y aplicamos el operador.
        double valorIzquierdo = evaluarRecursivo(nodo->izquierda);
        double valorDerecho = evaluarRecursivo(nodo->derecha);

        if (nodo->dato == "+") return valorIzquierdo + valorDerecho;
        if (nodo->dato == "-") return valorIzquierdo - valorDerecho;
        if (nodo->dato == "*") return valorIzquierdo * valorDerecho;
        if (nodo->dato == "/") {
            if (valorDerecho == 0) throw std::runtime_error("Error: Division por cero.");
            return valorIzquierdo / valorDerecho;
        }
        if (nodo->dato == "^") return pow(valorIzquierdo, valorDerecho);

        throw std::runtime_error("Operador no valido: " + nodo->dato);
    }

    // Función recursiva para imprimir el árbol (para visualización)
    void imprimirRecursivo(Nodo* nodo, int espacio) {
        if (nodo == NULL) return;
        espacio += 5;

        imprimirRecursivo(nodo->derecha, espacio);
        std::cout << std::endl;
        for (int i = 5; i < espacio; i++) std::cout << " ";
        std::cout << nodo->dato << "\n";
        imprimirRecursivo(nodo->izquierda, espacio);
    }

    // Función para liberar memoria del árbol
    void eliminarRecursivo(Nodo* nodo) {
        if (nodo == NULL) return;
        eliminarRecursivo(nodo->izquierda);
        eliminarRecursivo(nodo->derecha);
        delete nodo;
    }

public:
    ArbolExpresion() : raiz(NULL) {}
    ~ArbolExpresion() {
        eliminarRecursivo(raiz);
    }

    // (b) Construir el árbol desde una expresión postfija
    void construirDesdePostfija(const std::vector<std::string>& postfija) {
        std::stack<Nodo*> pilaNodos;

        for (const std::string& token : postfija) {
            // Si es un operando (número)
            if (std::isdigit(token[0]) || (token.length() > 1 && token[0] == '-')) { // Maneja números negativos y multí-dígito
                pilaNodos.push(new Nodo(token));
            }
            // Si es un operador
            else {
                if (pilaNodos.size() < 2) {
                    throw std::runtime_error("Expresion postfija invalida.");
                }
                Nodo* derecho = pilaNodos.top(); pilaNodos.pop();
                Nodo* izquierdo = pilaNodos.top(); pilaNodos.pop();
                pilaNodos.push(new Nodo(token, izquierdo, derecho));
            }
        }

        if (pilaNodos.size() != 1) {
            throw std::runtime_error("La expresión postfija no es valida para construir un arbol.");
        }

        raiz = pilaNodos.top();
    }

    // (c) Evaluar el árbol y mostrar el resultado
    double evaluar() {
        if (raiz == NULL) {
            throw std::runtime_error("El arbol está vacio.");
        }
        return evaluarRecursivo(raiz);
    }

    // Mostrar el árbol de expresión
    void imprimirArbol() {
        if (raiz == NULL) {
            std::cout << "El arbol esta vacio." << std::endl;
            return;
        }
        std::cout << "\t Arbol de Expresion (rotado 90 grados) " << std::endl;
        std::cout << "--------------------------------------" << std::endl;
        imprimirRecursivo(raiz, 0);
        std::cout << "--------------------------------------" << std::endl;
    }
};

// --- Clase para convertir de Infija a Postfija ---
class ConvertidorExpresiones {
private:
    int getPrecedencia(const std::string& op) {
        if (op == "^") return 3;
        if (op == "*" || op == "/") return 2;
        if (op == "+" || op == "-") return 1;
        return 0;
    }
    std::vector<std::string> tokenizar(const std::string& expresion) {
        std::vector<std::string> tokens;
        std::string tokenActual;
        for (char c : expresion) {
            if (isspace(c)) continue; // Ignorar espacios

            if (std::isdigit(c) || c == '.') {
                tokenActual += c;
            }
            else {
                if (!tokenActual.empty()) {
                    tokens.push_back(tokenActual);
                    tokenActual.clear();
                }
                if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '(' || c == ')') {
                    tokens.push_back(std::string(1, c));
                }
            }
        }
        if (!tokenActual.empty()) {
            tokens.push_back(tokenActual);
        }
        return tokens;
    }

public:
    std::vector<std::string> infijaAPostfija(const std::string& expresionInfija) {
        std::vector<std::string> tokens = tokenizar(expresionInfija);
        std::vector<std::string> salida;
        std::stack<std::string> pilaOperadores;

        for (const std::string& token : tokens) {
            if (std::isdigit(token[0])) {
                salida.push_back(token);
            }
            else if (token == "(") {
                pilaOperadores.push(token);
            }
            else if (token == ")") {
                while (!pilaOperadores.empty() && pilaOperadores.top() != "(") {
                    salida.push_back(pilaOperadores.top());
                    pilaOperadores.pop();
                }
                pilaOperadores.pop();
            }
            else {
                while (!pilaOperadores.empty() && pilaOperadores.top() != "(" && getPrecedencia(pilaOperadores.top()) > getPrecedencia(token)) {
                    salida.push_back(pilaOperadores.top());
                    pilaOperadores.pop();
                }
                pilaOperadores.push(token);
            }
        }

        while (!pilaOperadores.empty()) {
            salida.push_back(pilaOperadores.top());
            pilaOperadores.pop();
        }

        return salida;
    }
};

// --- Función principal para ejecutar el proyecto ---
int main() {
    std::string expresionInfija;
    std::cout << "UNMSM - FISI - Estructura de Datos" << std::endl;
    std::cout << "Sexto Proyecto" << std::endl;
    std::cout << "-----------------------------------" << std::endl;
    std::cout << "Ingrese una expresion aritmetica infija: ";
    std::getline(std::cin, expresionInfija);

    try {
        // --- a) Convertir de infija a postfija ---
        ConvertidorExpresiones convertidor;
        std::vector<std::string> expresionPostfija = convertidor.infijaAPostfija(expresionInfija);

        std::cout << "\n a) Conversion a Postfija" << std::endl;
        std::cout << "\tExpresion Infija: " << expresionInfija << std::endl;
        std::cout << "\tExpresion Postfija: ";
        for (size_t i = 0; i < expresionPostfija.size(); ++i) {
            std::cout << expresionPostfija[i];
            if (i < expresionPostfija.size() - 1) {
                std::cout << " ";
            }
        }
        std::cout << std::endl;

        // --- b) Construir el árbol de expresión ---
        ArbolExpresion arbol;
        arbol.construirDesdePostfija(expresionPostfija);

        std::cout << "\n b) Arbol de Expresion Construido " << std::endl;
        arbol.imprimirArbol();

        // --- c) Evaluar el árbol y mostrar el resultado ---
        std::cout << "\n c) Evaluacion del Arbol " << std::endl;
        double resultado = arbol.evaluar();
        std::cout << "\tResultado de la evaluacion: " << resultado << std::endl;

    }
    catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
