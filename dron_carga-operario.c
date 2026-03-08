#include "main.h"

void* dron_carga(void *arg){ 
    int id_dron = *((int*)arg);
    while(1){
        //El dron espera a ser llamado por el brazo clasificador
        sem_wait(&sem_iniciar_viaje_dron);
        pthread_mutex_lock(&mutex_dronCarga); //solo permitira el acceso a que un dron de carga sea asignado a un brazo
        int buzon_brazo = buzon_id_brazo; //Se le asigna el ID del brazo actual a el dron actual
        pthread_mutex_unlock(&mutex_dronCarga);
        printf(COLOR_NARANJA "\n>>> DRON DE CARGA [%d]:" COLOR_RESET "Iniciando traslado para el Brazo [%d]...\n\n", id_dron, buzon_brazo);
        sleep(rand()%3+1); //Simulación del tiempo de viaje
        printf(COLOR_NARANJA "\n>>> DRON DE CARGA [%d]:" COLOR_RESET "Traslado finalizado.\n\n", id_dron);
        sem_post(&sem_fin_viaje_brazo[buzon_brazo - 1]); //Manda  señal de que el dron de carga terminó su viaje
    }
    return NULL;
}

void* operario_almacen(void *arg){
    while(1){
        //El operario espera a ser llamado para retirar los depositos llenos
        sem_wait(&sem_llamar_operario);
        pthread_mutex_lock(&mutex_almacen); //Entra solo uno a la vez al almacen
        for (int i = 0; i < TOTAL_DEPOSITOS; i++){ //pasamos por todos los depositos
            if(deposito[i] == 3){
                printf(COLOR_MORADO "\n>>> OPERARIO: " COLOR_RESET "Depósito [%d] lleno. Retirando producto y colocando cajas nuevas...\n\n", i);
                sleep(rand()%3+1); //Simulación del tiempo de descarga y reponer cajas
                deposito[i] = 0; //Se vacia el deposito
                sem_post(&deposito_libre[i]);
                sem_post(&deposito_libre[i]);
                sem_post(&deposito_libre[i]);
                printf(COLOR_MORADO "\n>>> OPERARIO: " COLOR_RESET "Depósito [%d] vaciado y listo.\n\n", i);
                sem_post(&deposito_vaciado[i]);
            }
        }
        pthread_mutex_unlock(&mutex_almacen); //da espacio al siguiente
    }
}