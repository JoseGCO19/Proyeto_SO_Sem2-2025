#ifndef MAIN_H
#define MAIN_H

// Colores ANSI
#define COLOR_ROJO     "\x1b[31m"
#define COLOR_VERDE    "\x1b[32m"
#define COLOR_AMARILLO "\x1b[33m"
#define COLOR_AZUL     "\x1b[34m"
#define COLOR_RESET    "\x1b[0m"

#include <semaphore.h>
#include <pthread.h>
#include <time.h>

#define N_DRONES_PR 3
#define CAP_ZONA_DESCARGA 2

typedef struct {
    int tipo_producto;
    struct timespec tiempo_inicio;

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

//contadores de resultados
extern int usos_plataforma;
extern int producto_estandar;
extern int producto_refrigerado;
extern int producto_ultra_procesado;

#endif