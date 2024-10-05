//Codigo 3
//Analizado y reparado por: Capibara Team 
//Grupo: 4CV4

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

pthread_mutex_t mutex; //Declara mutex para los hilos, evita condiciones de carrera cuando hay varios hilos
int saldo = 0; //Inicializa la variable de "saldo"
 
 void * realizarRetiro(void* arg) { //Función para realizar un retiro 
    int monto = (int) arg; //Indica el monto a retirar 
    pthread_mutex_lock(&mutex);
    //Bloquea el mutex antes de acceder a la variable "saldo"
    //Garantizando que un solo hilo pueda modificar a la vez a "saldo"
    
    printf("Realizando retiro de %d\n", monto); //Imprime el monto que se esta retirando
    saldo -= monto; //Se resta el monto al saldo actual
    printf("Saldo restante: %d\n", saldo); //Imprime el saldo restante 
    pthread_mutex_unlock(&mutex); 
    //Desbloquea el mutx, permitiendo que otros hilos accedan a "saldo"
    pthread_exit(NULL); //Finaliza el hilo
}

void* realizarDeposito(void* arg){ //Funcion de realizar depositos 
    int monto = (int) arg; //Valor del monto a depositar 
    pthread_mutex_lock(&mutex); //Bloquea al mutex para evitar conflictos con "saldo"
    printf("Realizando deposito de %d\n", monto);//Monto de saldo a depositar 
    saldo += monto;//Incrementa el saldo con el monto depositado
    printf("Saldo actual: %d\n", saldo); //Imprime el saldo actualizado
    pthread_mutex_unlock(&mutex); //Desbloquea el mutex 
    pthread_exit(NULL); //Finaliza el hilo 
}

int main() {
    pthread_t hiloRetiro, hiloDeposito; //Declara hilos
    int montoRetiro = 100; //Declara e inicializa montoRetiro 
    int montoDeposito = 200; //Declara e inicializa montoDeposito
    pthread_mutex_init(&mutex, NULL); //Inicializa el mutex con valores por defecto 
    pthread_create(&hiloRetiro, NULL, realizarRetiro, &montoRetiro); 
    //Se crea un hilo que ejecuta la funcion de "realizarRetiro", usando montoRetiro como argumento   
    
    pthread_create(&hiloDeposito, NULL, realizarDeposito, &montoDeposito);
    //Se crea un hilo que ejecuta la funcion de "realizarDeposito" usando montoDeposito como argumento 
    
    //Espera a que los hilos terminen su ejecución 
    pthread_join(hiloRetiro, NULL);
    pthread_join(hiloDeposito, NULL);
    
    pthread_mutex_destroy(&mutex);//Destruye el mutex
    return 0; //Fin:)
    
    //Podemos decir que el uso del mutex asegura que "saldo" no sea modificado simultaneamente por ambos 
    //hilos, sin el mutex provocaria que los hilos manejen "saldo" al mismo tiempo, lo que nos daria resultados erroneos
}