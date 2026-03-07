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
#define TOTAL_DEPOSITOS 7

typedef struct {
    int tipo_producto;              //0: Tipo Estandar; 1:Tipo Refrigerado; 2:Tipo Ultra-Procesado
    struct timespec tiempo_inicio;  //Lleva el tiempo que el producto desde que entro en el programa

} Producto;

// Declaración de variables globales (EXPORT)
extern sem_t sem_cap_recoleccion;                   //Limita la entrada al centro
extern pthread_mutex_t sem_sala_desinfeccion;       //Mutex para la entrada a la sala de desinfeccion
extern sem_t sem_agente_des;                        //Despierta al agente de desinfeccion
extern sem_t sem_fin_des;                           //Semaforo donde el Dron recolector espera hasta ser desinfectado
extern sem_t sem_espacios_vacios;                   //Limita la entrada a la zona de descarga
extern sem_t sem_elementos_disp;                    //Llama al proceso del brazo. Indica que dejo elementos en la zona de descarga
extern sem_t sem_plataforma;                        //Semaforo que limita el uso de la PLataforma Magnetica

extern int deposito[TOTAL_DEPOSITOS];               //Vector que almacena la cantidad de cajas por deposito. 0-3:Estandar; 4-6:Refrigerado; 7:Ultra-Procesado
extern int indice_deposito_estandar;                //Lleva el indice del vector deposito en la seccion de productos Estandar
extern int indice_deposito_refrigerado;             //Lleva el indice del vector deposito en la seccion de productos Refrigerados

extern pthread_mutex_t mutex_buffer;                //permite la modificacion en la estructura que almacena los elementos de la zona de descarga
extern Producto buffer_descarga[CAP_ZONA_DESCARGA]; //[Zona de descarga], buffer donde se almacenaran los productos
extern int indice_producto;                         //lleva el conteo de cuantos productos hay
extern pthread_t drones[N_DRONES_PR];               //Hilos de los drones
extern int ids_drones[N_DRONES_PR];                 //ID's de los Drones de Recoleccion

//contadores de resultados
extern int usos_plataforma;                         //Variable que cuenta los usos que tuvo la plataforma magnetica a lo largo del programa
extern int producto_estandar;                       //Variable que lleva la cuenta de cuantos productos de tipo Estandar se proceso
extern int producto_refrigerado;                    //Variable que lleva la cuenta de cuantos productos de tipo Refrigerado se proceso
extern int producto_ultra_procesado;                //Variable que lleva la cuenta de cuantos productos de tipo Ultra-Procesado se proceso

#endif