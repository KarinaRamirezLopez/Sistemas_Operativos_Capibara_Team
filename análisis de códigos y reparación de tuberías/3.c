#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
    pid_t rf;
    rf = fork();
    
    switch (rf) {
        case -1:
            printf("\nNo he podido crear el proceso hijo\n");
            break;
        case 0:
            printf("\nSoy el hijo, mi PID es %d y mi PPID es %d\n", getpid(), getppid());
            sleep(10);  // El hijo duerme por 10 segundos
            break;
        default:
            printf("\nSoy el padre, mi PID es %d y el PID de mi hijo es %d\n", getpid(), rf);
            break;
    }
    
    printf("\nFinal de ejecución de %d\n", getpid());
    exit(0);
}
