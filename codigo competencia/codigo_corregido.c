//Revisado por Capibara Team
#include <stdio.h>                     // Incluir la biblioteca estándar para entrada/salida
#include <stdlib.h>                    // Incluir la biblioteca estándar para funciones de utilidad
#include <sys/ipc.h>                   // Incluir la biblioteca para la creación de claves IPC
#include <sys/shm.h>                   // Incluir la biblioteca para la gestión de memoria compartida
#include <pthread.h>                   // Incluir la biblioteca para el uso de hilos
#include <semaphore.h>                 // Incluir la biblioteca para el uso de semáforos
#include <unistd.h>                    // Incluir la biblioteca para funciones de Unix
#include <sys/types.h>                 // Incluir la biblioteca para tipos de datos en sistemas Unix
#include <sys/wait.h>                  // Incluir la biblioteca para la gestión de procesos

#define QUANTUM 1                     // Definir el tiempo fijo (quantum) para el algoritmo Round Robin

// Estructura para gestionar el estado del pedido
typedef struct {
    int entradaLista;                // Indicador de si la entrada está lista
    int platoPrincipalListo;         // Indicador de si el plato principal está listo
    int bebidaLista;                 // Indicador de si la bebida está lista
    pthread_mutex_t mutex;           // Mutex para proteger el acceso a los datos compartidos
    sem_t semEntrada;                // Semáforo para la entrada
    sem_t semPlatoPrincipal;         // Semáforo para el plato principal
    sem_t semBebida;                 // Semáforo para la bebida
} Pedido;

// Función para preparar la entrada
void* prepararEntrada(void* arg) {
    Pedido* pedido = (Pedido*)arg;   // Convertir el argumento a tipo Pedido
    printf("Preparando entrada...\n"); // Mensaje indicando que se está preparando la entrada
    sleep(2);                        // Simular el tiempo de preparación de la entrada
    pthread_mutex_lock(&pedido->mutex); // Bloquear el mutex para acceso seguro a datos
    pedido->entradaLista = 1;         // Marcar la entrada como lista
    printf("Entrada lista.\n");      // Mensaje indicando que la entrada está lista
    pthread_mutex_unlock(&pedido->mutex); // Desbloquear el mutex
    sem_post(&pedido->semEntrada);   // Señalizar que la entrada está lista mediante el semáforo
    return NULL;                     // Terminar la función
}

// Función para preparar el plato principal
void* prepararPlatoPrincipal(void* arg) {
    Pedido* pedido = (Pedido*)arg;   // Convertir el argumento a tipo Pedido
    sem_wait(&pedido->semEntrada);   // Esperar a que la entrada esté lista
    printf("Preparando plato principal...\n"); // Mensaje indicando que se está preparando el plato principal
    sleep(3);                        // Simular el tiempo de preparación del plato principal
    pthread_mutex_lock(&pedido->mutex); // Bloquear el mutex para acceso seguro a datos
    pedido->platoPrincipalListo = 1;  // Marcar el plato principal como listo
    printf("Plato principal listo.\n"); // Mensaje indicando que el plato principal está listo
    pthread_mutex_unlock(&pedido->mutex); // Desbloquear el mutex
    sem_post(&pedido->semPlatoPrincipal); // Señalizar que el plato principal está listo mediante el semáforo
    return NULL;                     // Terminar la función
}

// Función para preparar la bebida
void* prepararBebida(void* arg) {
    Pedido* pedido = (Pedido*)arg;   // Convertir el argumento a tipo Pedido
    sem_wait(&pedido->semEntrada);   // Esperar a que la entrada esté lista
    printf("Preparando bebida...\n"); // Mensaje indicando que se está preparando la bebida
    sleep(1);                        // Simular el tiempo de preparación de la bebida
    pthread_mutex_lock(&pedido->mutex); // Bloquear el mutex para acceso seguro a datos
    pedido->bebidaLista = 1;         // Marcar la bebida como lista
    printf("Bebida lista.\n");       // Mensaje indicando que la bebida está lista
    pthread_mutex_unlock(&pedido->mutex); // Desbloquear el mutex
    sem_post(&pedido->semBebida);    // Señalizar que la bebida está lista mediante el semáforo
    return NULL;                     // Terminar la función
}

