#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define FILAS 3
#define COLUMNAS 9
#define NUM_HILOS FILAS

/* Datos que el proceso padre le pasa a cada hilo mediante un apuntador.
 * - fila:      apuntador a la fila de la matriz que debe procesar.
 * - resultado: apuntador a la celda del arreglo de resultados donde el
 *              hilo escribe el producto (memoria del proceso padre). */
typedef struct {
    int numero;
    const int *fila;
    int columnas;
    long long *resultado;
} DatosHilo;

/* Funcion que ejecuta cada hilo: multiplica los elementos de su fila. */
static void *multiplicar_fila(void *arg) {
    DatosHilo *datos = (DatosHilo *)arg;
    char linea[256];
    int pos = 0;
    long long producto = 1;

    pos += snprintf(linea + pos, sizeof(linea) - (size_t)pos,
                    "[HILO %d | id=%lu] Datos de la fila:",
                    datos->numero, (unsigned long)pthread_self());

    for (int j = 0; j < datos->columnas; j++) {
        pos += snprintf(linea + pos, sizeof(linea) - (size_t)pos,
                        " %d", datos->fila[j]);
        producto *= datos->fila[j];
    }

    /* Escritura del resultado en la memoria del proceso padre. */
    *(datos->resultado) = producto;

    /* Un solo printf por hilo para que las lineas no se mezclen. */
    printf("%s\n[HILO %d | id=%lu] Resultado (producto de la fila): %lld\n",
           linea, datos->numero, (unsigned long)pthread_self(), producto);

    return NULL;
}

int main(void) {
    int matriz[FILAS][COLUMNAS] = {
        {1, 2, 3, 4, 5, 6, 7, 8, 9},
        {1, 3, 5, 7, 9, 11, 13, 15, 17},
        {2, 4, 6, 8, 10, 12, 14, 16, 18}
    };
    long long resultados[NUM_HILOS] = {0};
    pthread_t hilos[NUM_HILOS];
    DatosHilo datos[NUM_HILOS];

    printf("[PADRE] Matriz creada (%d x %d):\n", FILAS, COLUMNAS);
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            printf("%3d ", matriz[i][j]);
        }
        printf("\n");
    }

    printf("\n[PADRE] Creando %d hilos...\n", NUM_HILOS);
    for (int i = 0; i < NUM_HILOS; i++) {
        datos[i].numero = i + 1;
        datos[i].fila = matriz[i];
        datos[i].columnas = COLUMNAS;
        datos[i].resultado = &resultados[i];

        int rc = pthread_create(&hilos[i], NULL, multiplicar_fila, &datos[i]);
        if (rc != 0) {
            fprintf(stderr, "Error al crear el hilo %d (codigo %d)\n", i + 1, rc);
            return EXIT_FAILURE;
        }
    }

    /* El padre detecta que los hilos terminaron esperandolos con join. */
    for (int i = 0; i < NUM_HILOS; i++) {
        int rc = pthread_join(hilos[i], NULL);
        if (rc != 0) {
            fprintf(stderr, "Error al esperar al hilo %d (codigo %d)\n", i + 1, rc);
            return EXIT_FAILURE;
        }
    }

    printf("\n[PADRE] Todos los hilos terminaron. Resultados:\n");
    for (int i = 0; i < NUM_HILOS; i++) {
        printf("[PADRE] Fila %d -> %lld\n", i + 1, resultados[i]);
    }

    return EXIT_SUCCESS;
}
