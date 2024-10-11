#include <stdio.h>
#include <pthread.h>

// Variable global que representa el saldo
int saldo = 0;

// Declaración del candado para sincronización
pthread_mutex_t lock;

// Función para realizar un retiro
void* retirar(void* arg) {
    int monto = *(int*)arg;

    // Bloquear el acceso a la sección crítica
    pthread_mutex_lock(&lock);

    if (saldo >= monto) {
        printf("Retirando %d...\n", monto);
        saldo -= monto;
        printf("Retiro completado. Saldo actual: %d\n", saldo);
    } else {
        printf("Saldo insuficiente para retirar %d. Saldo actual: %d\n", monto, saldo);
    }

    // Desbloquear el acceso
    pthread_mutex_unlock(&lock);

    return NULL;
}

// Función para realizar un depósito
void* depositar(void* arg) {
    int monto = *(int*)arg;

    // Bloquear el acceso a la sección crítica
    pthread_mutex_lock(&lock);

    printf("Depositando %d...\n", monto);
    saldo += monto;
    printf("Depósito completado. Saldo actual: %d\n", saldo);

    // Desbloquear el acceso
    pthread_mutex_unlock(&lock);

    return NULL;
}

int main() {
    pthread_t hilo_retiro, hilo_deposito;
    int opcion, monto;

    // Inicializar el candado
    pthread_mutex_init(&lock, NULL);

    do {
        // Mostrar menú
        printf("\n--- Menú del Banco ---\n");
        printf("1. Retirar dinero\n");
        printf("2. Depositar dinero\n");
        printf("3. Salir\n");
        printf("Elija una opción: ");
        scanf("%d", &opcion);

        if (opcion == 1) {
            // Solicitar monto para retirar
            printf("Ingrese el monto a retirar: ");
            scanf("%d", &monto);

            // Crear hilo para retiro
            pthread_create(&hilo_retiro, NULL, retirar, &monto);
            pthread_join(hilo_retiro, NULL);  // Esperar a que el hilo termine

        } else if (opcion == 2) {
            // Solicitar monto para depositar
            printf("Ingrese el monto a depositar: ");
            scanf("%d", &monto);

            // Crear hilo para depósito
            pthread_create(&hilo_deposito, NULL, depositar, &monto);
            pthread_join(hilo_deposito, NULL);  // Esperar a que el hilo termine
        }

    } while (opcion != 3);

    // Destruir el candado
    pthread_mutex_destroy(&lock);

    printf("Operaciones finalizadas. Saldo final: %d\n", saldo);

    return 0;
}

