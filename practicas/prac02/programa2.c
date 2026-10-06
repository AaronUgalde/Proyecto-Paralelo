#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define FILAS 3
#define COLUMNAS 9
#define ARCHIVO_ORIGEN "matriz.txt"
#define ARCHIVO_DESTINO "resultados.txt"

void tarea_hijo(int col) {
    FILE *f_in = fopen(ARCHIVO_ORIGEN, "r");
    if (f_in == NULL) {
        perror("[HIJO] Error al abrir archivo origen");
        exit(EXIT_FAILURE);
    }

    int matriz[FILAS][COLUMNAS];
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            if (fscanf(f_in, "%d", &matriz[i][j]) != 1) {
                fprintf(stderr, "[HIJO %d] Error al leer la matriz\n", col + 1);
                fclose(f_in);
                exit(EXIT_FAILURE);
            }
        }
    }
    fclose(f_in);

    long long producto = 1;
    for (int i = 0; i < FILAS; i++) {
        producto *= matriz[i][col];
    }

    FILE *f_out = fopen(ARCHIVO_DESTINO, "a");
    if (f_out == NULL) {
        perror("[HIJO] Error al abrir archivo de resultados");
        exit(EXIT_FAILURE);
    }

    if (fprintf(f_out, "Columna %d: %lld\n", col + 1, producto) < 0) {
        fprintf(stderr, "[HIJO %d] Error al escribir resultado\n", col + 1);
        fclose(f_out);
        exit(EXIT_FAILURE);
    }

    fclose(f_out);
    exit(EXIT_SUCCESS);
}

int main(void) {
    FILE *f_check = fopen(ARCHIVO_ORIGEN, "r");
    if (f_check == NULL) {
        fprintf(stderr, "[PADRE] Error: No existe '%s'. Ejecute primero el Programa 1.\n", ARCHIVO_ORIGEN);
        return EXIT_FAILURE;
    }
    fclose(f_check);

    FILE *f_init = fopen(ARCHIVO_DESTINO, "w");
    if (f_init == NULL) {
        perror("[PADRE] Error al inicializar el archivo de resultados");
        return EXIT_FAILURE;
    }
    fclose(f_init);

    printf("[PADRE] Creando %d procesos hijos para procesar las columnas en paralelo...\n", COLUMNAS);
    fflush(stdout); // Vacia el bufer antes de fork(): evita que cada hijo repita este mensaje al terminar

    for (int col = 0; col < COLUMNAS; col++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("[PADRE] Error al ejecutar fork()");
            return EXIT_FAILURE;
        } else if (pid == 0) {
            // Proceso Hijo
            tarea_hijo(col);
        }
    }

    for (int i = 0; i < COLUMNAS; i++) {
        int status;
        pid_t pid_hijo = wait(&status);
        if (pid_hijo > 0) {
            if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
                fprintf(stderr, "[PADRE] Advertencia: Proceso hijo (PID %d) termino con error.\n", pid_hijo);
            }
        }
    }

    printf("\n[PADRE] Todos los hijos han terminado. Resultados contenidos en '%s':\n", ARCHIVO_DESTINO);
    printf("--------------------------------------------------\n");

    FILE *f_res = fopen(ARCHIVO_DESTINO, "r");
    if (f_res == NULL) {
        perror("[PADRE] Error al abrir el archivo de resultados");
        return EXIT_FAILURE;
    }

    char linea[256];
    while (fgets(linea, sizeof(linea), f_res) != NULL) {
        printf("%s", linea);
    }
    fclose(f_res);

    printf("--------------------------------------------------\n");
    return EXIT_SUCCESS;
}