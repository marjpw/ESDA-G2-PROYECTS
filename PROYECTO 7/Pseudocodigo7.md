## Introducción al Problema

El proyecto busca gestionar ciudades (vértices) y vuelos entre ellas (aristas), permitiendo insertarlos, eliminarlos y mostrarlos, simulando la estructura de una red de rutas aéreas.

<img width="546" height="737" alt="{23A4F3BD-0CBE-46C4-BE3B-CADF4F129B23}" src="https://github.com/user-attachments/assets/1f2db105-d1ee-498e-b7a0-080b32014e83" />

## 1. DEFINICIÓN DE ESTRUCTURAS

Registro Arista

```plaintext
Registro Arista
    Entero ValorArista
    Apuntador a Vertice Destino
    Apuntador a Arista SigArista
FinRegistro
```

---

Registro Vertice

```plaintext
Registro Vertice
    Cadena ValorVertice
    Apuntador a Vertice SigVertice
    Apuntador a Arista Ady
FinRegistro
```

---

Estructura GrafoVuelos

```plaintext
Registro GrafoVuelos
    Apuntador a Vertice Principio
FinRegistro
```

---

## 2. OPERACIONES PRINCIPALES DEL GRAFO

Acción Insertar_Vertice(G, nombreCiudad)

```plaintext
Acción Insertar_Vertice(G, nombreCiudad)
Inicio
    aux ← G.Principio

    // Verificar si ya existe
    Mientras (aux ≠ Nulo)
        Si (aux.ValorVertice = nombreCiudad)
            Escribir "La ciudad ya existe."
            FinAcción
        FinSi
        aux ← aux.SigVertice
    FinMientras

    // Crear nuevo vértice
    nuevo ← Nuevo Vertice
    nuevo.ValorVertice ← nombreCiudad
    nuevo.SigVertice ← Nulo
    nuevo.Ady ← Nulo

    // Caso 1: insertar al inicio
    Si (G.Principio = Nulo O nombreCiudad < G.Principio.ValorVertice)
        nuevo.SigVertice ← G.Principio
        G.Principio ← nuevo
        Escribir "Ciudad insertada correctamente."
        FinAcción
    FinSi

    // Caso 2: insertar en posición ordenada
    anterior ← G.Principio
    actual ← G.Principio.SigVertice

    Mientras (actual ≠ Nulo Y nombreCiudad > actual.ValorVertice)
        anterior ← actual
        actual ← actual.SigVertice
    FinMientras

    nuevo.SigVertice ← actual
    anterior.SigVertice ← nuevo

    Escribir "Ciudad insertada correctamente."
Fin
```

---

Acción Insertar_Vuelo(G, origen, destino, numeroVuelo)

```plaintext
Acción Insertar_Vuelo(G, origen, destino, numeroVuelo)
Inicio
    vertOrigen ← Buscar_Vertice(G, origen)
    Si (vertOrigen = Nulo)
        Escribir "Ciudad origen no existe."
        FinAcción
    FinSi

    vertDestino ← Buscar_Vertice(G, destino)
    Si (vertDestino = Nulo)
        Escribir "Ciudad destino no existe."
        FinAcción
    FinSi

    Si (Existe_Numero_Vuelo(G, numeroVuelo))
        Escribir "El número de vuelo ya existe."
        FinAcción
    FinSi

    nueva ← Nuevo Arista
    nueva.ValorArista ← numeroVuelo
    nueva.Destino ← vertDestino

    nueva.SigArista ← vertOrigen.Ady
    vertOrigen.Ady ← nueva

    Escribir "Vuelo insertado correctamente."
Fin

```

---

Acción Eliminar_Vuelo(G, numeroVuelo)

```plaintext
Acción Eliminar_Vuelo(G, numeroVuelo)
Inicio
    vert ← G.Principio

    Mientras (vert ≠ Nulo)
        anterior ← Nulo
        actual ← vert.Ady

        Mientras (actual ≠ Nulo)
            Si (actual.ValorArista = numeroVuelo)
                Si (anterior = Nulo)
                    vert.Ady ← actual.SigArista
                Sino
                    anterior.SigArista ← actual.SigArista
                FinSi
                Liberar(actual)
                Escribir "Vuelo eliminado correctamente."
                FinAcción
            FinSi

            anterior ← actual
            actual ← actual.SigArista
        FinMientras

        vert ← vert.SigVertice
    FinMientras

    Escribir "El vuelo no existe."
Fin
```

