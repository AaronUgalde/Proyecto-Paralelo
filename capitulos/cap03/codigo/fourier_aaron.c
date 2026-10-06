/*
 * =====================================================================================
 * INSTITUTO POLITÉCNICO NACIONAL - ESCUELA SUPERIOR DE CÓMPUTO
 * Cómputo Paralelo - Grupo 6BV1
 * Capítulo 3: Programación con Procesos
 * 
 * Autor: Aarón Ugalde Téllez
 * Función asignada: f(x) = -5/6 * x^2 + pi/3 * x + 2/7
 * Configuración: 100 Procesos Hijos, Memoria Compartida, Semáforos System V.
 * Puntos de evaluación: 64 valores específicos de x especificados por el usuario.
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>

#define NUM_PROCESOS 100  // Exactly 100 child processes
#define NUM_PUNTOS   64   // Array size corresponding to specified x values

// Arreglo estático con los 64 valores exactos de x
static const double x_values[NUM_PUNTOS] = {
    -3.1416, -3.0416, -2.9416, -2.8416, -2.7416, -2.6416, -2.5416, -2.4416,
    -2.3416, -2.2416, -2.1416, -2.0416, -1.9416, -1.8416, -1.7416, -1.6416,
    -1.5416, -1.4416, -1.3416, -1.2416, -1.1416, -1.0416, -0.9416, -0.8416,
    -0.7416, -0.6416, -0.5416, -0.4416, -0.3416, -0.2416, -0.1416, -0.0416,
     0.0584,  0.1584,  0.2584,  0.3584,  0.4584,  0.5584,  0.6584,  0.7584,
     0.8584,  0.9584,  1.0584,  1.1584,  1.2584,  1.3584,  1.4584,  1.5584,
     1.6584,  1.7584,  1.8584,  1.9584,  2.0584,  2.1584,  2.2584,  2.3584,
     2.4584,  2.5584,  2.6584,  2.7584,  2.8584,  2.9584,  3.0584,  3.1584
};

// Estructura requerida para semctl
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

// Operación P (Wait / Bloquear semáforo)
void sem_wait_op(int sem_id) {
    struct sembuf sb = {0, -1, 0};
    semop(sem_id, &sb, 1);
}

// Operación V (Signal / Liberar semáforo)
void sem_post_op(int sem_id) {
    struct sembuf sb = {0, 1, 0};
    semop(sem_id, &sb, 1);
}

int main() {
    printf("====================================================================\n");
    printf(" CÓMPUTO PARALELO - CAPÍTULO 3: PROGRAMACIÓN CON PROCESOS\n");
    printf(" Integrante: Aarón Ugalde Téllez\n");
    printf(" Evaluación de Serie de Fourier con 100 Procesos Hijos (%d Puntos)\n", NUM_PUNTOS);
    printf("====================================================================\n\n");

    // 1. Crear Memoria Compartida para guardar el arreglo de los 64 resultados
    size_t shm_size = NUM_PUNTOS * sizeof(double);
    int shm_id = shmget(IPC_PRIVATE, shm_size, IPC_CREAT | 0666);
    if (shm_id < 0) {
        perror("Error al crear la memoria compartida (shmget)");
        exit(EXIT_FAILURE);
    }

    // Acoplar la memoria compartida al espacio de direcciones
    double *f_x = (double *)shmat(shm_id, NULL, 0);
    if (f_x == (double *)-1) {
        perror("Error al acoplar la memoria compartida (shmat)");
        exit(EXIT_FAILURE);
    }

    // 2. Inicializar la memoria compartida con el valor a0
    // a0 = -5/18 * pi^2 - 2/7
    double a0 = (-5.0 / 18.0) * (M_PI * M_PI) - (2.0 / 7.0);
    for (int i = 0; i < NUM_PUNTOS; i++) {
        f_x[i] = a0;
    }

    // 3. Crear e inicializar el semáforo System V
    int sem_id = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
    if (sem_id < 0) {
        perror("Error al crear el semáforo (semget)");
        exit(EXIT_FAILURE);
    }

    union semun arg;
    arg.val = 1; // Inicializado en 1 (sección crítica libre)
    if (semctl(sem_id, 0, SETVAL, arg) < 0) {
        perror("Error al inicializar el semáforo (semctl)");
        exit(EXIT_FAILURE);
    }

    printf("[PADRE | PID: %d] Creando %d procesos hijos...\n\n", getpid(), NUM_PROCESOS);

    // 4. Crear los 100 procesos hijos
    for (int p = 1; p <= NUM_PROCESOS; p++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("Error al crear proceso hijo (fork)");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            // === PROCESO HIJO ===
            int n = p; // El hijo 'p' procesa el armónico n = p
            double local_terms[NUM_PUNTOS];

            // Coeficientes an y bn de la función de Aarón Ugalde
            double signo = ((n + 1) % 2 == 0) ? 1.0 : -1.0; // (-1)^(n+1)
            double an = (10.0 * signo) / (3.0 * n * n);
            double bn = (2.0 * signo) / (3.0 * n);

            // Calcular término n para cada uno de los 64 puntos x especificados
            for (int i = 0; i < NUM_PUNTOS; i++) {
                double x = x_values[i];
                local_terms[i] = an * cos(n * x) + bn * sin(n * x);
            }

            // --- SECCIÓN CRÍTICA ---
            sem_wait_op(sem_id); // Entrar a sección crítica

            for (int i = 0; i < NUM_PUNTOS; i++) {
                f_x[i] += local_terms[i];
            }

            sem_post_op(sem_id); // Salir de sección crítica
            // ------------------------

            printf("[HIJO #%03d | PID: %d] Armónico n = %3d procesado para los %d puntos.\n", n, getpid(), n, NUM_PUNTOS);

            shmdt(f_x);
            exit(EXIT_SUCCESS);
        }
    }

    // 5. El proceso padre espera a que finalicen los 100 hijos
    for (int i = 0; i < NUM_PROCESOS; i++) {
        wait(NULL);
    }

    printf("\n[PADRE] Los %d procesos hijos han completado el cálculo.\n", NUM_PROCESOS);

    // 6. Generar archivo CSV con los resultados exactos
    const char *filename = "fourier_aaron_64puntos.csv";
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        perror("Error al crear el archivo CSV de salida");
        exit(EXIT_FAILURE);
    }

    fprintf(fp, "x,f_fourier\n");
    for (int i = 0; i < NUM_PUNTOS; i++) {
        fprintf(fp, "%.4f,%.6f\n", x_values[i], f_x[i]);
    }
    fclose(fp);

    printf("[PADRE] Resultados guardados en el archivo '%s'.\n", filename);

    // 7. Liberar memoria compartida y semáforos
    shmdt(f_x);
    shmctl(shm_id, IPC_RMID, NULL);
    semctl(sem_id, 0, IPC_RMID);

    printf("[PADRE] Recursos IPC (SHM y semáforos) liberados correctamente.\n");
    printf("====================================================================\n");

    return 0;
}