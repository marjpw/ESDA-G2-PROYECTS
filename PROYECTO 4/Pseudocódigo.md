## Introducción al Problema

Este proyecto tiene como objetivo implementar el control de inventarios y órdenes de venta de una tienda de repuestos utilizando exclusivamente estructuras de datos lineales: Pilas (para el inventario bajo el sistema UEPS: último en entrar, primero en salir) y Colas (para el registro y atención de órdenes de venta bajo la política FIFO: primero en entrar, primero en salir).

## 1. Registrar Producto en el Inventario (Pila UEPS)

**Entrada:** Datos del producto p, Pila de productos pilaProductos 
**Salida:** pilaProductos actualizada con el nuevo producto en el tope

```plaintext
Acción RegistrarProducto (p, pilaProductos)
Inicio
    nuevoNodo ← CrearNodo(p)

    // Operación básica de Pila: PUSH
    Si (pilaProductos.tope = Null) Entonces
        pilaProductos.tope ← nuevoNodo
    Sino
        nuevoNodo.sig ← pilaProductos.tope
        pilaProductos.tope ← nuevoNodo
    FinSi

    Escribir "Producto registrado correctamente"
Fin
```
---

## 2. Registrar Orden de Venta (Cola FIFO)

**Entrada:** Datos de la orden o, Cola de órdenes pendientes colaOrdenes
**Salida:** colaOrdenes actualizada con la orden encolada al final
```plaintext
Acción RegistrarOrdenVenta (o, colaOrdenes)
Inicio
    nuevoNodo ← CrearNodo(o)

    // Operación básica de Cola: ENCOLAR
    Si (colaOrdenes.frente = Null) Entonces
        colaOrdenes.frente ← nuevoNodo
        colaOrdenes.fin ← nuevoNodo
    Sino
        colaOrdenes.fin.sig ← nuevoNodo
        colaOrdenes.fin ← nuevoNodo
    FinSi

    Escribir "Orden registrada correctamente"
Fin
```
---

## 3. Atender Orden de Venta (Actualizando Inventario y Ganancia)

Entrada: Cola de órdenes pendientes colaOrdenes,
Pila de productos pilaProductos,
Cola de ventas procesadas colaVentas,
Variable global gananciaTotal
Salida: Orden atendida (si es posible), inventario y ganancia actualizados
```plaintext
Acción AtenderOrden (colaOrdenes, pilaProductos, colaVentas, gananciaTotal)
Inicio
    Si (colaOrdenes.frente = Null) Entonces
        Escribir "No hay órdenes pendientes"
    Sino
        // Operación básica de Cola: DESENCOLAR
        nodoOrden ← colaOrdenes.frente
        colaOrdenes.frente ← colaOrdenes.frente.sig

        Si (colaOrdenes.frente = Null) Entonces
            colaOrdenes.fin ← Null
        FinSi

        orden ← nodoOrden.valor
        Eliminar nodoOrden

        // Buscar producto en la pila de inventario usando una pila auxiliar
        Crear pilaAux
        productoEncontrado ← FALSO

        Mientras (pilaProductos.tope ≠ Null Y productoEncontrado = FALSO) Hacer
            nodoProd ← pilaProductos.tope
            pilaProductos.tope ← pilaProductos.tope.sig

            prod ← nodoProd.valor

            Si (prod.codigo = orden.codigoProducto) Entonces
                productoEncontrado ← VERDADERO

                Si (prod.existencia ≥ orden.cantidad) Entonces
                    prod.existencia ← prod.existencia - orden.cantidad
                    importe ← prod.precioUnitario * orden.cantidad
                    gananciaTotal ← gananciaTotal + importe
                    Escribir "Orden atendida. Importe: ", importe

                Sino
                    Escribir "Stock insuficiente para el producto ", prod.codigo
                FinSi
            FinSi

            // Volvemos a apilar el nodo (con stock actualizado si era el buscado)
            nuevoNodoProd ← CrearNodo(prod)
            nuevoNodoProd.sig ← pilaAux.tope
            pilaAux.tope ← nuevoNodoProd

            Eliminar nodoProd
        FinMientras

        // Restaurar pila original desde pilaAux (para mantener UEPS)
        Mientras (pilaAux.tope ≠ Null) Hacer
            nodoProd ← pilaAux.tope
            pilaAux.tope ← pilaAux.tope.sig

            nodoProd.sig ← pilaProductos.tope
            pilaProductos.tope ← nodoProd
        FinMientras

        // Registrar la orden atendida en la cola de ventas
        Si (productoEncontrado = VERDADERO) Entonces
            Encolar(colaVentas, orden)
        FinSi
    FinSi
Fin
```
---

## 4. Atender Orden Urgente por Código

