#ifndef MAIN_H
#define MAIN_H

// Colores ANSI
#define COLOR_ROJO     "\x1b[31m"
#define COLOR_VERDE    "\x1b[32m"
#define COLOR_AMARILLO "\x1b[33m"
#define COLOR_AZUL     "\x1b[34m"
#define COLOR_RESET    "\x1b[0m"
#define COLOR_NARANJA  "\033[38;5;214m"
#define COLOR_MORADO   "\033[38;5;129m" 

#include <semaphore.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define N_DRONES_PR 10
#define CAP_ZONA_DESCARGA 10
#define TOTAL_DEPOSITOS 8
#define M_DONES_CARGA 4
#define BRAZOS 3

typedef struct {
    int tipo_producto;              //0: Tipo Estandar; 1:Tipo Refrigerado; 2:Tipo Ultra-delicado
    struct timespec tiempo_inicio;  //Lleva el tiempo que el producto desde que entro en el programa

} Producto;

extern void* agente_desinfeccion(void* arg);
extern void* dron_recolector(void* arg);
extern void* dron_carga(void *arg);
extern void* operario_almacen(void *arg);
extern void* brazo_clasificado(void *arg);
extern void inicializar_hilos();
extern void inicializar_sem();

//FUNCIONES DEL MENU
extern void menu_principal();
extern void iniciar_simulacion_segundos(int segundos);
extern void reiniciar_estado();
extern void destruir_sem();
extern void mostrar_resultados();

//VARIABLE PARA CONTROLAR EJECUCIÓN
extern int sistema_activo;  // 1 = ejecutar, 0 = detener

//Hilos e ID's
extern pthread_t drones[N_DRONES_PR];               //definicion de un array de 25 drones (hilos)
extern int ids_drones[N_DRONES_PR];                 //ID's de los Drones de Recoleccion
extern pthread_t drones_carga[M_DONES_CARGA];
extern int ids_drones_carga[M_DONES_CARGA];
extern pthread_t brazo[BRAZOS];
extern int ids_brazo[BRAZOS];
extern pthread_t operador_almacen;
extern pthread_t hilo_agente;

// Declaración de variables globales (EXPORT)

//VARIABLE PARA DRONES_RECOLECTORES
extern sem_t sem_cap_recoleccion;                   //Limita la entrada al centro
extern pthread_mutex_t sem_sala_desinfeccion;       //Mutex para la entrada a la sala de desinfeccion
extern sem_t sem_agente_des;                        //Despierta al agente de desinfeccion
extern sem_t sem_fin_des;                           //Semaforo donde el Dron recolector espera hasta ser desinfectado
extern sem_t sem_espacios_vacios;                   //Limita la entrada a la zona de descarga
extern sem_t sem_elementos_disp;                    //Llama al proceso del brazo. Indica que dejo elementos en la zona de descarga
extern pthread_mutex_t mutex_buffer;                //permite la modificacion en la estructura que almacena los elementos de la zona de descarga
extern Producto buffer_descarga[CAP_ZONA_DESCARGA]; //[Zona de descarga], buffer donde se almacenaran los productos
extern pthread_mutex_t mutex_contador_resultado;
extern int indice_productor;                         //lleva el conteo de cuantos productos hay
extern int indice_consumidor;


//VARIABLES PARA EL DEPOSITO
extern int deposito[TOTAL_DEPOSITOS];               //Vector que almacena la cantidad de cajas por deposito. 0-3:Estandar; 4-6:Refrigerado; 7:Ultra-Procesado
extern int indice_deposito_estandar;                //Lleva el indice del vector deposito en la seccion de productos Estandar
extern int indice_deposito_refrigerado;             //Lleva el indice del vector deposito en la seccion de productos Refrigerados
extern pthread_mutex_t mutex_standar; 
extern pthread_mutex_t mutex_refri; 
extern char tipo_producto_str[3][20];               //Vector de strings para imprimir el tipo de producto en texto
extern int frecuencias_pr[4][12];                   //Frecuencias de generación de productos para cada tipo
extern int selector_frecuencia;                    //Índice para seleccionar la frecuencia de generación de productos

//contadores de resultados
extern int usos_plataforma;                         //Variable que cuenta los usos que tuvo la plataforma magnetica a lo largo del programa
extern int producto_estandar;                       //Variable que lleva la cuenta de cuantos productos de tipo Estandar se proceso
extern int producto_refrigerado;                    //Variable que lleva la cuenta de cuantos productos de tipo Refrigerado se proceso
extern int producto_ultra_delicado;                //Variable que lleva la cuenta de cuantos productos de tipo Ultra-Procesado se proceso
//mutex_deposito
//VARIABLES NECESARIAS PARA EL PROCESO BRAZO_RECOLECTOR
extern pthread_mutex_t mutex_buffer_descarga;
extern sem_t sem_drones_carga; // inicializar en 4
extern pthread_mutex_t mutex_metricas; 
extern int bloqueos_evitados;
extern pthread_mutex_t mutex_buzon;
extern int buzon_id_brazo;
extern sem_t sem_iniciar_viaje_dron; 
extern sem_t sem_fin_viaje_brazo[BRAZOS];
extern sem_t sem_plataforma_levitacion;
extern pthread_mutex_t mutex_metricas_levitacion;
extern double tiempo_total_acum;
extern int productos_procesados;

//Semaforos de depositos
extern sem_t deposito_vaciado[TOTAL_DEPOSITOS];
extern sem_t deposito_libre[TOTAL_DEPOSITOS];
extern pthread_mutex_t mutex_almacen;
extern sem_t sem_llamar_operario;
extern pthread_mutex_t mutex_dronCarga;
extern pthread_mutex_t mutex_deposito;




#endif