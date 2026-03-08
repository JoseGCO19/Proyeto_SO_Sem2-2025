#include<stdio.h> //libreria para funciones estandares
#include<semaphore.h> //para la inclusion de semaforos
#include<pthread.h> //libreria que proporciona herramientas para manejar hilos , standar POSIX
#include<unistd.h> //para funciones de interaccion con el sistema operativo, ejemplo sleep (para simular tiempos de espera )
#include<time.h>

#include "main.h"

//Prototipos
void* agente_desinfeccion(void* arg);
void* dron_recolector(void* arg);
void* dron_carga(void *arg);
void* operario_almacen(void *arg);
void* brazo_clasificado( void *arg);
void inicializar_hilos();

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
//VARIABLE PARA DRONES_RECOLECTORES
sem_t sem_cap_recoleccion;  //25                 //Limita la entrada al centro 
pthread_mutex_t sem_sala_desinfeccion;       //Mutex para la entrada a la sala de desinfeccion
sem_t sem_agente_des;   //1                     //Despierta al agente de desinfeccion
sem_t sem_fin_des;    //0                       //Semaforo donde el Dron recolector espera hasta ser desinfectado
sem_t sem_espacios_vacios; //CAP_ZONA_DESCARGA Limita la entrada a la zona de descarga
sem_t sem_elementos_disp; //0                  //Llama al proceso del brazo. Indica que dejo elementos en la zona de descarga
pthread_mutex_t mutex_buffer;                //permite la modificacion en la estructura que almacena los elementos de la zona de descarga
Producto buffer_descarga[CAP_ZONA_DESCARGA]; //[Zona de descarga], buffer donde se almacenaran los productos
int indice_producto=0;                         //lleva el conteo de cuantos productos hay
pthread_t drones[N_DRONES_PR];              //definicion de un array de 25 drones (hilos)
int ids_drones[N_DRONES_PR];                // para almacenar los id de todos los drones


//VARIABLES PARA EL DEPOSITO
extern int deposito[TOTAL_DEPOSITOS];               //Vector que almacena la cantidad de cajas por deposito. 0-3:Estandar; 4-6:Refrigerado; 7:Ultra-Procesado
extern int indice_deposito_estandar=0;                //Lleva el indice del vector deposito en la seccion de productos Estandar
extern int indice_deposito_refrigerado=0;             //Lleva el indice del vector deposito en la seccion de productos Refrigerados
extern pthread_mutex_t mutex_standar; 
extern pthread_mutex_t mutex_refri; 
extern char tipo_producto_str[3][20];               //Vector de strings para imprimir el tipo de producto en texto

//Hilos e ID's
pthread_t drones[N_DRONES_PR];               //Hilos de los drones
int ids_drones[N_DRONES_PR];                 //ID's de los Drones de Recoleccion
pthread_t drones_carga[M_DONES_CARGA];
int ids_drones_carga[M_DONES_CARGA];
pthread_t brazo[BRAZOS];
int ids_brazo[BRAZOS];
pthread_t operador_almacen;

//contadores de resultados
int usos_plataforma=0;                         //Variable que cuenta los usos que tuvo la plataforma magnetica a lo largo del programa
int producto_estandar=0;                       //Variable que lleva la cuenta de cuantos productos de tipo Estandar se proceso
int producto_refrigerado=0;                    //Variable que lleva la cuenta de cuantos productos de tipo Refrigerado se proceso
int producto_ultra_procesado=0;                //Variable que lleva la cuenta de cuantos productos de tipo Ultra-Procesado se proceso

//VARIABLES NECESARIAS PARA EL PROCESO BRAZO_RECOLECTOR
extern pthread_mutex_t mutex_buffer_descarga;
extern sem_t sem_drones_carga; // inicializar en 4
extern pthread_mutex_t mutex_metricas; 
extern int bloqueos_evitados=0;
extern pthread_mutex_t mutex_buzon;
extern int buzon_id_brazo;
extern sem_t sem_iniciar_viaje_dron;  //inicializar en 0
extern sem_t sem_fin_viaje_brazo[BRAZOS];
extern sem_t sem_plataforma_levitacion;//inicializar en 1
extern pthread_mutex_t mutex_metricas_levitacion;
extern double tiempo_total_acum=0;
extern int productos_procesados=0;



int main(int argc, char const *argv[]){
    
    srand(time(NULL));
    //inicializacion de los semaforos para el proceso dron_recolector
    sem_init(&sem_cap_recoleccion,0,N_DRONES_PR); //25 capacidad de drones 
    pthread_mutex_init(&sem_sala_desinfeccion,NULL); //este por defecto se inicializa en 1
    sem_init(&sem_agente_des,0,0); //inicializado en 0 dado que debe espera que dron_recolector lo llame
    sem_init(&sem_fin_des,0,0);
    sem_init(&sem_espacios_vacios,0,CAP_ZONA_DESCARGA); //inicializado en 10 para el buffer
    sem_init(&sem_elementos_disp,0,0);
    sem_init(&sem_llamar_operario,0,0); 
    pthread_mutex_init(&mutex_buffer,NULL); //para el acceso al buffer , 1 por hilo
    pthread_mutex_init(&mutex_dronCarga,NULL); //para el acceso al viaje del dron, 1 por hilo
    pthread_mutex_init(&mutex_almacen,NULL); //Para el acceso al almacen, 1 por hilo


    //Inicialización del semaforo de sem_fin_viaje_brazo
    for (int i = 0; i < BRAZOS; i++){
        sem_init(&sem_fin_viaje_brazo[i],0,0);
    }
    //Inicialización de los semaforos de depositoLibre y deposito_vaciado
    for (int i = 0; i < TOTAL_DEPOSITOS; i++){
        sem_init(&deposito_libre[i],0,3);
        sem_init(&deposito_vaciado[i],0,0);
    }
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
    //printf("\nEl tiempo tomado es: %ld segundos %ld nanosegundos.\n", );

    return 0;
}

void inicializar_hilos(){

    pthread_t hilo_agente; //hilo para el agente
    pthread_create(&hilo_agente, NULL, agente_desinfeccion, NULL); 
    pthread_create(&operador_almacen, NULL, operario_almacen, NULL);
    //creacion de los hilos para los drones
    for (int i = 0; i < N_DRONES_PR; i++){
        ids_drones[i]=i+1; //asignacion de los ids unicos 
        pthread_create(&drones[i], NULL, dron_recolector, &ids_drones[i]);
    }
    for(int i=0 ; i<BRAZOS ; i++){
        ids_brazo[i]=i+1;
        pthread_create(&brazo[i],NULL, brazo_clasificado, &ids_brazo[i]);
    }

    /*//creacion de los hilos para los drones de Carga
    for (int i = 0; i < M_DONES_CARGA; i++) {
        ids_drones_carga[i] = i + 1;
        pthread_create(&drones_carga[i], NULL, dron_carga, &ids_drones_carga[i]);
    }
    */
    
}