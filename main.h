#ifndef MAIN_H
#define MAIN_H

#include <semaphore.h>
#include <pthread.h>

#define N_DRONES_PR 25
#define CAP_ZONA_DESCARGA 10

typedef struct {
    int tipo_producto;
} Producto;

// Declaración de variables globales (EXPORT)
extern sem_t sem_cap_recoleccion;
extern pthread_mutex_t sem_sala_desinfeccion;
extern sem_t sem_agente_des;
extern sem_t sem_fin_des;
extern sem_t sem_espacios_vacios;
extern sem_t sem_elementos_disp;
extern pthread_mutex_t mutex_buffer;
extern Producto buffer_descarga[CAP_ZONA_DESCARGA];
extern int indice_producto;
extern pthread_t drones[N_DRONES_PR];
extern int ids_drones[N_DRONES_PR];

#endif