Entrada: int codigoOrdenBuscado, Cola de órdenes pendientes colaOrdenes,
Pila de productos pilaProductos, Cola de ventas colaVentas, gananciaTotal
Salida: Orden urgente atendida primero (si existe)
```plaintext
Acción AtenderOrdenUrgente (codigoOrdenBuscado, colaOrdenes, pilaProductos, colaVentas, gananciaTotal)
Inicio
    Si (colaOrdenes.frente = Null) Entonces
        Escribir "No hay órdenes pendientes"
    Sino
        Crear colaAux
        ordenUrgente ← Null

        // Buscar la orden en la cola usando solo operaciones básicas
        Mientras (colaOrdenes.frente ≠ Null) Hacer
            nodo ← colaOrdenes.frente
            colaOrdenes.frente ← colaOrdenes.frente.sig

            orden ← nodo.valor

            Si (orden.codigoOrden = codigoOrdenBuscado Y ordenUrgente = Null) Entonces
                ordenUrgente ← orden
                Eliminar nodo        // No se pasa a la cola auxiliar
            Sino
                Encolar(colaAux, orden)
                Eliminar nodo
            FinSi
        FinMientras

        colaOrdenes ← colaAux     // Restaurar cola sin la orden urgente

        Si (ordenUrgente = Null) Entonces
            Escribir "No se encontró la orden urgente"
        Sino
            // Reutilizamos la lógica de atención normal
            AtenderOrdenIndividual(ordenUrgente, pilaProductos, colaVentas, gananciaTotal)
        FinSi
    FinSi
Fin
```
---


## 5. Reporte de Ventas por Fecha

Entrada: Cola de ventas procesadas colaVentas, string fechaBuscada
Salida: Lista de ventas realizadas en la fecha indicada
```plaintext
Acción ReporteVentasPorFecha (colaVentas, fechaBuscada)
Inicio
    Si (colaVentas.frente = Null) Entonces
        Escribir "No hay ventas registradas"
    Sino
        Crear colaAux

        Mientras (colaVentas.frente ≠ Null) Hacer
            nodo ← colaVentas.frente
            colaVentas.frente ← colaVentas.frente.sig

            venta ← nodo.valor

            Si (venta.fechaOrden = fechaBuscada) Entonces
                Escribir "Orden: ", venta.codigoOrden,
                         "  Producto: ", venta.codigoProducto,
                         "  Cantidad: ", venta.cantidad
            FinSi

            Encolar(colaAux, venta)
            Eliminar nodo
        FinMientras

        // Restaurar cola original
        Mientras (colaAux.frente ≠ Null) Hacer
            nodo ← colaAux.frente
            colaAux.frente ← colaAux.frente.sig

            Encolar(colaVentas, nodo.valor)
            Eliminar nodo
        FinMientras
    FinSi
Fin
```
---
## 6. Reporte de Ventas por Producto

Entrada: Cola de ventas colaVentas, int codigoProductoBuscado
Salida: Lista de ventas del producto indicado
```plaintext
Acción ReporteVentasPorProducto (colaVentas, codigoProductoBuscado)
Inicio
    Si (colaVentas.frente = Null) Entonces
        Escribir "No hay ventas registradas"
    Sino
        Crear colaAux

        Mientras (colaVentas.frente ≠ Null) Hacer
            nodo ← colaVentas.frente
            colaVentas.frente ← colaVentas.frente.sig

            venta ← nodo.valor

            Si (venta.codigoProducto = codigoProductoBuscado) Entonces
                Escribir "Orden: ", venta.codigoOrden,
                         "  Fecha: ", venta.fechaOrden,
                         "  Cantidad: ", venta.cantidad
            FinSi

            Encolar(colaAux, venta)
            Eliminar nodo
        FinMientras

        // Restaurar cola original
        Mientras (colaAux.frente ≠ Null) Hacer
            nodo ← colaAux.frente
            colaAux.frente ← colaAux.frente.sig

            Encolar(colaVentas, nodo.valor)
            Eliminar nodo
        FinMientras
    FinSi
Fin
```
---
## 7. Mostrar Productos que Exceden un Precio Dado

Entrada: Pila de productos pilaProductos, real precioMinimo
Salida: Lista de productos cuyo precio unitario es mayor al valor ingresado
```plaintext
Acción MostrarProductosPorPrecio (pilaProductos, precioMinimo)
Inicio
    Si (pilaProductos.tope = Null) Entonces
        Escribir "No hay productos en el inventario"
    Sino
        Crear pilaAux

        Mientras (pilaProductos.tope ≠ Null) Hacer
            nodo ← pilaProductos.tope
            pilaProductos.tope ← pilaProductos.tope.sig

            prod ← nodo.valor

            Si (prod.precioUnitario > precioMinimo) Entonces
                Escribir "Codigo: ", prod.codigo,
                         "  Desc: ", prod.descripcion,
                         "  Precio: ", prod.precioUnitario
            FinSi

            // Pasamos a pila auxiliar para luego restaurar
            nodo.sig ← pilaAux.tope
            pilaAux.tope ← nodo
        FinMientras

        // Restaurar pila original
        Mientras (pilaAux.tope ≠ Null) Hacer
            nodo ← pilaAux.tope
            pilaAux.tope ← pilaAux.tope.sig

            nodo.sig ← pilaProductos.tope
            pilaProductos.tope ← nodo
        FinMientras
    FinSi
Fin
```
---
## 8. Reporte de Ganancia Total de la Tienda

Entrada: Variable global gananciaTotal
Salida: Monto total acumulado por las ventas
```plaintext
Acción MostrarGananciaTienda (gananciaTotal)
Inicio
    Escribir "Ganancia total de la tienda: ", gananciaTotal
Fin
```
---