---

Acción Mostrar_Vuelos(G)

```plaintext
Acción Mostrar_Vuelos(G)
Inicio
    Si (G.Principio = Nulo)
        Escribir "El grafo está vacío."
        FinAcción
    FinSi

    Escribir "=== LISTADO DE VUELOS ==="

    vert ← G.Principio
    Mientras (vert ≠ Nulo)
        ar ← vert.Ady
        Mientras (ar ≠ Nulo)
            Escribir ar.ValorArista, " | ", vert.ValorVertice,
                     " -> ", ar.Destino.ValorVertice
            ar ← ar.SigArista
        FinMientras
        vert ← vert.SigVertice
    FinMientras
Fin
```

---

Acción Mostrar_Grafo(G)

```plaintext
Acción Mostrar_Grafo(G)
Inicio
    Si (G.Principio = Nulo)
        Escribir "El grafo está vacío."
        FinAcción
    FinSi

    vert ← G.Principio

    Mientras (vert ≠ Nulo)
        Escribir vert.ValorVertice, " -> "

        ar ← vert.Ady
        Si (ar = Nulo)
            Escribir "NULL"
        FinSi

        Mientras (ar ≠ Nulo)
            Escribir "[Vuelo:", ar.ValorArista,
                     " -> ", ar.Destino.ValorVertice, "]"
            Si (ar.SigArista ≠ Nulo)
                Escribir " -> "
            FinSi
            ar ← ar.SigArista
        FinMientras

        Escribir salto
        vert ← vert.SigVertice
    FinMientras
Fin
```

## 3. FUNCIONES AUXILIARES

Función Buscar_Vertice(G, nombre): Apuntador a Vertice

```plaintext
Función Buscar_Vertice(G, nombre): Vertice
Inicio
    aux ← G.Principio
    Mientras (aux ≠ Nulo)
        Si (aux.ValorVertice = nombre)
            Retornar(aux)
        FinSi
        aux ← aux.SigVertice
    FinMientras
    Retornar(Nulo)
Fin
```

---

Función Existe_Numero_Vuelo(G, numero): Booleano

```plaintext
Función Existe_Numero_Vuelo(G, numero): Booleano
Inicio
    vert ← G.Principio
    Mientras (vert ≠ Nulo)
        ar ← vert.Ady
        Mientras (ar ≠ Nulo)
            Si (ar.ValorArista = numero)
                Retornar(Verdadero)
            FinSi
            ar ← ar.SigArista
        FinMientras
        vert ← vert.SigVertice
    FinMientras
    Retornar(Falso)
Fin
```

---

Acción Eliminar_Aristas_Hacia(G, verticeDestino)

```plaintext
Acción Eliminar_Aristas_Hacia(G, verticeDestino)
Inicio
    vert ← G.Principio

    Mientras (vert ≠ Nulo)
        anterior ← Nulo
        actual ← vert.Ady

        Mientras (actual ≠ Nulo)
            Si (actual.Destino = verticeDestino)
                Si (anterior = Nulo)
                    vert.Ady ← actual.SigArista
                Sino
                    anterior.SigArista ← actual.SigArista
                FinSi
                temp ← actual
                actual ← actual.SigArista
                Liberar(temp)
            Sino
                anterior ← actual
                actual ← actual.SigArista
            FinSi
        FinMientras

        vert ← vert.SigVertice
    FinMientras
Fin
```

## 4. PROGRAMA PRINCIPAL

Programa Principal
Inicio
    Crear G como GrafoVuelos
    G.Principio ← Nulo

```plaintext
    Repetir
        Escribir menú principal
        Leer opcion

        Segun opcion Hacer
            Caso 1:
                Leer ciudad
                Insertar_Vertice(G, ciudad)

            Caso 2:
                Leer origen, destino, numeroVuelo
                Insertar_Vuelo(G, origen, destino, numeroVuelo)

            Caso 3:
                Leer ciudad
                Eliminar_Vertice(G, ciudad)

            Caso 4:
                Leer numeroVuelo
                Eliminar_Vuelo(G, numeroVuelo)

            Caso 5:
                Mostrar_Vuelos(G)

            Caso 6:
                Mostrar_Grafo(G)

            Caso 7:
                Cargar ejemplo del proyecto en G

            Caso 0:
                Escribir "Saliendo..."

            Otro caso:
                Escribir "Opción inválida."
        FinSegun

    Hasta (opcion = 0)
Fin
```
