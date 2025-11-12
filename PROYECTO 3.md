## Introducción al Problema

Este proyecto tiene como objetivo implementar y manejar una lista enlazada circular para el almacenamiento y la manipulación de datos relacionados con personas. Las operaciones principales incluyen la inserción, eliminación y visualización de personas dentro de un "ingreso", organizado por rangos de datos.

<img width="723" height="699" alt="Captura de pantalla 2025-11-12 085305" src="https://github.com/user-attachments/assets/710ec93b-f848-4d02-8c23-5f8056f3998c" />


A continuación, se presenta el pseudocódigo para diversas operaciones de la lista enlazada circular, que permiten realizar estas acciones de manera eficiente y organizada.

## 1. Insertar Persona a la Lista Circular por Ingreso

**Entrada:** Persona `p`, `NodoIngreso*` ingreso  
**Salida:** NodoIngreso actualizado con la persona añadida

```plaintext
Acción InsertarPersona (p, ingreso)
Inicio
    Si (ingreso → ultimo = Null) Entonces
        ingreso → ultimo ← p
        p → sig ← p
    Sino
        nodo ← ingreso → ultimo → sig
        Mientras (nodo → sig ≠ ingreso → ultimo) Hacer
            Si (nodo → valor > p → valor) Entonces
                Nodo.insertar(nodo, p)
            FinSi
            nodo ← nodo → sig
        FinMientras
        ingreso → ultimo ← nodo
    FinSi
Fin
```
---
## 2. Eliminar Persona por Nombre en Lista Circular

**Entrada:** `string nombre`, `NodoIngreso*` ingreso  
**Salida:** Persona eliminada o mensaje de no encontrado

```plaintext
Acción EliminarPorNombre (nombre, ingreso)
Inicio
    Si (ingreso → ultimo = Null) Entonces
        Escribir "Ingreso vacío"
    Sino
        nodo ← ingreso → ultimo → sig
        anterior ← ingreso → ultimo
        Mientras (nodo ≠ ingreso → ultimo) Hacer
            Si (nodo → valor → apellidosNombres = nombre) Entonces
                anterior → sig ← nodo → sig
                Eliminar nodo
                Escribir "Persona eliminada"
                Fin
            FinSi
            anterior ← nodo
            nodo ← nodo → sig
        FinMientras
        Si (nodo → valor → apellidosNombres = nombre) Entonces
            ingreso → ultimo ← anterior
            anterior → sig ← nodo → sig
            Eliminar nodo
            Escribir "Persona eliminada"
        Sino
            Escribir "Persona no encontrada"
        FinSi
    FinSi
Fin
```
---
## 3. Mostrar Personas en Ingreso por Sexo

**Entrada:** `NodoIngreso*` ingreso, `char sexo`  
**Salida:** Lista de personas del sexo especificado

```plaintext
Acción MostrarPorSexo (ingreso, sexo)
Inicio
    Si (ingreso → ultimo = Null) Entonces
        Escribir "Ingreso vacío"
    Sino
        nodo ← ingreso → ultimo → sig
        Mientras (nodo ≠ ingreso → ultimo) Hacer
            Si (nodo → valor → sexo = sexo) Entonces
                Escribir nodo → valor → apellidosNombres
                Escribir nodo → valor → edad
            FinSi
            nodo ← nodo → sig
        FinMientras
        Si (nodo → valor → sexo = sexo) Entonces
            Escribir nodo → valor → apellidosNombres
            Escribir nodo → valor → edad
        FinSi
    FinSi
Fin
```
---
## 4. Intercalar Listas L1 y L2 en L3 (Orden Ascendente)

**Entrada:** `NodoIngreso*` cab1, `NodoIngreso*` cab2  
**Salida:** `NodoIngreso*` cab3 (lista intercalada)

```plaintext
Acción IntercalarListas (cab1, cab2, cab3)
Inicio
    n1 ← cab1
    n2 ← cab2
    cab3 ← Null
    Mientras (n1 ≠ Null ∧ n2 ≠ Null) Hacer
        Si (n1 → valor < n2 → valor) Entonces
            Adiciona (cab3, n1 → valor)
            n1 ← n1 → sig
        Sino
            Adiciona (cab3, n2 → valor)
            n2 ← n2 → sig
    FinMientras
    Mientras (n1 ≠ Null) Hacer
        Adiciona (cab3, n1 → valor)
        n1 ← n1 → sig
    FinMientras
    Mientras (n2 ≠ Null) Hacer
        Adiciona (cab3, n2 → valor)
        n2 ← n2 → sig
    FinMientras
Fin
