#include <stdio.h>
#include <pthread.h>
#include <unistd.h> 


void* cliente(void* arg) {
    int id = *(int*)arg;

    printf("Cliente %d está siendo atendido...\n", id);
    sleep(1); 
    printf("Cliente %d ha sido atendido y deja la fila.\n", id);

    return NULL;
}

int main() {
    const int num_clientes = 5;
    pthread_t hilos[num_clientes];
    int ids[num_clientes];

    for (int i = 0; i < num_clientes; i++) {
        ids[i] = i + 1; 
        pthread_create(&hilos[i], NULL, cliente, &ids[i]);
    }

    for (int i = 0; i < num_clientes; i++) {
        pthread_join(hilos[i], NULL);
    }

    printf("Todos los clientes han sido atendidos.\n");

    return 0;
}