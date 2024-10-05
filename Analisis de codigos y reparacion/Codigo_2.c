//Codigo 2
//Analizado y reparado por: Capibara Team 
//Grupo: 4CV4

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_CLIENTES 100
#define CLIENTES_POR_FURGON 20

// Definir semáforos y variables de control
sem_t clientes_en_sucursal;
sem_t operacion_en_curso;
pthread_mutex_t mutex;

int clientes = 0;

void* llegada_cliente(void* arg) {
    while (1) {
        usleep(rand() % 3000000); // Espera un tiempo aleatorio entre 0 y 3 segundos.
        pthread_mutex_lock(&mutex); // Bloquea el acceso a la variable clientes.
        clientes++;
        printf("Cliente entró a la sucursal. Clientes en la sucursal: %d\n", clientes);

        if (clientes % CLIENTES_POR_FURGON == 0 && clientes <= MAX_CLIENTES) {
            printf("\nEl furgón blindado está llegando! La sucursal se está vaciando...\n");
            sem_wait(&operacion_en_curso); // Espera a que se complete la operación del furgón.
            for (int i = clientes; i > 0; i--) {
                printf("Cliente salió de la sucursal. Clientes en la sucursal: %d\n", i - 1);
                usleep(50000); // Simula la salida de los clientes.
            }
            printf("El furgón blindado puede realizar la operación ahora.\n");
            sem_post(&operacion_en_curso); // Señala que la operación del furgón puede comenzar.
        }
        pthread_mutex_unlock(&mutex); // Libera el acceso a la variable clientes.
        sem_post(&clientes_en_sucursal); // Incrementa el número de clientes en la sucursal.
    }
}

void* operacion_furgon(void* arg) {
    while (1) {
        sem_wait(&clientes_en_sucursal); // Espera hasta que haya clientes en la sucursal.
        if (clientes % CLIENTES_POR_FURGON == 0 && clientes <= MAX_CLIENTES) {
            sem_wait(&operacion_en_curso); // Espera a que se le permita realizar la operación.
            printf("Furgón blindado realizando la operación de carga/descarga de dinero...\n");
            usleep(rand() % 200000); // Simula la operación del furgón durante un tiempo aleatorio.
            sem_post(&operacion_en_curso); // Señala que la operación ha finalizado.
            printf("Furgón blindado completó la operación.\n");
        }
    }
}

int main() {
    pthread_t hilo_clientes, hilo_furgon;

    sem_init(&clientes_en_sucursal, 0, 0);
    sem_init(&operacion_en_curso, 0, 1);
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&hilo_clientes, NULL, llegada_cliente, NULL);
    pthread_create(&hilo_furgon, NULL, operacion_furgon, NULL);

    pthread_join(hilo_clientes, NULL);
    pthread_join(hilo_furgon, NULL);

    sem_destroy(&clientes_en_sucursal);
    sem_destroy(&operacion_en_curso);
    pthread_mutex_destroy(&mutex);

    return 0;
}