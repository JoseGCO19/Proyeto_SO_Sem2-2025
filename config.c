#include<stdio.h> //libreria para funciones estandares
#include<semaphore.h> //para la inclusion de semaforos
#include<pthread.h> //libreria que proporciona herramientas para manejar hilos , standar POSIX
#include<unistd.h> //para funciones de interaccion con el sistema operativo, ejemplo sleep (para simular tiempos de espera )
#include<time.h>

#include "main.h"

void* agente_desinfeccion(void* arg);
void* dron_recolector(void* arg);
void* dron_carga(void *arg);
void* operario_almacen(void *arg);
void* brazo_clasificado( void *arg);
void inicializar_hilos();
void inicializar_sem();

//VARIABLES MENU
int estandar = 0;
int opcion;

//VARIABLE PARA DRONES_RECOLECTORES
sem_t sem_cap_recoleccion;  //25                 //Limita la entrada al centro 
pthread_mutex_t sem_sala_desinfeccion;           //Mutex para la entrada a la sala de desinfeccion
sem_t sem_agente_des;   //1                      //Despierta al agente de desinfeccion
sem_t sem_fin_des;    //0                        //Semaforo donde el Dron recolector espera hasta ser desinfectado
sem_t sem_espacios_vacios;                       //CAP_ZONA_DESCARGA Limita la entrada a la zona de descarga
sem_t sem_elementos_disp; //0                    //Llama al proceso del brazo. Indica que dejo elementos en la zona de descarga
pthread_mutex_t mutex_buffer;                    //permite la modificacion en la estructura que almacena los elementos de la zona de descarga
Producto buffer_descarga[CAP_ZONA_DESCARGA];     //[Zona de descarga], buffer donde se almacenaran los productos
int indice_productor=0;    
int indice_consumidor=0;
pthread_mutex_t mutex_contador_resultado;                     //lleva el conteo de cuantos productos hay
int prob_standar;
int prob_ultradelicado;


//VARIABLES PARA EL DEPOSITO
int deposito[TOTAL_DEPOSITOS];                 //Vector que almacena la cantidad de cajas por deposito. 0-3:Estandar; 4-6:Refrigerado; 7:Ultra-Procesado
int indice_deposito_estandar=0;                //Lleva el indice del vector deposito en la seccion de productos Estandar
int indice_deposito_refrigerado=0;             //Lleva el indice del vector deposito en la seccion de productos Refrigerados
pthread_mutex_t mutex_standar;                 //Permite editar el indice del vector deposito en el rango de los depositos estandar
pthread_mutex_t mutex_refri;                   //Permite editar el indice del vector deposito en el rango de los depositos Refrigerados
char tipo_producto_str[3][20];                 //Vector de strings para imprimir el tipo de producto en texto

//Hilos e ID's
pthread_t drones[N_DRONES_PR];               //definicion de un array de 25 drones (hilos)
int ids_drones[N_DRONES_PR];                 //ID's de los Drones de Recoleccion
pthread_t drones_carga[M_DONES_CARGA];
int ids_drones_carga[M_DONES_CARGA];
pthread_t brazo[BRAZOS];
int ids_brazo[BRAZOS];
pthread_t operador_almacen;
pthread_t hilo_agente;

//contadores de resultados
int usos_plataforma=0;                         //Variable que cuenta los usos que tuvo la plataforma magnetica a lo largo del programa
int producto_estandar=0;                       //Variable que lleva la cuenta de cuantos productos de tipo Estandar se proceso
int producto_refrigerado=0;                    //Variable que lleva la cuenta de cuantos productos de tipo Refrigerado se proceso
int producto_ultra_delicado=0;                //Variable que lleva la cuenta de cuantos productos de tipo Ultra-Procesado se proceso

//VARIABLES NECESARIAS PARA EL PROCESO BRAZO_RECOLECTOR
pthread_mutex_t mutex_buffer_descarga;
sem_t sem_drones_carga; // inicializar en 4
pthread_mutex_t mutex_metricas; 
int bloqueos_evitados=0;                        //Contador de bloqueos evitados
pthread_mutex_t mutex_buzon;                    //Mutex para que el brazo guarde su ID y se lo mande a los drones de carga
int buzon_id_brazo;                             //Variable globar a la cual acceden el Brazo y los Drones de carga, uno evia su ID y el otro lo recibe
sem_t sem_iniciar_viaje_dron;                   //inicializar en 0
sem_t sem_fin_viaje_brazo[BRAZOS];              //Semaforo donde espera el brazo hasta que el dron finalize su proceso
sem_t sem_plataforma_levitacion;                //inicializar en 1//Mutex para exlcluir el uso de la plataforma magnetica
pthread_mutex_t mutex_metricas_levitacion;      //Mutex que cuida el contador de usos de la Plataforma de Levitacion
double tiempo_total_acum=0;
int productos_procesados=0;                     //Contador de los productos totales
sem_t sem_finalizo_producto;                    //Semafdoro para finalizar el proceso de la simulacion por cantidad de productos
int productos_necesarios;

