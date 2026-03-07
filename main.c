#include<stdio.h> //libreria para funciones estandares
#include<semaphore.h> //para la inclusion de semaforos
#include<pthread.h> //libreria que proporciona herramientas para manejar hilos , standar POSIX
#include<unistd.h> //para funciones de interaccion con el sistema operativo, ejemplo sleep (para simular tiempos de espera )
#include<time.h>

#include "main.h"

//Prototipos
void* agente_desinfeccion(void* arg);
void* dron_recolector(void* arg);

//Jesus crisrto
//Samuel lafffkkkkk
//estructura para el producto

/**/
//estructura para el buffer de descarga , asumiendo una capacidad para 10 productos
Producto buffer_descarga[CAP_ZONA_DESCARGA]; //buffer 
int indice_producto = 0;                    //para desplazarse a traves del buffer 
pthread_t drones[N_DRONES_PR];              //definicion de un array de 25 drones (hilos)
//falta definir si es global o local al main ??
int ids_drones[N_DRONES_PR];                // para almacenar los id de todos los drones
//definicion de los semaforos de interaccion entre drop_recolector y agente_desifeccion
sem_t sem_cap_recoleccion;                  //semaforo contador de dron recolector (25 disponibles)
pthread_mutex_t sem_sala_desinfeccion;      //semaforo para controlar la espera a la sala de desinfeccion , inicializado en 1
sem_t sem_agente_des;                       //semaforo para el agente de desifeccion inicializado en 0, dado que hay un solo agente
                                            //el agente de desinfeccion debe esperar a que un dron lo llame
sem_t sem_fin_des;                          //semaforo para validar, que el agente ya proceso el id y bateria del semaforo, inicializado en 0
//semaforos para la zona de carga
sem_t sem_espacios_vacios;                  //semafor para el buffer , inicializado en CAP_ZONA_DESCARGA
sem_t sem_elementos_disp;                   // inicializado en 0
pthread_mutex_t mutex_buffer;               //iniciarlizar en 1

int main(int argc, char const *argv[]){
    
    srand(time(NULL));
    //inicializacion de los semaforos para el proceso dron_recolector
    sem_init(&sem_cap_recoleccion,0,N_DRONES_PR); //25 capacidad de drones 
    pthread_mutex_init(&sem_sala_desinfeccion,NULL); //este por defecto se inicializa en 1
    sem_init(&sem_agente_des,0,0); //inicializado en 0 dado que debe espera que dron_recolector lo llame
    sem_init(&sem_fin_des,0,0);
    sem_init(&sem_espacios_vacios,0,CAP_ZONA_DESCARGA); //inicializado en 10 para el buffer
    sem_init(&sem_elementos_disp,0,0); 
    pthread_mutex_init(&mutex_buffer,NULL); //para el acceso al buffer , 1 por hilo

    pthread_t hilo_agente; ///hilo para el agente
    pthread_create(&hilo_agente, NULL, agente_desinfeccion, NULL); 
    //creacion de los hilos para los drones
    for (int i = 0; i < N_DRONES_PR; i++){
        ids_drones[i]=i+1; //asignacion de los ids unicos 
        pthread_create(&drones[i], NULL, dron_recolector,&ids_drones[i]);
    }
    /*
    for (int i = 0; i < N_DRONES_PR; i++){
        pthread_join(drones[i],NULL);
    }
    
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
