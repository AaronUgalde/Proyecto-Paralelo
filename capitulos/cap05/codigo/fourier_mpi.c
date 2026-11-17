/*
 * =====================================================================
 * INSTITUTO POLITECNICO NACIONAL - ESCUELA SUPERIOR DE COMPUTO
 * Computo Paralelo - Grupo 6BV1
 * Capitulo 5: Programacion con openMPI
 *
 * Funcion: f(x) = -(1/4) x^4 + 3   (misma funcion del Capitulo 4)
 * Calcula los coeficientes n, an, bn de los primeros 100 armonicos de
 * su serie de Fourier repartiendolos entre los procesos MPI.
 *
 * Uso:  mpirun -np 100 --oversubscribe ./fourier_mpi
 *       (con -np P, 1 <= P <= 100; el reparto se adapta a P)
 * Salidas: fourier_mpi_resultados.csv (n, an, bn, proceso; n=0 es a0/2)
 *          fourier_mpi_grafica.csv    (x, f(x) y sumas parciales S_N(x)
 *                                      con N = 1, 3, 5, 10, 50, 100)
 * =====================================================================
 */

#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_ARMONICOS 100
#define CAMPOS 3 /* n, an, bn por armonico */
#define ARCHIVO_CSV "fourier_mpi_resultados.csv"
#define ARCHIVO_GRAFICA "fourier_mpi_grafica.csv"
#define NUM_PUNTOS 201 /* x en [-pi, pi], paso de 2*pi/200 */
#define NUM_CORTES 6   /* sumas parciales con N armonicos */

static const int CORTES[NUM_CORTES] = {1, 3, 5, 10, 50, 100};

/* Coeficientes del armonico n (misma formula que el programa de hilos). */
static void calcular_armonico(int n, double *an, double *bn)
{
    *an = (2.0 * ((n % 2 == 0) ? 1.0 : -1.0) * (6.0 - (n * n * M_PI * M_PI))) /
          ((double)n * n * n * n);
    *bn = 0.0; /* la funcion es par */
}