// Función que implementa el algoritmo Round Robin
void ejecutarRoundRobin(Pedido* pedido) {
    int tiempoRestanteEntrada = 2;   // Tiempo total para preparar entrada
    int tiempoRestantePlato = 3;     // Tiempo total para preparar plato
    int tiempoRestanteBebida = 1;     // Tiempo total para preparar bebida

    // Bucle de Round Robin
    while (tiempoRestanteEntrada > 0 || tiempoRestantePlato > 0 || tiempoRestanteBebida > 0) {
        // Preparar la entrada si aún queda tiempo
        if (tiempoRestanteEntrada > 0) {
            sleep(QUANTUM);            // Simular el quantum de tiempo
            tiempoRestanteEntrada -= QUANTUM; // Reducir el tiempo restante
            if (tiempoRestanteEntrada <= 0) { // Verificar si la entrada está lista
                pthread_mutex_lock(&pedido->mutex); // Bloquear el mutex
                pedido->entradaLista = 1; // Indicar que la entrada está lista
                pthread_mutex_unlock(&pedido->mutex); // Desbloquear el mutex
                sem_post(&pedido->semEntrada); // Señalizar que la entrada está lista
            }
        }

        // Preparar el plato principal si la entrada está lista
        if (tiempoRestantePlato > 0 && pedido->entradaLista) {
            sleep(QUANTUM);            // Simular el quantum de tiempo
            tiempoRestantePlato -= QUANTUM; // Reducir el tiempo restante
            if (tiempoRestantePlato <= 0) { // Verificar si el plato principal está listo
                pthread_mutex_lock(&pedido->mutex); // Bloquear el mutex
                pedido->platoPrincipalListo = 1; // Indicar que el plato principal está listo
                pthread_mutex_unlock(&pedido->mutex); // Desbloquear el mutex
                sem_post(&pedido->semPlatoPrincipal); // Señalizar que el plato principal está listo
            }
        }

        // Preparar la bebida si la entrada está lista
        if (tiempoRestanteBebida > 0 && pedido->entradaLista) {
            sleep(QUANTUM);            // Simular el quantum de tiempo
            tiempoRestanteBebida -= QUANTUM; // Reducir el tiempo restante
            if (tiempoRestanteBebida <= 0) { // Verificar si la bebida está lista
                pthread_mutex_lock(&pedido->mutex); // Bloquear el mutex
                pedido->bebidaLista = 1; // Indicar que la bebida está lista
                pthread_mutex_unlock(&pedido->mutex); // Desbloquear el mutex
                sem_post(&pedido->semBebida); // Señalizar que la bebida está lista
            }
        }
    }
}

// Proceso para gestionar el pedido
void gestionarPedido(Pedido* pedido) {
    // Ejecutar el algoritmo Round Robin
    ejecutarRoundRobin(pedido);

    // Esperar a que todos los componentes del pedido estén listos
    while (!(pedido->entradaLista && pedido->platoPrincipalListo && pedido->bebidaLista)) {
        sleep(1); // Esperar a que todos estén listos
    }
    printf("Todos los componentes del pedido están listos.\n"); // Mensaje final de componentes listos
    printf("Pedido completado.\n"); // Mensaje de pedido completado
}

int main() {
    key_t key = IPC_PRIVATE; // Crear una clave para la memoria compartida
    int shmid = shmget(key, sizeof(Pedido), IPC_CREAT | 0666); // Obtener ID de memoria compartida
    Pedido* pedido = shmat(shmid, NULL, 0); // Adjuntar memoria compartida

    // Inicializar los indicadores de estado
    pedido->entradaLista = 0;
    pedido->platoPrincipalListo = 0;
    pedido->bebidaLista = 0;

    // Inicializar el mutex
    pthread_mutexattr_t attr; 
    pthread_mutexattr_init(&attr); // Inicializar atributos del mutex
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED); // Permitir uso compartido entre procesos
    pthread_mutex_init(&pedido->mutex, &attr); // Inicializar el mutex

    // Inicializar los semáforos
    sem_init(&pedido->semEntrada, 1, 0); // Inicializar semáforo para la entrada
    sem_init(&pedido->semPlatoPrincipal, 1, 0); // Inicializar semáforo para el plato principal
    sem_init(&pedido->semBebida, 1, 0); // Inicializar semáforo para la bebida

    // Crear hilos para preparar cada componente del pedido
    pthread_t hEntrada, hPlatoPrincipal, hBebida;

    pthread_create(&hEntrada, NULL, prepararEntrada, pedido); // Crear hilo para la entrada
    pthread_create(&hPlatoPrincipal, NULL, prepararPlatoPrincipal, pedido); // Crear hilo para el plato principal
    pthread_create(&hBebida, NULL, prepararBebida, pedido); // Crear hilo para la bebida

    // Crear un proceso para gestionar el pedido
    pid_t pid = fork();
    if (pid == 0) { // Proceso hijo
        gestionarPedido(pedido); // Ejecutar gestión del pedido
        exit(0); // Terminar proceso hijo
    }

    // Esperar a que todos los hilos terminen
    pthread_join(hEntrada, NULL); // Esperar a que el hilo de entrada termine
    pthread_join(hPlatoPrincipal, NULL); // Esperar a que el hilo del plato principal termine
    pthread_join(hBebida, NULL); // Esperar a que el hilo de bebida termine

    // Esperar a que el proceso hijo termine
    wait(NULL); // Esperar a que el proceso hijo finalice

    // Limpieza de recursos
    pthread_mutex_destroy(&pedido->mutex); // Destruir el mutex
    sem_destroy(&pedido->semEntrada); // Destruir semáforo de entrada
    sem_destroy(&pedido->semPlatoPrincipal); // Destruir semáforo del plato principal
    sem_destroy(&pedido->semBebida); // Destruir semáforo de bebida

    // Limpiar memoria compartida
    shmdt(pedido); // Desvincular memoria compartida
    shmctl(shmid, IPC_RMID, NULL); // Marcar la memoria compartida para eliminación

    return 0; // Terminar el programa
}
