## Introducción al Problema

Este proyecto implementa un árbol general (n-ario) utilizando la representación Hijo más a la Izquierda – Hermano Derecho (HI–HD). Esta estructura permite manejar nodos con cualquier número de hijos empleando solo dos punteros por nodo, lo que simplifica su implementación y mantiene una gran flexibilidad.

<img width="1064" height="551" alt="image" src="https://github.com/user-attachments/assets/c6e4b0eb-2540-497b-b9fc-a281133533f7" />

## 1. Registro del Nodo
```plaintext
Registro Nodo
    valor        : Entero
    hijoIzq      : Nodo
    hermanoDer   : Nodo
FinRegistro
```
---

## 2. Inicializar Árbol General

**Entrada:** Ninguna 
**Salida:** Árbol inicializado con raíz vacía
```plaintext
Acción INIT_ARBOL()
Inicio
    RAIZ ← Nulo
Fin
```
---

## 3. Buscar un Nodo en el Árbol

**Entrada:** `Nodo n`, `Entero x` 
**Salida:** Referencia al nodo encontrado o Nulo

```plaintext
Función BUSCAR(n, x)
Inicio
    Si n = Nulo Entonces
        Retornar Nulo
    FinSi

    Si n.valor = x Entonces
        Retornar n
    FinSi

    encontrado ← BUSCAR(n.hijoIzq, x)
    Si encontrado ≠ Nulo Entonces
        Retornar encontrado
    FinSi

    Retornar BUSCAR(n.hermanoDer, x)
Fin

```
---

## 4. Insertar Nodo (HIJO IZQ – HERMANO DER)

**Entrada:** `Entero valorNuevo`, `Entero valorPadre`  
**Salida:** Árbol con el nodo insertado correctamente

```plaintext
Acción INSERTAR(valorNuevo, valorPadre)
Inicio
    nuevo ← Nuevo Nodo
    nuevo.valor ← valorNuevo
    nuevo.hijoIzq ← Nulo
    nuevo.hermanoDer ← Nulo

    Si valorPadre = 0 Entonces
        Si RAIZ = Nulo Entonces
            RAIZ ← nuevo
        SiNo
            Escribir "Error: ya existe raíz"
            Liberar(nuevo)
        FinSi
        Retornar
    FinSi

    Si RAIZ = Nulo Entonces
        Escribir "Debe insertar primero la raíz"
        Liberar(nuevo)
        Retornar
    FinSi

    padre ← BUSCAR(RAIZ, valorPadre)
    Si padre = Nulo Entonces
        Escribir "Padre no encontrado"
        Liberar(nuevo)
        Retornar
    FinSi

    Si padre.hijoIzq = Nulo Entonces
        padre.hijoIzq ← nuevo
    SiNo
        temp ← padre.hijoIzq
        Mientras temp.hermanoDer ≠ Nulo
            temp ← temp.hermanoDer
        FinMientras
        temp.hermanoDer ← nuevo
    FinSi
Fin

```
---

## 5. Recorrido Pre-Orden
```plaintext
Acción PREORDEN(n)
Inicio
    Si n ≠ Nulo Entonces

        Escribir n.valor

        h ← n.hijoIzq
        Mientras h ≠ Nulo
            PREORDEN(h)
            h ← h.hermanoDer
        FinMientras

    FinSi
Fin

```
---

## 6. Recorrido Inorden (n-ario Correcto)
```plaintext
Acción INORDEN(n)
Inicio
    Si n ≠ Nulo Entonces

        h ← n.hijoIzq
        INORDEN(h)

        Escribir n.valor

        Mientras h ≠ Nulo
            h ← h.hermanoDer
            INORDEN(h)
        FinMientras

    FinSi
Fin

    FinSi
Fin

```
---

## 7. Recorrido Postorden
```plaintext
Acción POSTORDEN(n)
Inicio
    Si n ≠ Nulo Entonces

        h ← n.hijoIzq
        Mientras h ≠ Nulo
            POSTORDEN(h)
            h ← h.hermanoDer
        FinMientras

        Escribir n.valor

    FinSi
Fin

```
---

## 8. Recorrido por Niveles (Anchura)
```plaintext
Acción RECORRIDO_NIVELES()
Inicio
    Si RAIZ = Nulo Entonces
        Escribir "Árbol vacío"
        Retornar
    FinSi

    Crear cola Q
    Encolar(Q, RAIZ)

    Mientras Q no esté vacía
        n ← Desencolar(Q)
        Escribir n.valor

        h ← n.hijoIzq
        Mientras h ≠ Nulo
            Encolar(Q, h)
            h ← h.hermanoDer
        FinMientras
    FinMientras
Fin

```
---

## 9. Mostrar Caminos desde la Raíz hasta las Hojas
```plaintext
Acción MOSTRAR_CAMINOS()
Inicio
    camino ← Lista vacía
    CAMINOS_RECURSIVO(RAIZ, camino)
Fin

```
```plaintext
Acción CAMINOS_RECURSIVO(n, camino)
Inicio
    Si n = Nulo Entonces
        Retornar
    FinSi

    Agregar(n.valor, camino)

    Si n.hijoIzq = Nulo Entonces
        Escribir camino
    SiNo
        h ← n.hijoIzq
        Mientras h ≠ Nulo
            CAMINOS_RECURSIVO(h, camino)
            h ← h.hermanoDer
        FinMientras
    FinSi

    EliminarÚltimo(camino)
Fin

```
---

## 10. Menú Principal del Proyecto
```plaintext
Acción MAIN()
Inicio
    INIT_ARBOL()

    Repetir
        Mostrar menú
        Leer opcion

        Segun opcion Hacer
            1: Leer valor y padre; INSERTAR(valor, padre)
            2: PREORDEN(RAIZ)
            3: INORDEN(RAIZ)
            4: POSTORDEN(RAIZ)
            5: RECORRIDO_NIVELES()
            6: MOSTRAR_CAMINOS()
        FinSegun
    Hasta opcion = 0
Fin

```
---
