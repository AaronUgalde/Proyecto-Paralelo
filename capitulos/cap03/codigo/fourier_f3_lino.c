#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define N_PROCESOS 100

typedef struct {
    int n;
    double an;
    double bn;
} TerminoFourier;

void sem_wait(int semid) {
    struct sembuf sb = {0, -1, 0};
    semop(semid, &sb, 1);
}

void sem_post(int semid) {
    struct sembuf sb = {0, 1, 0};
    semop(semid, &sb, 1);
}

int main() {
    int shmid = shmget(IPC_PRIVATE, sizeof(TerminoFourier) * N_PROCESOS, IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("Error en memoria compartida");
        exit(EXIT_FAILURE);
    }

    TerminoFourier *tabla = (TerminoFourier *) shmat(shmid, NULL, 0);
    if (tabla == (void *) -1) {
        perror("Error al acoplar memoria compartida");
        exit(EXIT_FAILURE);
    }

    int semid = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
    if (semid < 0) {
        perror("Error en semáforo");
        exit(EXIT_FAILURE);
    }
    semctl(semid, 0, SETVAL, 1);

    printf("[PADRE] Calculando Fourier para f(x) = -(1/4)x^4 + 3 con %d procesos hijos...\n", N_PROCESOS);

    for (int i = 0; i < N_PROCESOS; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("Error en fork()");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            int n = i + 1;
            double an = (2.0 * ((n % 2 == 0) ? 1.0 : -1.0) * (6.0 - (n * n * M_PI * M_PI))) / (n * n * n * n);
            double bn = 0.0;

            sem_wait(semid);
            tabla[i].n = n;
            tabla[i].an = an;
            tabla[i].bn = bn;
            sem_post(semid);

            shmdt(tabla);
            exit(EXIT_SUCCESS);
        }
    }

    for (int i = 0; i < N_PROCESOS; i++) {
        wait(NULL);
    }

    double a0_half = 3.0 - (pow(M_PI, 4) / 20.0);

    FILE *archivo = fopen("fourier_f3_lino.csv", "w");
    if (!archivo) {
        perror("Error al crear archivo CSV");
        exit(EXIT_FAILURE);
    }

    fprintf(archivo, "n,a_n,b_n\n");
    fprintf(archivo, "0,%.6f,0.000000\n", a0_half);
    for (int i = 0; i < N_PROCESOS; i++) {
        fprintf(archivo, "%d,%.6f,%.6f\n", tabla[i].n, tabla[i].an, tabla[i].bn);
    }
    fclose(archivo);

    printf("[PADRE] Proceso completado. Datos exportados a 'fourier_f3_lino.csv'.\n");

    shmdt(tabla);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID);

    return 0;
}
