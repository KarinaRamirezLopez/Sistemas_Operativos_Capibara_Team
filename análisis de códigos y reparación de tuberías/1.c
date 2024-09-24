#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

void imprimeDePipe(int leePipe);  // Funcionalidad para el hijo
void enviaAPipe(int escribePipe); // Funcionalidad para el padre

int main() {
    pid_t procHijo;
    int pipeFileDescriptors[2]; // Descriptores de ambos extremos

    // Genera pipe
    if (pipe(pipeFileDescriptors) == -1) {
        printf("Error al crear pipe\n");
        exit(1);
    }

    procHijo = fork(); // Genera proceso hijo
    if (procHijo < 0) {
        int errnum = errno; // Preservamos código de error
        printf("Error %d al generar proceso hijo con fork\n", errnum);
        exit(1);
    }

    if (procHijo == 0) {
        // Es el hijo, cierra pipe de envío y procede
        close(pipeFileDescriptors[1]);
        imprimeDePipe(pipeFileDescriptors[0]);
    } else {
        // Es el padre, cierra pipe de recepción y procede
        close(pipeFileDescriptors[0]);
        enviaAPipe(pipeFileDescriptors[1]);
    }

    return 0;
}

/**
 * Funcionalidad para el padre: lee una cadena de hasta 10 caracteres,
 * la escribe a su extremo del pipe, cierra su extremo y espera a que
 * el proceso hijo termine antes de terminar su propio proceso.
 */
void enviaAPipe(int escribePipe) {
    char buf[10]; // Buffer de hasta 10 caracteres
    printf("Proceso padre, ingresa una cadena de hasta 10 caracteres y presiona Enter: ");

    // Leer la cadena del usuario
    scanf("%10s", buf);
    printf("\n\n"); // Separa la entrada de las futuras salidas.

    // Escribir en el pipe
    write(escribePipe, buf, strlen(buf));

    // Cerrar el pipe de escritura
    close(escribePipe); // Presenta EOF al proceso hijo

    // Esperar la terminación del proceso hijo
    wait(NULL);
    printf("Hijo terminado, terminando proceso padre\n");
    exit(0);
}

/**
 * Funcionalidad para el hijo: lee desde el pipe y muestra el contenido.
 */
void imprimeDePipe(int leePipe) {
    char buf[10];
    int bytesLeidos;

    // Leer los datos del pipe
    bytesLeidos = read(leePipe, buf, sizeof(buf) - 1);

    if (bytesLeidos > 0) {
        buf[bytesLeidos] = '\0'; // Asegurar que el buffer sea una cadena nula terminada
        printf("Proceso hijo recibió: %s\n", buf);
    }

    // Cerrar el pipe de lectura
    close(leePipe);
    exit(0);
}
