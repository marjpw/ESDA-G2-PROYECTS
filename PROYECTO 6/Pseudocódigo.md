## Introducción al Problema

Este proyecto tiene como objetivo implementar el control de inventarios y órdenes de venta de una tienda de repuestos utilizando exclusivamente estructuras de datos lineales: Pilas (para el inventario bajo el sistema UEPS: último en entrar, primero en salir) y Colas (para el registro y atención de órdenes de venta bajo la política FIFO: primero en entrar, primero en salir).

<img width="1135" height="564" alt="image" src="https://github.com/user-attachments/assets/aefecf20-e340-43d7-b7f6-98d95ce0eca7" />

## 1. Convertidor de expresiones aritméticas: infija a postfija

```plaintext
        Función infijaAPostfija(Cadena expresionInfija) : Vector<Cadena>
            Inicio
                Vector<Cadena> tokens ← tokenizar(expresionInfija)
                Vector<Cadena> salida
                Pila<Cadena> pilaOperadores
                
                Para cada token en tokens Hacer
                    Si (EsDigito(token[0])) Entonces
                        salida.agregar(token)
                    Sino Si (token = "(") Entonces
                        pilaOperadores.apilar(token)
                    Sino Si (token = ")") Entonces
                        Mientras (NO pilaOperadores.estaVacia() Y pilaOperadores.cima() ≠ "(") Hacer
                            salida.agregar(pilaOperadores.cima())
                            pilaOperadores.desapilar()
                        FinMientras
                        pilaOperadores.desapilar() // Eliminar el paréntesis izquierdo
                    Sino // Es un operador
                        Mientras (NO pilaOperadores.estaVacia() Y pilaOperadores.cima() ≠ "(" Y 
                               getPrecedencia(pilaOperadores.cima()) > getPrecedencia(token)) Hacer
                            salida.agregar(pilaOperadores.cima())
                            pilaOperadores.desapilar()
                        FinMientras
                        pilaOperadores.apilar(token)
                    FinSi
                FinPara
                
                Mientras (NO pilaOperadores.estaVacia()) Hacer
                    salida.agregar(pilaOperadores.cima())
                    pilaOperadores.desapilar()
                FinMientras
                
                Retornar salida
            Fin
        FinFunción

```
---

## 2. Construcción del árbol a partir de expresión aritmética postfija

```plaintext
        Función construirDesdePostfija(Vector<Cadena> postfija)
            Inicio
                Pila<Nodo> pilaNodos
                
                Para cada token en postfija Hacer
                    // Si es un operando (número)
                    Si (EsDigito(token[0]) O (Longitud(token) > 1 Y token[0] = '-')) Entonces
                        pilaNodos.apilar(CrearNodo(token))
                    Sino // Es un operador
                        Si (pilaNodos.tamaño() < 2) Entonces
                            Lanzar Error "Expresión postfija inválida"
                        FinSi
                        
                        derecho ← pilaNodos.desapilar()
                        izquierdo ← pilaNodos.desapilar()
                        pilaNodos.apilar(CrearNodo(token, izquierdo, derecho))
                    FinSi
                FinPara
                
                Si (pilaNodos.tamaño() ≠ 1) Entonces
                    Lanzar Error "La expresión postfija no es válida para construir un árbol"
                FinSi
                
                raiz ← pilaNodos.cima()
            Fin
        FinFunción
```
---

## 3. Mostrar el árbol construido

```plaintext
      Función evaluarRecursivo(Nodo nodo) : Real
            Inicio
                Si (nodo = NULO) Entonces
                    Retornar 0
                FinSi
                
                // Si es una hoja, es un operando
                Si (nodo.izquierda = NULO Y nodo.derecha = NULO) Entonces
                    Retornar ConvertirAReal(nodo.dato)
                FinSi
                
                // Si no es hoja, es un operador
                valorIzquierdo ← evaluarRecursivo(nodo.izquierda)
                valorDerecho ← evaluarRecursivo(nodo.derecha)
                
                Si (nodo.dato = "+") Entonces
                    Retornar valorIzquierdo + valorDerecho
                Sino Si (nodo.dato = "-") Entonces
                    Retornar valorIzquierdo - valorDerecho
                Sino Si (nodo.dato = "*") Entonces
                    Retornar valorIzquierdo * valorDerecho
                Sino Si (nodo.dato = "/") Entonces
                    Si (valorDerecho = 0) Entonces
                        Lanzar Error "División por cero"
                    FinSi
                    Retornar valorIzquierdo / valorDerecho
                Sino Si (nodo.dato = "^") Entonces
                    Retornar Potencia(valorIzquierdo, valorDerecho)
                FinSi
                
                Lanzar Error "Operador no válido: " + nodo.dato
            Fin
        FinFunción
        Función imprimirRecursivo(Nodo nodo, Entero espacio)
            Inicio
                Si (nodo = NULO) Entonces
                    Retornar
                FinSi
                
                espacio ← espacio + 5
                
                imprimirRecursivo(nodo.derecha, espacio)
                Escribir ""
                Para i ← 5 Hasta espacio-1 Hacer
                    Escribir " "
                FinPara
                Escribir nodo.dato
                imprimirRecursivo(nodo.izquierda, espacio)
            Fin
        FinFunción
```
---

## 4. Evaluar el arbol

```plaintext
        Función evaluarRecursivo(Nodo nodo) : Real
            Inicio
                Si (nodo = NULO) Entonces
                    Retornar 0
                FinSi
                
                // Si es una hoja, es un operando
                Si (nodo.izquierda = NULO Y nodo.derecha = NULO) Entonces
                    Retornar ConvertirAReal(nodo.dato)
                FinSi
                
                // Si no es hoja, es un operador
                valorIzquierdo ← evaluarRecursivo(nodo.izquierda)
                valorDerecho ← evaluarRecursivo(nodo.derecha)
                
                Si (nodo.dato = "+") Entonces
                    Retornar valorIzquierdo + valorDerecho
                Sino Si (nodo.dato = "-") Entonces
                    Retornar valorIzquierdo - valorDerecho
                Sino Si (nodo.dato = "*") Entonces
                    Retornar valorIzquierdo * valorDerecho
                Sino Si (nodo.dato = "/") Entonces
                    Si (valorDerecho = 0) Entonces
                        Lanzar Error "División por cero"
                    FinSi
                    Retornar valorIzquierdo / valorDerecho
                Sino Si (nodo.dato = "^") Entonces
                    Retornar Potencia(valorIzquierdo, valorDerecho)
                FinSi
                
                Lanzar Error "Operador no válido: " + nodo.dato
            Fin
        FinFunción

        Función evaluar() : Real
            Inicio
                Si (raiz = NULO) Entonces
                    Lanzar Error "El árbol está vacío"
                FinSi
                
                Retornar evaluarRecursivo(raiz)
            Fin
        FinFunción
```
---
