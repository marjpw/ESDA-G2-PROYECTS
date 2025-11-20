## Introducción al Problema

Este programa implementa un sistema de gestión de instructores y empleados, donde:
Se lee un archivo/listado de instructores con:
- Código
- Nombre
- Notas de tres exámenes
  
Se lee un archivo/listado de empleados con:
- Código
- Nombre
- Cursos aprobados anteriormente

El programa:
- Calcula la nota final de cada instructor.
- Cruza información por código.
- Construye varios reportes automáticos:

Reportes generados
(a) Reporte general con cursos aprobados acumulados.
(b) Lista de desaprobados (nota final < 11).
(c) Instructores sin registro en empleados.
(d) Tabla completa con promedios finales generales.

## Definición de Registros
Registro Instructor
Registro Instructor
    entero codigo
    cadena nombre
    real n1       // nota examen 1
    real n2       // nota examen 2
    real n3       // nota examen 3
    real nf       // nota final calculada
FinRegistro

Registro Empleado
Registro Empleado
    entero codigo
    cadena nombre
    entero cursos_antes   // cursos aprobados antes del actual
FinRegistro

## Funciones Auxiliares
Función Trim (eliminar espacios al inicio y fin)
Función Trim(cadena s) : cadena
Inicio
    Mientras (s inicia con espacio)
        eliminar primer carácter
    FinMientras

    Mientras (s termina con espacio)
        eliminar último carácter
    FinMientras

    Retornar s
Fin

Función Split_Semicolon
Función Split_Semicolon(cadena line) : arreglo de cadena
Inicio
    partes ← []
    actual ← ""

    Para cada carácter c en line
        Si (c = ';')
            agregar Trim(actual) a partes
            actual ← ""
        SiNo
            actual ← actual + c
        FinSi
    FinPara

    agregar Trim(actual) a partes

    Retornar partes
Fin

## 1. Lectura de Instructores
Acción Leer_Instructores(Referencia Instrs, Referencia N)
Inicio
    Leer N
    Si falla
        Escribir "Error: no se pudo leer N"
        Terminar
    FinSi

    Leer línea vacía

    Para i desde 1 hasta N
        Leer line

        Si (line está vacía)
            i ← i - 1
            Continuar
        FinSi

        partes ← Split_Semicolon(line)

        Si (longitud(partes) ≠ 5)
            Escribir "Error en instructor: ", line
            Terminar
        FinSi

        Crear r : Instructor
        r.codigo ← entero(partes[1])
        r.nombre ← partes[2]
        r.n1 ← real(partes[3])
        r.n2 ← real(partes[4])
        r.n3 ← real(partes[5])

        r.nf ← (r.n1 + r.n2 + r.n3) / 3

        agregar r a Instrs
    FinPara
Fin

## 2. Lectura de Empleados
Acción Leer_Empleados(Referencia Emps, Referencia M)
Inicio
    Leer M
    Si falla
        Escribir "Error: no se pudo leer M"
        Terminar
    FinSi

    Leer línea vacía

    Para i desde 1 hasta M
        Leer line

        Si (line está vacía)
            i ← i - 1
            Continuar
        FinSi

        partes ← Split_Semicolon(line)

        Si (longitud(partes) ≠ 3)
            Escribir "Error en empleado: ", line
            Terminar
        FinSi

        Crear e : Empleado
        e.codigo ← entero(partes[1])
        e.nombre ← partes[2]
        e.cursos_antes ← entero(partes[3])

        agregar e a Emps
    FinPara
Fin

## 3. Construcción del Índice de Empleados por Código
Acción Construir_Indice_Empleados(Emps, Referencia EmpByCode)
Inicio
    EmpByCode ← diccionario vacío

    Para cada e en Emps
        EmpByCode[e.codigo] ← e
    FinPara
Fin

## 4. Comparador por Nombre
Definir Comparador_Por_Nombre(a, b)
    Retornar (a.nombre < b.nombre)
Fin

## 5. Reporte (a): Reporte General
Descripción

Cruza instructores y empleados por código.
Calcula:

Nota final.

Cursos totales = cursos_antes + 1 si aprobó (nf ≥ 11).

Luego ordena por nombre.

Pseudocódigo
Acción Reporte_A(Instrs, EmpByCode)
Inicio
    RepA ← []

    Para cada r en Instrs
        Si existe EmpByCode[r.codigo]
            e ← EmpByCode[r.codigo]

            Si (r.nf ≥ 11)
                total ← e.cursos_antes + 1
            SiNo
                total ← e.cursos_antes
            FinSi

            Crear fila
            fila.codigo ← r.codigo
            fila.nombre ← r.nombre
            fila.nf ← r.nf
            fila.cursos_tot ← total

            agregar fila a RepA
        FinSi
    FinPara

    Ordenar RepA por nombre

    Imprimir tabla
Fin

## 6. Reporte (b): Desaprobados
Acción Reporte_B(Instrs)
Inicio
    RepB ← []

    Para cada r en Instrs
        Si (r.nf < 11)
            agregar r a RepB
        FinSi
    FinPara

    Ordenar RepB por nombre

    Imprimir tabla de desaprobados
Fin

## 7. Reporte (c): Instructores NO registrados como empleados
Acción Reporte_C(Instrs, EmpByCode)
Inicio
    RepC ← []

    Para cada r en Instrs
        Si NO existe EmpByCode[r.codigo]
            agregar r a RepC
        FinSi
    FinPara

    Ordenar RepC por nombre
    Imprimir tabla
Fin

## 8. Reporte (d): Tabla completa + Promedios
Acción Reporte_D(Instrs)
Inicio
    RepD ← copia de Instrs
    Ordenar RepD por nombre

    sum1 ← sum2 ← sum3 ← sumf ← 0

    Para cada r en RepD
        sum1 ← sum1 + r.n1
        sum2 ← sum2 + r.n2
        sum3 ← sum3 + r.n3
        sumf ← sumf + r.nf
    FinPara

    C ← tamaño de RepD

    Imprimir tabla completa

    Si (C > 0)
        Imprimir promedios de (sum1/C), (sum2/C), (sum3/C), (sumf/C)
    FinSi
Fin

## 9. Programa Principal
Acción Principal
Inicio
    Leer_Instructores(Instrs, N)
    Leer_Empleados(Emps, M)

    Construir_Indice_Empleados(Emps, EmpByCode)

    Reporte_A(Instrs, EmpByCode)
    Reporte_B(Instrs)
    Reporte_C(Instrs, EmpByCode)
    Reporte_D(Instrs)
Fin
