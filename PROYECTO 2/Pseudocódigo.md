## Introducción al Problema
Este proyecto implementa un sistema de gestión de departamentos en renta utilizando un arreglo de registros ordenado por precio. Permite registrar, eliminar, listar y actualizar información de inmuebles, además de gestionar procesos de renta y renovación de contratos. Su finalidad es simular el funcionamiento básico de una inmobiliaria empleando estructura de datos secuenciales.


<img width="694" height="693" alt="image" src="https://github.com/user-attachments/assets/1b364873-80fc-45a1-b5b3-f85fa9128056" />

## Registro Departamento
```plaintext
Registro Departamento
    entero clave
    real extension
    carácter ubicacion(15)
    real precio
    booleano disponible
FinRegistro

Constante Max = 100
Departamento D(Max)
entero N

```
---

## 1. Verificar si existe la clave
**Entrada:** Arreglo D, número N, claveBus
**Salida:** verdadero si existe, falso en caso contrario
```plaintext
Predicado ExisteClave(D, N, claveBus)
Inicio
    encontro ← falso
    i ← 1

    Mientras (i ≤ N Y NO encontro)
        Si (D(i).clave = claveBus)
            encontro ← verdad
        FinSi
        i ← i + 1
    FinMientras

    Retornar encontro
Fin

```
---

## 2. Determinar posición de inserción (por precio)
**Entrada:** D, N, precioNuevo
**Salida:** posición donde insertar en orden ascendente
```plaintext
Función PosicionInsercion(D, N, precioNuevo): entero
Inicio
    i ← 1

    Mientras (i ≤ N Y D(i).precio < precioNuevo)
        i ← i + 1
    FinMientras

    Retornar i
Fin

```
---

## 3. Registrar Departamento (Inserción Ordenada)
**Entrada:** Datos del nuevo departamento
**Salida:** Arreglo D actualizado en orden ascendente por precio
```plaintext
Acción Insertar_Departamento(D, N)
Inicio
    Leer nuevo.clave
    Leer nuevo.extension
    Leer nuevo.ubicacion
    Leer nuevo.precio
    Leer nuevo.disponible

    Si (ExisteClave(D, N, nuevo.clave))
        Escribir "ERROR: La clave ya existe"
        Salir
    FinSi

    Si (N = Max)
        Escribir "ERROR: Arreglo lleno"
        Salir
    FinSi

    pos ← PosicionInsercion(D, N, nuevo.precio)

    Para i desde N hasta pos
        D(i+1) ← D(i)
    FinPara

    D(pos) ← nuevo
    N ← N + 1

    Escribir "Departamento registrado correctamente"
Fin

```
---

## 4. Eliminar Departamento por Clave
**Entrada:** D, N, clave
**Salida:** D y N actualizados
```plaintext
Acción Eliminar_Departamento(D, N)
Inicio
    Leer claveBus

    Si (NO ExisteClave(D, N, claveBus))
        Escribir "No existe departamento con esa clave"
        Salir
    FinSi

    i ← 1
    Mientras (i ≤ N Y D(i).clave ≠ claveBus)
        i ← i + 1
    FinMientras

    Para j desde i hasta N-1
        D(j) ← D(j+1)
    FinPara

    N ← N - 1
    Escribir "Departamento eliminado correctamente"
Fin

```
---

## 5. Listar Departamentos Disponibles con Precio ≤ X
**Entrada:** D, N, precioMax
**Salida:** Lista de departamentos
```plaintext
Acción Listar_Por_Precio(D, N)
Inicio
    Leer precioMax

    Para i desde 1 hasta N
        Si (D(i).disponible = verdad Y D(i).precio ≤ precioMax)
            Escribir D(i).clave, D(i).extension, D(i).ubicacion, D(i).precio
        FinSi
    FinPara
Fin

```
---

## 6. Listar Disponibles por Extensión ≥ X y Ubicación Excelente
```plaintext
Acción Listar_Por_Extension_Ubicacion(D, N)
Inicio
    Leer extMin

    Para i desde 1 hasta N
        Si (D(i).disponible = verdad Y
            D(i).extension ≥ extMin Y
            D(i).ubicacion = "Excelente")

            Escribir D(i).clave, D(i).precio, D(i).ubicacion, D(i).extension
        FinSi
    FinPara
Fin

```
---

## 7. Calcular Monto Total de Departamentos Alquilados
```plaintext
Acción Monto_Total_Alquilados(D, N)
Inicio
    total ← 0

    Para i desde 1 hasta N
        Si (D(i).disponible = falso)
            total ← total + D(i).precio
            Escribir D(i).clave, D(i).precio
        FinSi
    FinPara

    Escribir "Monto total = ", total
Fin

```
---

## 8. Rentar Departamento según requisitos
**Entrada:** extensión mínima, precio máximo, ubicación requerida
```plaintext
Acción Rentar_Departamento(D, N)
Inicio
    Leer extMin
    Leer precioMax
    Leer ubicReq

    rentado ← falso
    i ← 1

    Mientras (i ≤ N Y NO rentado)
        Si (D(i).disponible = verdad Y
            D(i).extension ≥ extMin Y
            D(i).precio ≤ precioMax Y
            D(i).ubicacion = ubicReq)

            D(i).disponible ← falso
            Escribir "Departamento rentado:"
            Escribir D(i).clave, D(i).precio
            rentado ← verdad
        FinSi

        i ← i + 1
    FinMientras

    Si (NO rentado)
        Escribir "No hay departamentos disponibles con esos requisitos"
    FinSi
Fin

```
---

## 9. Actualizar contrato vencido (volviendo a disponible)
```plaintext
Acción Actualizar_Contrato_Vencido(D, N)
Inicio
    Leer claveBus

    Si (NO ExisteClave(D, N, claveBus))
        Escribir "Clave no encontrada"
        Salir
    FinSi

    i ← 1
    Mientras (i ≤ N Y D(i).clave ≠ claveBus)
        i ← i + 1
    FinMientras

    D(i).disponible ← verdad

    Escribir "Contrato actualizado: ahora disponible"
Fin

```
---

## 10. Aumentar precio a departamentos NO alquilados
```plaintext
Acción Actualizar_Precios_No_Alquilados(D, N)
Inicio
    Leer porc

    Para i desde 1 hasta N
        Si (D(i).disponible = verdad)
            D(i).precio ← D(i).precio * (1 + porc / 100)
        FinSi
    FinPara

    Escribir "Precios actualizados"

    Para i desde 1 hasta N
        Si (D(i).disponible = verdad)
            Escribir D(i).clave, D(i).precio
        FinSi
    FinPara
Fin

```
---

## Menú Principal
```plaintext
Acción Menu(D, N)
Inicio
    Repetir
        Escribir "1. Insertar departamento"
        Escribir "2. Eliminar departamento"
        Escribir "3. Listar por precio"
        Escribir "4. Listar por extensión y ubicación"
        Escribir "5. Monto total de rentas"
        Escribir "6. Rentar departamento"
        Escribir "7. Actualizar contrato vencido"
        Escribir "8. Actualizar precios"
        Escribir "0. Salir"

        Leer op

        Segun op hacer
            1: Insertar_Departamento(D, N)
            2: Eliminar_Departamento(D, N)
            3: Listar_Por_Precio(D, N)
            4: Listar_Por_Extension_Ubicacion(D, N)
            5: Monto_Total_Alquilados(D, N)
            6: Rentar_Departamento(D, N)
            7: Actualizar_Contrato_Vencido(D, N)
            8: Actualizar_Precios_No_Alquilados(D, N)
        FinSegun

    Hasta (op = 0)
Fin


```
---
