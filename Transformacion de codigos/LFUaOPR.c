#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NUM_FRAMES 4 // Número de frames (páginas físicas en memoria)

typedef struct Frame {
    int page; // Número de página almacenada en el frame (valor -1 si está vacío)
    bool valid; // Indica si el frame está ocupado (true) o vacío (false)
    struct Frame *prev; // Puntero al frame previo (para lista doblemente enlazada)
    struct Frame *next; // Puntero al frame siguiente (para lista doblemente enlazada)
} Frame;

typedef struct FrameList {
    int numFrames; // Número de frames actualmente ocupados
    Frame *head; // Puntero al primer frame de la lista
    Frame *tail; // Puntero al último frame de la lista
} FrameList;

// Función para crear un nuevo frame y asignar memoria
Frame* createFrame() {
    Frame *frame = (Frame *)malloc(sizeof(Frame));
    if (frame != NULL) {
        frame->page = -1; // Inicializa la página como vacía
        frame->valid = false; // Marca el frame como no válido
        frame->prev = NULL;
        frame->next = NULL;
    }
    return frame;
}

// Función para crear una lista de frames
FrameList* createFrameList() {
    FrameList *frameList = (FrameList *)malloc(sizeof(FrameList));
    if (frameList != NULL) {
        frameList->numFrames = 0; // Inicialmente no hay frames ocupados
        frameList->head = NULL; // La lista está vacía
        frameList->tail = NULL; // La lista está vacía
    }
    return frameList;
}

// Mueve un frame al frente de la lista (para mantener el orden LRU)
void moveToFront(FrameList *frameList, Frame *frame) {
    if (frame == frameList->head) {
        return; // Si ya está en la cabeza, no hace nada
    }

    // Desconecta el frame de su posición actual
    if (frame->prev != NULL) {
        frame->prev->next = frame->next;
    } else {
        frameList->tail = frame->next;
    }

    if (frame->next != NULL) {
        frame->next->prev = frame->prev;
    }

    // Coloca el frame al frente de la lista
    frame->next = frameList->head;
    frame->prev = NULL;

    if (frameList->head != NULL) {
        frameList->head->prev = frame;
    }

    frameList->head = frame;
    if (frameList->tail == NULL) {
        frameList->tail = frame;
    }
}

// Elimina el frame LRU (el último en la lista)
void removeLRUFrame(FrameList *frameList) {
    if (frameList->tail != NULL) {
        Frame *lruFrame = frameList->tail;
        if (lruFrame->prev != NULL) {
            frameList->tail = lruFrame->prev;
            frameList->tail->next = NULL; // Desconecta el último frame
        } else {
            frameList->head = NULL; // La lista queda vacía
            frameList->tail = NULL;
        }
        free(lruFrame); // Libera la memoria del frame eliminado
        frameList->numFrames--; // Decrementa el contador de frames
    }
}

// Carga una nueva página en la lista de frames
void loadPage(FrameList *frameList, int page) {
    Frame *existingFrame = frameList->head;

    // Busca si la página ya está en los frames
    while (existingFrame != NULL) {
        if (existingFrame->page == page) {
            // Si la página ya está, la mueve al frente (para LRU)
            moveToFront(frameList, existingFrame);
            return;
        }
        existingFrame = existingFrame->next;
    }

    // Si llegamos aquí, es porque la página no está en memoria, y necesitamos cargarla
    Frame *newFrame = createFrame();
    newFrame->page = page; // Asigna la nueva página al frame
    newFrame->valid = true; // Marca el frame como válido

    // Si los frames están llenos, elimina el frame LRU (el último)
    if (frameList->numFrames == NUM_FRAMES) {
        removeLRUFrame(frameList);
    }

    // Añade el nuevo frame al frente de la lista
    moveToFront(frameList, newFrame);
    frameList->numFrames++; // Incrementa el contador de frames

    // Imprimir el estado de los frames tras cada reemplazo
    printf("Reemplazo de página realizado. Estado actual de la lista de frames:\n");
    Frame *current = frameList->head;
    while (current != NULL) {
        printf("Página: %d, Estado: Ocupado\n", current->page);
        current = current->next;
    }
    printf("\n");
}

// Libera la memoria de la lista de frames
void freeFrameList(FrameList *frameList) {
    Frame *current = frameList->head;
    while (current != NULL) {
        Frame *next = current->next;
        free(current); // Libera la memoria de cada frame
        current = next;
    }
    free(frameList); // Libera la memoria de la lista de frames
}

int main() {
    // Crea una lista de frames
    FrameList *frameList = createFrameList();

    // Secuencia de páginas a cargar
    int pages[] = {1, 2, 3, 4, 5, 2, 1, 6, 7};
    int numPages = sizeof(pages) / sizeof(pages[0]);

    // Carga cada página en la lista de frames
    for (int i = 0; i < numPages; i++) {
        loadPage(frameList, pages[i]);
    }

    // Libera la memoria de la lista al final
    freeFrameList(frameList);
    return 0;
}