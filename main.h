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
#include<unistd.h>
#include<stdio.h>

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
extern pthread_mutex_t mutex_buffer_descarga;
extern sem_t sem_drones_carga; // inicializar en 1
extern pthread_mutex_t mutex_metricas; 
extern int bloqueos_evitados;
extern pthread_mutex_t mutex_buzon;
extern int buzon_id_brazo;
extern sem_t sem_iniciar_viaje_dron; 
extern sem_t sem_fin_viaje_brazo[3];
extern sem_t sem_plataforma_levitacion;
extern pthread_mutex_t mutex_metricas_levitacion;
extern int uso_plataforma_levitacion;
extern double tiempo_total_acum;
extern int productos_procesados;



#endif