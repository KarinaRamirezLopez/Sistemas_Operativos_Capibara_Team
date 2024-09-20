#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Estructura para pasar argumentos a las funciones
typedef struct {
    int id;
    int value;
} ThreadArgs;

// Función 1: Suma
void* sumar(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    int result = args->value + 10; // Suma 10
    printf("Hilo %d: Suma = %d\n", args->id, result);
    return NULL;
}

// Función 2: Multiplicación
void* multiplicar(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    int result = args->value * 5; // Multiplica por 5
    printf("Hilo %d: Multiplicación = %d\n", args->id, result);
    return NULL;
}

// Función 3: Factorial
void* factorial(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    int result = 1;
    for (int i = 1; i <= args->value; i++) {
        result *= i; // Calcula el factorial
    }
    printf("Hilo %d: Factorial de %d = %d\n", args->id, args->value, result);
    return NULL;
}

// Función 4: Generar número aleatorio
void* generar_random(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    srand(time(NULL) + args->id); // Semilla diferente para cada hilo
    int random_num = rand() % 100; // Número aleatorio entre 0 y 99
    printf("Hilo %d: Número aleatorio = %d\n", args->id, random_num);
    return NULL;
}

// Función 5: Dormir un tiempo
void* dormir(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    printf("Hilo %d: Durmiendo 2 segundos...\n", args->id);
    sleep(2); // Duerme 2 segundos
    printf("Hilo %d: Despertó\n", args->id);
    return NULL;
}

int main() {
    pthread_t hilos[5];
    ThreadArgs args[5];

    // Crear hilos con argumentos
    for (int i = 0; i < 5; i++) {
        args[i].id = i + 1;
        args[i].value = i + 1; // Valor diferente para cada hilo
        switch (i) {
            case 0: pthread_create(&hilos[i], NULL, sumar, &args[i]); break;
            case 1: pthread_create(&hilos[i], NULL, multiplicar, &args[i]); break;
            case 2: pthread_create(&hilos[i], NULL, factorial, &args[i]); break;
            case 3: pthread_create(&hilos[i], NULL, generar_random, &args[i]); break;
            case 4: pthread_create(&hilos[i], NULL, dormir, &args[i]); break;
        }
    }

    // Esperar a que los hilos terminen
    for (int i = 0; i < 5; i++) {
        pthread_join(hilos[i], NULL);
    }

    printf("Todos los hilos han terminado.\n");
    return 0;
}