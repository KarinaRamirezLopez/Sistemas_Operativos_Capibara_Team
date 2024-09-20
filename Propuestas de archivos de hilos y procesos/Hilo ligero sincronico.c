#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* tarea(void* arg) {
    printf("Hilo iniciado\n");
    sleep(2);  
    printf("Hilo terminado\n");
    return NULL;
}

int main() {
    pthread_t hilo;

    
    if (pthread_create(&hilo, NULL, tarea, NULL) != 0) {
        perror("Error al crear el hilo");
        return 1;
    }

    
    if (pthread_join(hilo, NULL) != 0) {
        perror("Error al esperar al hilo");
        return 1;
    }

    printf("El hilo ha terminado, continuando en el hilo principal\n");
    return 0;
}

