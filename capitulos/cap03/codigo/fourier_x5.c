#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include <math.h>

int main() {
    int i, n;
    int shmid;
    double *memoria;
    sem_t *semaforo;
    double PI = 3.14159265359;
    
    // Se reservan 500 espacios para la suma total y
    // 100 procesos con 500 puntos para cada término.
    // Total = 101 filas x 500 columnas = 50500 espacios
    int total_espacios = 101 * 500;

    // 1. Solicitar la memoria compartida
    shmid = shmget(IPC_PRIVATE, total_espacios * sizeof(double), IPC_CREAT | 0666);
    if (shmid == -1) {
        printf("Error pidiendo memoria\n");
        return 1;
    }

    // 2. Conectar la memoria compartida al proceso
    memoria = (double *) shmat(shmid, NULL, 0);

    // Inicializar todos los valores de la memoria en cero
    for (i = 0; i < total_espacios; i++) {
        memoria[i] = 0.0;
    }

    // 3. Crear el semáforo para controlar el acceso a la memoria compartida
    sem_unlink("/candado_said"); 
    semaforo = sem_open("/candado_said", O_CREAT, 0644, 1);

    printf("Iniciando los 100 procesos para f(x) = x^5...\n");

    // 4. Crear los 100 procesos hijos
    for (n = 1; n <= 100; n++) {
        pid_t pid = fork();

        if (pid == 0) {
            // --- CÓDIGO DEL HIJO ---
            double signo;

            // Determinar el signo correspondiente al término n
            if (n % 2 == 0) {
                signo = -1.0;
            } else {
                signo = 1.0;
            }

            // Calcular los componentes del coeficiente b_n
            double parte1 = pow(PI, 4) / n;
            double parte2 = (20.0 * pow(PI, 2)) / pow(n, 3);
            double parte3 = 120.0 / pow(n, 5);
            double bn = 2.0 * signo * (parte1 - parte2 + parte3);

            // Evaluar el término de Fourier en 500 puntos
            for (i = 0; i < 500; i++) {
                double x = -PI + i * (2.0 * PI / 499.0);
                double calculo = bn * sin(n * x);

                // A) Almacenar el valor correspondiente al término n
                // La expresión n * 500 permite acceder a la sección
                // de memoria asignada al término correspondiente.
                memoria[n * 500 + i] = calculo;

                // B) Acumular el valor en la suma total de Fourier
                // El semáforo garantiza el acceso exclusivo a esta
                // sección de memoria compartida.
                sem_wait(semaforo);
                memoria[i] = memoria[i] + calculo; 
                sem_post(semaforo);
            }
            
            exit(0); 
            // --- FIN DEL CÓDIGO DEL HIJO ---
        }
    }

    // --- CÓDIGO DEL PADRE ---

    // 5. Esperar a que finalicen los 100 procesos hijos
    for (n = 1; n <= 100; n++) {
        wait(NULL);
    }

    // 6. Crear el archivo CSV para almacenar los resultados
    FILE *archivo = fopen("datos_said.csv", "w");
    
    // Escribir la primera fila con los encabezados de las columnas
    fprintf(archivo, "x,f(x)=x^5,Fourier(N=100)");
    for (n = 1; n <= 100; n++) {
        fprintf(archivo, ",Termino n=%d", n);
    }
    fprintf(archivo, "\n");

    // Escribir los datos correspondientes a cada punto x
    for (i = 0; i < 500; i++) {
        double x = -PI + i * (2.0 * PI / 499.0);
        double fx = pow(x, 5); 
        
        // Escribir x, f(x) y la suma de los 100 términos de Fourier
        fprintf(archivo, "%f,%f,%f", x, fx, memoria[i]);
        
        // Escribir los valores individuales de los 100 términos
        for (n = 1; n <= 100; n++) {
            fprintf(archivo, ",%f", memoria[n * 500 + i]);
        }
        fprintf(archivo, "\n");
    }
    fclose(archivo);

    printf("Listo. Datos guardados en datos_said.csv\n");

    // 7. Liberar la memoria compartida y cerrar el semáforo
    sem_close(semaforo);
    sem_unlink("/candado_said");
    shmdt(memoria);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}