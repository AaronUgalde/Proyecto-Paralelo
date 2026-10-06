#include <stdio.h>
#include <stdlib.h>

#define FILAS 3
#define COLUMNAS 9
#define ARCHIVO_MATRIZ "matriz.txt"

int main(void) {
    int matriz[FILAS][COLUMNAS] = {
        {1, 2, 3, 4, 5, 6, 7, 8, 9},
        {1, 3, 5, 7, 9, 11, 13, 15, 17},
        {2, 4, 6, 8, 10, 12, 14, 16, 18}
    };

    FILE *archivo = fopen(ARCHIVO_MATRIZ, "w");
    if (archivo == NULL) {
        perror("Error al crear el archivo de la matriz");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            if (fprintf(archivo, "%d%s", matriz[i][j], (j == COLUMNAS - 1) ? "" : " ") < 0) {
                fprintf(stderr, "Error al escribir datos en %s\n", ARCHIVO_MATRIZ);
                fclose(archivo);
                return EXIT_FAILURE;
            }
        }
        fprintf(archivo, "\n");
    }

    fclose(archivo);
    printf("[PROGRAMA 1] El archivo '%s' se ha generado exitosamente.\n", ARCHIVO_MATRIZ);
    return EXIT_SUCCESS;
}