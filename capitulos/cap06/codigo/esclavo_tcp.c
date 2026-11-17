#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <math.h>

// Compilación: gcc Cliente_TCP.c -o Cliente_TCP -lm
// Ejecución:   ./Cliente_TCP [IP Servidor] [Puerto] [N_armonico]

int main(int argc, char *argv[])
{
    if(argc != 4)
    {
        fprintf(stderr, "Uso: %s [IP Servidor] [Puerto] [N_armonico]\n", argv[0]);
        exit(-1);
    }

    char *ip = argv[1];
    int puerto = atoi(argv[2]);
    int n = atoi(argv[3]);

    struct hostent *he;
    struct sockaddr_in servidor;
    int fd;

    if ((he = gethostbyname(ip)) == NULL)
    {
        perror("gethostbyname() error");
        exit(-1);
    }

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket() error");
        exit(-1);
    }

    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(puerto);
    servidor.sin_addr = *((struct in_addr *)he->h_addr);
    bzero(&(servidor.sin_zero), 8);

    if(connect(fd, (struct sockaddr *)&servidor, sizeof(struct sockaddr)) == -1)
    {
        perror("connect() error");
        exit(-1);
    }

    // --- CÁLCULO DEL ARMÓNICO ---
    // Función: -1/4 * n^4 + 3
    double armonico = -0.25 * pow((double)n, 4) + 3.0;

    char buffer[100];
    snprintf(buffer, sizeof(buffer), "%lf", armonico);

    if(send(fd, buffer, strlen(buffer), 0) == -1)
    {
        perror("Error en send()");
        exit(-1);
    }

    printf("Armónico %d (%lf) enviado al servidor TCP con éxito.\n", n, armonico);

    close(fd);
    return 0;
}
