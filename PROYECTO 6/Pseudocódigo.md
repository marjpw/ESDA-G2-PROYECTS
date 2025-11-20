## Introducción al Problema

Este proyecto tiene como objetivo implementar el control de inventarios y órdenes de venta de una tienda de repuestos utilizando exclusivamente estructuras de datos lineales: Pilas (para el inventario bajo el sistema UEPS: último en entrar, primero en salir) y Colas (para el registro y atención de órdenes de venta bajo la política FIFO: primero en entrar, primero en salir).

<img width="1135" height="564" alt="image" src="https://github.com/user-attachments/assets/aefecf20-e340-43d7-b7f6-98d95ce0eca7" />

## 1. Registro nodo:

```plaintext
Registro Nodo
    Cadena Dato
    Nodo *HI      // hijo izquierdo
    Nodo *HD      // hijo derecho
FinRegistro
```
---

## 2. Obtener precedencia

```plaintext
Función ObtenerPrecedencia(op): Entero
Inicio
    Si (op = "^") Retornar(3)
    Si (op = "*" ˅ op = "/") Retornar(2)
    Si (op = "+" ˅ op = "-") Retornar(1)
    Retornar(0)
Fin

```
---

## 3. Accion tokenizar

```plaintext
Acción Tokenizar(expresion, Tokens[])
Inicio
    TokenActual ← ""

    Para cada carácter c en expresion
        Si (c es espacio) Continuar

        Si (c es dígito ˅ c = '.')
            TokenActual ← TokenActual + c
        Sino
            Si (TokenActual ≠ "")
                Añadir TokenActual a Tokens
                TokenActual ← ""
            FinSi

            Si (c ∈ {+, -, *, /, ^, (, )})
                Añadir c como token a Tokens
            FinSi
        FinSi
    FinPara

    Si (TokenActual ≠ "")
        Añadir TokenActual a Tokens
    FinSi
Fin

```
---

## 4. Pasar de expresión infija a expresión postfija

```plaintext
Función Infija_A_Postfija(ExpInf): Arreglo de Cadenas
Inicio
    Tokens ← vacío
    Llamar Tokenizar(ExpInf, Tokens)

    Salida ← vacío
    PilaOper ← pila vacía

    Para cada Token en Tokens
        Si (Token inicia con dígito)
            Añadir Token a Salida

        SinoSi (Token = "(")
            Empilar(PilaOper, Token)

        SinoSi (Token = ")")
            Mientras (Tope(PilaOper) ≠ "(")
                Añadir Tope(PilaOper) a Salida
                Desapilar(PilaOper)
            FinMientras
            Desapilar(PilaOper) // eliminar "("

        Sino // es operador
            Mientras (PilaOper no vacía) Y
                   (Tope(PilaOper) ≠ "(") Y
                   (ObtenerPrecedencia(Tope(PilaOper)) > ObtenerPrecedencia(Token))
                Añadir Tope(PilaOper) a Salida
                Desapilar(PilaOper)
            FinMientras
            Empilar(PilaOper, Token)
        FinSi
    FinPara

    Mientras (PilaOper no vacía)
        Añadir Tope(PilaOper) a Salida
        Desapilar(PilaOper)
    FinMientras

    Retornar(Salida)
Fin
```
---

## 5. Construcción del árbol desde postfija

```plaintext
Acción Construir_Desde_Postfija(Postfija[], Raiz)
Inicio
    PilaN ← pila vacía

    Para cada Token en Postfija
        Si (Token es operando)
            p ← nuevo Nodo
            p.Dato ← Token
            p.HI ← Nulo
            p.HD ← Nulo
            Empilar(PilaN, p)

        Sino // Token es operador
            Desapilar(PilaN, p2)
            Desapilar(PilaN, p1)

            p ← nuevo Nodo
            p.Dato ← Token
            p.HI ← p1
            p.HD ← p2

            Empilar(PilaN, p)
        FinSi
    FinPara

    Raiz ← Tope(PilaN)
Fin
```
---

## 6. Evaluar recursividad del arbol

```plaintext
Función EvaluarRecursividad(N): Real
Inicio
    Si (N = Nulo)
        Retornar(0)
    FinSi

    Si (N.HI = Nulo Y N.HD = Nulo)
        Retornar(Convertir_A_Real(N.Dato))
    FinSi

    VI ← Evaluar_Rec(N.HI)
    VD ← Evaluar_Rec(N.HD)

    Segun N.Dato Hacer
        Caso "+" : Retornar(VI + VD)
        Caso "-" : Retornar(VI - VD)
        Caso "*" : Retornar(VI * VD)
        Caso "/" :
                Si (VD = 0) Error("División por cero")
                Retornar(VI / VD)
        Caso "^" : Retornar(VI ^ VD)
        Otro Caso:
                Error("Operador no válido")
    FinSegun
Fin

Función Evaluar_Árbol(Raiz): Real
Inicio
    Si (Raiz = Nulo)
        Error("Árbol vacío")
    FinSi

    Retornar( Evaluar_Rec(Raiz) )
Fin
```
---

## 7. Impresión del árbol

```plaintext
Acción Imprimir_Recursivo(N, Espacio)
Inicio
    Si (N = Nulo) FinAcción

    Espacio ← Espacio + 5

    Imprimir_Rec(N.HD, Espacio)

    Escribir salto_linea
    Para i desde 1 hasta Espacio
         Escribir " "
    FinPara
    Escribir N.Dato

    Imprimir_Rec(N.HI, Espacio)
Fin

Acción Imprimir_Árbol(Raiz)
Inicio
    Si (Raiz = Nulo)
        Escribir "Árbol vacío"
        FinAcción
    FinSi

    Escribir "Árbol de expresión (rotado 90 grados)"
    Escribir "---------------------------------------"

    Imprimir_Recursivo(Raiz, 0)

    Escribir "---------------------------------------"
Fin

```
---

## 8. Acción principal

```plaintext
Accion Principal
Inicio
    Escribir "UNMSM - FISI - Estructura de Datos"
    Escribir "Sexto Proyecto"
    Escribir "-----------------------------------"

    Escribir "Ingrese una expresión en notación infija: "
    Leer ExpInfija

    // a) Conversión a postfija
    Postfija ← Infija_A_Postfija(ExpInfija)

    Escribir "Expresión postfija:"
    Escribir Postfija

    // b) Construir árbol
    Declarar Raiz como Nodo
    Construir_Desde_Postfija(Postfija, Raiz)

    Escribir "Árbol de expresión:"
    Imprimir_Árbol(Raiz)

    // c) Evaluar
    Resultado ← Evaluar_Árbol(Raiz)
    Escribir "Resultado de la evaluación:", Resultado
Fin
```
---