int main(int argc, char *argv[])
{
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    /* Validacion: no puede haber mas procesos que armonicos. */
    if (size > NUM_ARMONICOS) {
        if (rank == 0)
            fprintf(stderr, "Error: se pidieron %d procesos; el maximo es %d.\n",
                    size, NUM_ARMONICOS);
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    /* Reparto por bloques: los primeros 'resto' procesos toman uno mas. */
    int base = NUM_ARMONICOS / size;
    int resto = NUM_ARMONICOS % size;
    int mios = base + (rank < resto ? 1 : 0);
    int inicio = rank * base + (rank < resto ? rank : resto);

    double t0 = MPI_Wtime();

    double *local = malloc((size_t)mios * CAMPOS * sizeof(double));
    if (local == NULL) {
        fprintf(stderr, "[P%d] Error: sin memoria.\n", rank);
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    double *aporte = calloc((size_t)NUM_CORTES * NUM_PUNTOS, sizeof(double));
    double *suma = NULL;
    if (aporte == NULL) {
        fprintf(stderr, "[P%d] Error: sin memoria.\n", rank);
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
    if (rank == 0)
        suma = calloc((size_t)NUM_CORTES * NUM_PUNTOS, sizeof(double));

    for (int i = 0; i < mios; i++) {
        int n = inicio + i + 1; /* armonicos 1..100 */
        double an, bn;
        calcular_armonico(n, &an, &bn);
        local[i * CAMPOS + 0] = n;
        local[i * CAMPOS + 1] = an;
        local[i * CAMPOS + 2] = bn;

        /* Aporte del armonico n a cada suma parcial S_N(x) con n <= N. */
        for (int c = 0; c < NUM_CORTES; c++) {
            if (n > CORTES[c])
                continue;
            for (int k = 0; k < NUM_PUNTOS; k++) {
                double x = -M_PI + 2.0 * M_PI * k / (NUM_PUNTOS - 1);
                aporte[c * NUM_PUNTOS + k] += an * cos(n * x) + bn * sin(n * x);
            }
        }
    }

    /* El proceso 0 recopila los resultados de todos con MPI_Gatherv. */
    int *conteos = NULL, *desplaz = NULL;
    double *todos = NULL;
    if (rank == 0) {
        conteos = malloc((size_t)size * sizeof(int));
        desplaz = malloc((size_t)size * sizeof(int));
        todos = malloc((size_t)NUM_ARMONICOS * CAMPOS * sizeof(double));
        if (!conteos || !desplaz || !todos) {
            fprintf(stderr, "[P0] Error: sin memoria.\n");
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }
        for (int p = 0; p < size; p++) {
            conteos[p] = (base + (p < resto ? 1 : 0)) * CAMPOS;
            desplaz[p] = (p * base + (p < resto ? p : resto)) * CAMPOS;
        }
    }

    MPI_Gatherv(local, mios * CAMPOS, MPI_DOUBLE, todos, conteos, desplaz,
                MPI_DOUBLE, 0, MPI_COMM_WORLD);

    /* Suma de los aportes de todos los procesos en el proceso 0. */
    MPI_Reduce(aporte, suma, NUM_CORTES * NUM_PUNTOS, MPI_DOUBLE, MPI_SUM, 0,
               MPI_COMM_WORLD);

    double t_local = MPI_Wtime() - t0, t_max;
    MPI_Reduce(&t_local, &t_max, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        FILE *fp = fopen(ARCHIVO_CSV, "w");
        if (fp == NULL) {
            perror("Error al crear el archivo CSV");
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }
        fprintf(fp, "n,an,bn,proceso\n");
        /* Fila n = 0: termino constante a0/2 (como en la grafica del Cap. 4). */
        fprintf(fp, "0,%.10f,%.10f,0\n", 3.0 - pow(M_PI, 4) / 20.0, 0.0);
        for (int p = 0; p < size; p++) {
            int cuantos = conteos[p] / CAMPOS, ini = desplaz[p] / CAMPOS;
            for (int i = 0; i < cuantos; i++) {
                const double *r = &todos[(ini + i) * CAMPOS];
                fprintf(fp, "%d,%.10f,%.10f,%d\n", (int)r[0], r[1], r[2], p);
            }
        }
        fclose(fp);

        double a0_medio = 3.0 - pow(M_PI, 4) / 20.0; /* a0/2 */

        FILE *fg = fopen(ARCHIVO_GRAFICA, "w");
        if (fg == NULL) {
            perror("Error al crear el archivo CSV de la grafica");
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }
        fprintf(fg, "x,f_original");
        for (int c = 0; c < NUM_CORTES; c++)
            fprintf(fg, ",S_%d", CORTES[c]);
        fprintf(fg, "\n");
        for (int k = 0; k < NUM_PUNTOS; k++) {
            double x = -M_PI + 2.0 * M_PI * k / (NUM_PUNTOS - 1);
            fprintf(fg, "%.6f,%.10f", x, -0.25 * pow(x, 4) + 3.0);
            for (int c = 0; c < NUM_CORTES; c++)
                fprintf(fg, ",%.10f", a0_medio + suma[c * NUM_PUNTOS + k]);
            fprintf(fg, "\n");
        }
        fclose(fg);
        printf("====================================================\n");
        printf(" Serie de Fourier f(x) = -(1/4)x^4 + 3  con openMPI\n");
        printf(" Procesos MPI : %d\n", size);
        printf(" Armonicos    : %d (hasta %d por proceso)\n", NUM_ARMONICOS,
               base + (resto ? 1 : 0));
        printf(" a0/2         : %.10f\n", a0_medio);
        printf(" Tiempo (max) : %.6f s\n", t_max);
        printf(" Coeficientes : %s\n", ARCHIVO_CSV);
        printf(" Grafica      : %s\n", ARCHIVO_GRAFICA);
        printf("====================================================\n");
        free(conteos);
        free(desplaz);
        free(todos);
    }

    free(aporte);
    free(suma);
    free(local);
    MPI_Finalize();
    return EXIT_SUCCESS;
}
