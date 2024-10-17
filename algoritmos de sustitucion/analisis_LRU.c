//Capibara Team
//LRU (Menos Recientemente Utilizado)
#include <stdio.h>  // Librería para funciones de entrada y salida
#include <stdlib.h>  // Librería para funciones de asignación de memoria dinámica
#include <stdbool.h>  // Librería para usar valores booleanos

#define NUM_FRAMES 4   // Definir el número de frames (páginas físicas en memoria)
#define NUM_PAGES 10   // Definir el número total de páginas virtuales

// Estructura para un frame de página en memoria física
typedef struct Frame {
    int page;           // Número de página almacenada en el frame (valor -1 si está vacío)
    bool valid;         // Indica si el frame está ocupado (true) o vacío (false)
    struct Frame *prev; // Puntero al frame previo (para lista doblemente enlazada)
    struct Frame *next; // Puntero al frame siguiente (para lista doblemente enlazada)
} Frame;

// Estructura para la lista de frames en memoria física
typedef struct FrameList {
    int numFrames;      // Número de frames actualmente ocupados
    Frame *head;        // Puntero al primer frame de la lista
    Frame *tail;        // Puntero al último frame de la lista
} FrameList;

// Función para crear un nuevo frame
Frame* createFrame() {
    Frame *frame = (Frame *)malloc(sizeof(Frame));  // Reservar memoria para el frame
    if (frame != NULL) {
        frame->page = -1;   // Inicialmente no hay página asignada
        frame->valid = false;  // El frame está vacío
        frame->prev = NULL;  // No tiene frame anterior
        frame->next = NULL;  // No tiene frame siguiente
    }
    return frame;  // Devolver el frame creado
}

// Función para inicializar la lista de frames en memoria física
FrameList* createFrameList() {
    FrameList *frameList = (FrameList *)malloc(sizeof(FrameList));  // Reservar memoria para la lista de frames
    if (frameList != NULL) {
        frameList->numFrames = 0;  // Inicialmente no hay frames ocupados
        frameList->head = NULL;  // No hay frames en la lista
        frameList->tail = NULL;  // No hay frames en la lista
    }
    return frameList;  // Devolver la lista de frames creada
}

// Función para insertar un frame al frente de la lista (más recientemente usado)
void insertFrame(FrameList *frameList, Frame *frame) {
    if (frameList->head == NULL) {
        // Si la lista está vacía
        frameList->head = frame;  // El nuevo frame es el primero
        frameList->tail = frame;  // También es el último
    } else {
        // Insertar al frente de la lista
        frame->next = frameList->head;  // El siguiente del nuevo frame es el antiguo primero
        frameList->head->prev = frame;  // El antiguo primero ahora tiene como anterior al nuevo frame
        frameList->head = frame;  // El nuevo frame es ahora el primero de la lista
    }
    frameList->numFrames++;  // Incrementar el número de frames ocupados
}

// Función para eliminar un frame de la lista (menos recientemente usado)
void removeFrame(FrameList *frameList, Frame *frame) {
    if (frame->prev != NULL) {
        frame->prev->next = frame->next;  // El siguiente del anterior del frame eliminado apunta al siguiente del frame eliminado
    } else {
        frameList->head = frame->next;  // Si no tiene anterior, el siguiente es el nuevo primero
    }
    if (frame->next != NULL) {
        frame->next->prev = frame->prev;  // El anterior del siguiente del frame eliminado apunta al anterior del frame eliminado
    } else {
        frameList->tail = frame->prev;  // Si no tiene siguiente, el anterior es el nuevo último
    }
    frameList->numFrames--;  // Decrementar el número de frames ocupados
    free(frame);  // Liberar la memoria del frame eliminado
}

// Función para buscar un frame específico por número de página
Frame* findFrame(FrameList *frameList, int page) {
    Frame *current = frameList->head;  // Iniciar desde el primer frame
    while (current != NULL) {
        if (current->page == page) {  // Si se encuentra el frame con la página solicitada
            return current;  // Devolver el frame encontrado
        }
        current = current->next;  // Avanzar al siguiente frame
    }
    return NULL;  // Si no se encuentra, devolver NULL
}

// Función para simular la carga de una página a memoria física
void loadPage(FrameList *frameList, int page) {
    Frame *frame = createFrame();  // Crear un nuevo frame
    frame->page = page;  // Asignar la página al frame
    frame->valid = true;  // Marcar el frame como ocupado

    // Si la lista de frames ya está llena, reemplazar la página menos recientemente usada (LRU)
    if (frameList->numFrames == NUM_FRAMES) {
        Frame *lruFrame = frameList->tail;  // El último frame es el menos recientemente usado
        removeFrame(frameList, lruFrame);  // Remover el frame menos recientemente usado
    }

    insertFrame(frameList, frame);  // Insertar el nuevo frame al frente de la lista
}

// Función para imprimir el estado actual de la lista de frames (solo para fines de depuración)
void printFrameList(FrameList *frameList) {
    printf("Estado actual de la lista de frames:\n");
    Frame *current = frameList->head;
    while (current != NULL) {
        printf("Página: %d, ", current->page);
        if (current->valid) {
            printf("Estado: Ocupado\n");
        } else {
            printf("Estado: Vacío\n");
        }
        current = current->next;  // Avanzar al siguiente frame
    }
    printf("\n");
}

int main() {
    FrameList *frameList = createFrameList();  // Crear la lista de frames

    // Simular la carga de varias páginas a memoria física
    loadPage(frameList, 1);  // Cargar página 1
    loadPage(frameList, 2);  // Cargar página 2
    loadPage(frameList, 3);  // Cargar página 3
    loadPage(frameList, 4);  // Cargar página 4
    printFrameList(frameList);  // Imprimir el estado actual de los frames

    // Intentar cargar otra página cuando todos los frames están ocupados
    loadPage(frameList, 5);  // Cargar página 5 (reemplazar la menos recientemente usada)
    printFrameList(frameList);  // Imprimir el estado actual después de la sustitución

    // Liberar la memoria utilizada por la lista de frames
    free(frameList);

    return 0;  // Fin del programa
}
