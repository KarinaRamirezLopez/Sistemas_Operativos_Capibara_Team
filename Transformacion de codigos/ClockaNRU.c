#include <stdio.h>

#define MAX_MESAS 5  // Número máximo de páginas (mesas)

// Estructura para representar una página (mesa)
typedef struct {
    int personaje;    // Número de página (personaje)
    int referencia;   // Bit de referencia (0 o 1)
    int modificacion; // Bit de modificación (0 o 1)
} Mesa;

// Función para mostrar el estado actual de las mesas
void mostrar_mesas(Mesa mesas[], int num_mesas) {
    printf("Estado de las mesas (páginas):\n");
    for (int i = 0; i < num_mesas; i++) {
        printf("Mesa %d: Persona %d | Referencia: %d | Modificación: %d\n", i + 1, mesas[i].personaje, mesas[i].referencia, mesas[i].modificacion);
    }
}

// Función para buscar si un personaje ya está en una mesa
int buscar_personaje(Mesa mesas[], int num_mesas, int personaje) {
    for (int i = 0; i < num_mesas; i++) {
        if (mesas[i].personaje == personaje) {
            return i; // Personaje encontrado
        }
    }
    return -1; // Personaje no encontrado
}

// Función para determinar la categoría de una mesa en NRU
int obtener_categoria(Mesa mesa) {
    if (mesa.referencia == 0 && mesa.modificacion == 0) return 1;
    if (mesa.referencia == 0 && mesa.modificacion == 1) return 2;
    if (mesa.referencia == 1 && mesa.modificacion == 0) return 3;
    if (mesa.referencia == 1 && mesa.modificacion == 1) return 4;
    return 5; // No debería llegar aquí
}

// Función para encontrar la página a reemplazar según NRU
int encontrar_pagina_para_reemplazo(Mesa mesas[], int num_mesas) {
    int indice_para_reemplazar = -1;
    int categoria_minima = 5;  // La categoría más baja es 1 (referencia=0, modificación=0)
    
    for (int i = 0; i < num_mesas; i++) {
        int categoria = obtener_categoria(mesas[i]);
        if (categoria < categoria_minima) {
            categoria_minima = categoria;
            indice_para_reemplazar = i;
        }
    }
    
    return indice_para_reemplazar;
}

// Función para actualizar los bits de referencia (simulación de paso de tiempo)
void actualizar_bits(Mesa mesas[], int num_mesas) {
    // Simulación: Reseteo de algunos bits de referencia, como lo haría el SO en un ciclo de reloj
    for (int i = 0; i < num_mesas; i++) {
        if (mesas[i].referencia == 1) {
            mesas[i].referencia = 0; // Reseteo del bit de referencia
        }
    }
}

// Función principal que simula el algoritmo NRU
void fiesta_en_la_disco(int personajes[], int num_personajes) {
    Mesa mesas[MAX_MESAS];  // Mesas de la disco (páginas)
    int num_mesas = 0;      // Número de personajes sentados actualmente

    // Inicializamos las mesas
    for (int i = 0; i < MAX_MESAS; i++) {
        mesas[i].personaje = -1; // No hay personaje en la mesa
        mesas[i].referencia = 0;
        mesas[i].modificacion = 0;
    }

    for (int i = 0; i < num_personajes; i++) {
        int personaje = personajes[i];
        printf("\nPersona %d llega a la disco.\n", personaje);

        // Verificamos si el personaje ya está en la disco
        int pos = buscar_personaje(mesas, num_mesas, personaje);
        if (pos != -1) {
            // El personaje ya está dentro, actualizamos su bit de referencia
            printf("Persona %d ya está en la disco, se actualiza su bit de referencia.\n", personaje);
            mesas[pos].referencia = 1;
        } else {
            // El personaje no está en la disco
            if (num_mesas < MAX_MESAS) {
                // Hay espacio, el personaje entra directamente
                printf("Persona %d encuentra una mesa libre\n", personaje);
                mesas[num_mesas].personaje = personaje;
                mesas[num_mesas].referencia = 1;   // Se marca la referencia del nuevo personaje
                mesas[num_mesas].modificacion = 0; // Supongamos que al principio no está modificada
                num_mesas++;
            } else {
                // No hay espacio, necesitamos reemplazar una página
                int reemplazo = encontrar_pagina_para_reemplazo(mesas, MAX_MESAS);
                printf("Persona %d reemplaza a Persona %d en la mesa %d\n", personaje, mesas[reemplazo].personaje, reemplazo + 1);
                mesas[reemplazo].personaje = personaje;
                mesas[reemplazo].referencia = 1;   // Nueva referencia
                mesas[reemplazo].modificacion = 0; // Nueva página sin modificar
            }
        }

        // Mostrar estado actual de las mesas
        mostrar_mesas(mesas, MAX_MESAS);

        // Actualizamos los bits de referencia (simulación del paso de tiempo)
        actualizar_bits(mesas, MAX_MESAS);
    }
}

int main() {
    int personajes[] = {1, 2, 3, 4, 1, 5, 2, 6, 1, 2};  // Secuencia de personajes que llegan a la disco
    int num_personajes = sizeof(personajes) / sizeof(personajes[0]);

    printf("¡Bienvenidos a la Disco (Simulación NRU)!\n");
    fiesta_en_la_disco(personajes, num_personajes);

    return 0;
}