//Semaforos de depositos
sem_t deposito_vaciado[TOTAL_DEPOSITOS]; //inicializado en 3
sem_t deposito_libre[TOTAL_DEPOSITOS]; //inicializado en 0
pthread_mutex_t mutex_almacen;
sem_t sem_llamar_operario;
pthread_mutex_t mutex_dronCarga;
//pthread_mutex_t mutex_deposito;

//INICIALIZACION DE SEMAFOROS

void inicializar_sem(){
    sem_init(&sem_cap_recoleccion,0,N_DRONES_PR); //25 capacidad de drones 
    pthread_mutex_init(&sem_sala_desinfeccion,NULL); //este por defecto se inicializa en 1
    sem_init(&sem_agente_des,0,0); //inicializado en 0 dado que debe espera que dron_recolector lo llame
    sem_init(&sem_fin_des,0,0);
    sem_init(&sem_espacios_vacios,0,CAP_ZONA_DESCARGA); //inicializado en 10 para el buffer
    sem_init(&sem_elementos_disp,0,0);
    sem_init(&sem_llamar_operario,0,0);
    sem_init(&sem_drones_carga,0,4);
    sem_init(&sem_iniciar_viaje_dron,0,0);
    sem_init(&sem_plataforma_levitacion,0,1);
    sem_init(&sem_finalizo_producto,0,0);
    //pthread_mutex_init(&mutex_deposito,NULL);
    pthread_mutex_init(&mutex_buffer,NULL); //para el acceso al buffer , 1 por hilo
    pthread_mutex_init(&mutex_dronCarga,NULL); //para el acceso al viaje del dron, 1 por hilo
    pthread_mutex_init(&mutex_almacen,NULL); //Para el acceso al almacen, 1 por hilo
    pthread_mutex_init(&mutex_standar,NULL); 
    pthread_mutex_init(&mutex_refri, NULL);
    pthread_mutex_init(&mutex_buffer_descarga, NULL);
    pthread_mutex_init(&mutex_metricas, NULL);
    pthread_mutex_init(&mutex_buzon, NULL);
    pthread_mutex_init(&mutex_metricas_levitacion, NULL);
    pthread_mutex_init(&mutex_buzon, NULL);
    pthread_mutex_init(&mutex_contador_resultado,NULL);

    //Inicialización del semaforo de sem_fin_viaje_brazo
    for (int i = 0; i < BRAZOS; i++){
        sem_init(&sem_fin_viaje_brazo[i],0,0);
    }
    //Inicialización de los semaforos de depositoLibre y deposito_vaciado
    for (int i = 0; i < TOTAL_DEPOSITOS; i++){
        sem_init(&deposito_libre[i],0,3);
        sem_init(&deposito_vaciado[i],0,0);
    }
}

//INICIALIZACION DE HILOS
void inicializar_hilos(){
     //hilo para el agente
    pthread_create(&hilo_agente, NULL, agente_desinfeccion, NULL); 
    //hilo para el operador
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
    for (int i = 0; i < M_DONES_CARGA; i++){
        ids_drones_carga[i]= i+1;
        pthread_create(&drones_carga[i], NULL, dron_carga,&ids_drones_carga[i]);
    }
    
}

//Mostrar resultados al finalizar simulación
void mostrar_resultados() {
    printf("\n═══════════════════════════════════════\n");
    printf("    RESULTADOS DE LA SIMULACIÓN:\n");
    printf("═══════════════════════════════════════\n");
    printf("Total productos procesados: %d\n", productos_procesados);
    printf("   ├─ Estándar    : %d\n", producto_estandar);
    printf("   ├─ Refrigerado : %d\n", producto_refrigerado);
    printf("   └─ Ultra       : %d\n", producto_ultra_delicado);
    printf("Usos de plataforma magnética: %d\n", usos_plataforma);
    printf("Bloqueos evitados: %d\n", bloqueos_evitados);
    if (productos_procesados > 0) {
        printf("Tiempo promedio: %.2f ms\n", 
               tiempo_total_acum / productos_procesados);
    }
    printf("═══════════════════════════════════════\n");
}