#include<stdio.h> //libreria para funciones estandares
#include<semaphore.h> //para la inclusion de semaforos
#include<pthread.h> //libreria que proporciona herramientas para manejar hilos , standar POSIX
#include<unistd.h> //para funciones de interaccion con el sistema operativo, ejemplo sleep (para simular tiempos de espera )
#include<time.h>

#include "main.h"

//Prototipos

/*
//VARIABLES PARA EL PROCESO DRONES_RECOLECTORES
Producto buffer_descarga[CAP_ZONA_DESCARGA]; //buffer 
int indice_producto = 0;                    //para desplazarse a traves del buffer 
sem_t sem_cap_recoleccion;                  //semaforo contador de dron recolector (25 disponibles)
pthread_mutex_t sem_sala_desinfeccion;      //semaforo para controlar la espera a la sala de desinfeccion , inicializado en 1
sem_t sem_agente_des;                       //semaforo para el agente de desifeccion inicializado en 0, dado que hay un solo agente
                                           //el agente de desinfeccion debe esperar a que un dron lo llame
sem_t sem_fin_des;                          //semaforo para validar, que el agente ya proceso el id y bateria del semaforo, inicializado en 0
//semaforos para la zona de carga
sem_t sem_espacios_vacios;                  //semafor para el buffer , inicializado en CAP_ZONA_DESCARGA
sem_t sem_elementos_disp;                   // inicializado en 0
pthread_mutex_t mutex_buffer;               //iniciarlizar en 1
pthread_mutex_t mutex_buffer_descarga; //este sera para cuando un brazo, intente sacar un producto del buffer(almacen)
sem_t sem_drones_carga; //inicializar en 1
pthread_mutex_t mutex_metricas; //mutex para el acceso a la variable de control de bloqueos evitados
extern int bloqueos_evitados=0; //para contabilizar los bloqueos evitados
pthread_mutex_t mutex_buzon; 
int buzon_id_brazo;
sem_t sem_iniciar_viaje_dron; //inicializado en 0 (despierta a los drones de carga)
sem_t sem_fin_viaje_brazo[BRAZOS]; //arreglo de 3 semaforos inicializados en 0( 1 para cada brazo)
sem_t sem_plataforma_levitacion; //inicializada en 1
pthread_mutex_t mutex_metricas_levitacion; //paara acceder a la variables uso de plataforma de levitacion 
int usos_plataforma=0;
int productos_procesados=0; //productos totales procesados
const char tipo_producto_str[3][20] = {"Estándar", "Refrigerado", "Ultradelicado"}; //para imprimir el tipo de producto en texto

//Cosas Añadidas por Contin
int ids_brazos[BRAZOS];             //Para almacenar los id de todos los brazos
pthread_mutex_t mutex_dronCarga;    //semaforo que controla que los drones de carga se asignen a un sólo brazo
sem_t sem_llamar_operario;          //Semaforo que llama al operario para vaciar los depositos
pthread_mutex_t mutex_almacen;      //Semaforo que controla el acceso al almacen
int deposito[TOTAL_DEPOSITOS];      //Vector de los depositos
sem_t deposito_libre[TOTAL_DEPOSITOS]; 
sem_t deposito_vaciado[TOTAL_DEPOSITOS];
int ids_drones_carga[M_DONES_CARGA];       //Vector de los ID's de los drones de carga
pthread_t drones_carga[M_DONES_CARGA];       //definicion de un array de 4 drones de carga (hilos)
pthread_t operador_almacen;                  //definicion de el hilo del operador del almacen (hilos)
pthread_t brazo[BRAZOS];                     //definicion de un array de los 3 brazos mecanicos (hilos)
int ids_brazo[BRAZOS];                      //Array del Id de cada brazo

//VARIABLES PARA EL DEPOSITO
int deposito[TOTAL_DEPOSITOS];               //Vector que almacena la cantidad de cajas por deposito. 0-3:Estandar; 4-6:Refrigerado; 7:Ultra-Procesado
int indice_deposito_estandar=0;                //Lleva el indice del vector deposito en la seccion de productos Estandar
int indice_deposito_refrigerado=0;  
pthread_mutex_t mutex_standar; //para el item en el deposito
pthread_mutex_t mutex_refri; 
sem_t mutex_deposito;
sem_t sem_respuesta_operador;
*/


/*
const char tipo_producto_str[3][20] = {"Estándar", "Refrigerado", "Ultradelicado"}; 
                                                    //para imprimir el tipo de producto en texto
int frecuencias_pr[4][12] = {
    {0,0,0,0,1,1,1,1,2,2,2,2}, // Frecuencias normales
    {0,0,0,0,0,0,1,1,1,2,2,2},   // Frecuencias favorables para productos estandar
    {1,1,1,1,1,1,0,0,0,2,2,2},    // Frecuencias favorables para productos refrigerados
    {2,2,2,2,2,2,0,0,0,1,1,1}      // Frecuencias favorables para productos ultradelicados
};*/
void inicializar_hilos();
void inicializar_sem();
int main(int argc, char const *argv[]){
    
    srand(time(NULL));
    //inicializacion de los semaforos para el proceso dron_recolector
    inicializar_sem();
    //------ Creacion de los Hilos ------
    inicializar_hilos();
    /*
    //destruccion de los semaforos
    sem_destroy(&sem_cap_recoleccion);
    pthread_mutex_destroy(&sem_sala_desinfeccion);
    sem_destroy(&sem_agente_des);
    sem_destroy(&sem_fin_des);
    sem_destroy(&sem_espacios_vacios);
    sem_destroy(&sem_elementos_disp);
    pthread_mutex_destroy(&mutex_buffer);
    */
    printf("Sistema iniciado. Presiona Ctrl+C para salir.\n");
    while (1){
        sleep(1);
    }
    
    //printf("\nEl tiempo tomado es: %ld segundos %ld nanosegundos.\n", );

    return 0;
}

