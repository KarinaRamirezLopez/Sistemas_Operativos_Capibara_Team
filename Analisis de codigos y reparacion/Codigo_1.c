#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

sem_t mutex; //se le quito un guion bajo //Se declara una variable de tipo semáforo llamada mutex. 
            //Este semáforo se utilizará para controlar el acceso a la sección crítica.

void* thread_func(void* arg) { //argumento arg es un puntero genérico (puede ser cualquier tipo de dato).
        
    sem_wait(&mutex); //intenta decrementar el semáforo. 
                      //Si el valor del semáforo es mayor que cero, se permite el acceso a la sección crítica. 
                      //Si el valor es cero, el hilo se bloquea hasta que el semáforo se libera.
    printf("Seccion critica del hilo\n");
    
    sem_post(&mutex);//incrementa el semáforo, permitiendo que otros hilos puedan acceder a la sección crítica.
    return NULL;
}

int main () { // se le agrego la m porque decia mai
    pthread_t thread; //se le agrego la e ya que que estaba mal escrito, almacenará la identificación del hilo creado.

    sem_init(&mutex, 0, 1);//Se inicializa el semáforo mutex.
                          //El segundo argumento 0 indica que el semáforo es compartido entre hilos dentro del mismo proceso. 
                          //El tercer argumento 1 establece el valor inicial del semáforo en 1, lo que permite que un hilo acceda a la sección crítica.
                          
    pthread_create(&thread, NULL, thread_func, NULL);
    
    sem_wait(&mutex); //La función principal también espera para acceder a la sección crítica, llamando a sem_wait.
    printf ("Seccion critica del hilo principal\n");
    
    sem_post(&mutex);//Libera el semáforo, permitiendo que otros hilos (en este caso, el hilo creado) accedan a la sección crítica.

    pthread_join(thread, NULL);
    
    sem_destroy(&mutex); //Destruye el semáforo mutex, liberando los recursos que ha estado utilizando.
    return 1;
    
}

//Este código implementa un ejemplo básico de concurrencia usando hilos y semáforos.
//Se crean un hilo y un hilo principal que intentan acceder a una sección crítica (código que debe ser ejecutado por un solo hilo a la vez).
//Se utiliza un semáforo (mutex) para garantizar que solo un hilo pueda acceder a la sección crítica en un momento dado, evitando condiciones de carrera. 
//El hilo principal espera a que el hilo creado termine su ejecución antes de destruir el semáforo y finalizar el programa.
