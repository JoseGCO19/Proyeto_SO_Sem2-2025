//definicion del brazo recolector 
#include"main.h"

int target_deposito; //variable global compartida solo por este proceso

//funcion para calcular ms
long calcular_ms(struct timespec inicio, struct timespec fin) {
    long seg = fin.tv_sec - inicio.tv_sec;
    long nsec = fin.tv_nsec - inicio.tv_nsec;
    return (seg * 1000) + (nsec / 1000000);
}
void despacho_dron(Producto nuevo_pr_saliente,int id_brazo);
void get_target(Producto item);

void* brazo_clasificado( void *arg){

    Producto nuevo_pr_saliente;
    int id_brazo = *((int*)arg);

    while (1){
        //FASE 1: extraccion del producto
        sem_wait(&sem_elementos_disp);                      //espera a que el dron recolector le avise que ya hay recursos disponibles en el almacen
        pthread_mutex_lock(&mutex_buffer_descarga);         //para que solo 1 proceso hilo pueda sacar un recurso
        //SECION CRITICA 
        nuevo_pr_saliente= buffer_descarga[indice_producto]; //ademas se protege el uso de indice_producto y el buffer
        printf(COLOR_ROJO "\n BRAZO[%d]" COLOR_RESET "tomó un producto %s en la posicion %d del almacen termporal\n", id_brazo, tipo_producto_str[nuevo_pr_saliente.tipo_producto], indice_producto);
        indice_producto = (indice_producto +1) % CAP_ZONA_DESCARGA; 

        pthread_mutex_unlock(&mutex_buffer_descarga);       //da espacio al siguiente
        //FASE 2: DESPACHO
        sem_post(&sem_espacios_vacios);
        despacho_dron(nuevo_pr_saliente,id_brazo);
        struct timespec tiempo_fin;
        clock_gettime(CLOCK_MONOTONIC,&tiempo_fin);
        //calcular la duracion S
        long duracion = calcular_ms(nuevo_pr_saliente.tiempo_inicio, tiempo_fin);
        //actualizar metricas 
        pthread_mutex_lock(&mutex_metricas);
        tiempo_total_acum+=duracion;
        productos_procesados++;
        printf(COLOR_AZUL "\n[METRICA]" COLOR_RESET " Promedio actual: %.2f ms\n", (tiempo_total_acum / productos_procesados));
        pthread_mutex_unlock(&mutex_metricas);

        //FASE 4; DEPOSITOS Y OPERADORES DE ALMACEN
        pthread_mutex_lock(&mutex_deposito);
        get_target(nuevo_pr_saliente);
        deposito[target_deposito]=deposito[target_deposito]+1;
        if(deposito[target_deposito] == 3){
            sem_post(&sem_llamar_operario);
            pthread_mutex_unlock(&mutex_deposito);
            sem_wait(&deposito_libre[target_deposito]);
        }else
            pthread_mutex_unlock(&mutex_deposito);
    }
    
    return NULL;
}
void get_target(Producto item){

    if(item.tipo_producto == 0){
        pthread_mutex_lock(&mutex_standar);
        target_deposito = indice_deposito_estandar;
        indice_deposito_estandar = (target_deposito + 1) % 4;
        pthread_mutex_unlock(&mutex_standar);
    }else if(item.tipo_producto == 1){
        pthread_mutex_lock(&mutex_refri);
        target_deposito = indice_deposito_refrigerado +4;
        indice_deposito_refrigerado = (indice_deposito_refrigerado +1 ) % 3;
        pthread_mutex_unlock(&mutex_refri);
    }else{
        target_deposito=7;
    }
}

void despacho_dron(Producto nuevo_pr_saliente,int id_brazo){
    if(nuevo_pr_saliente.tipo_producto == 1){
            //producto refrigerado ; requiere 2 drones de carga
            while (1){
                sem_wait(&sem_drones_carga); //este debera estar inicializadp en 4 
                if(sem_trywait(&sem_drones_carga) == 0){ 
                    printf(COLOR_ROJO "\n BRAZO[%d] " COLOR_RESET "reservo 2 drones.\n ",id_brazo);
                    break;
                }else{
                    sem_post(&sem_drones_carga); //suelta 1
                    pthread_mutex_lock(&mutex_metricas);
                    bloqueos_evitados++;
                    pthread_mutex_unlock(&mutex_metricas);
                    printf(COLOR_AMARILLO "\n BRAZO[%d] " COLOR_ROJO "evito bloqueo por falta de drones. Reintentando... \n",id_brazo);
                    sleep(1);
                }
            }
            pthread_mutex_lock(&mutex_buzon);
            buzon_id_brazo = id_brazo;
            sem_post(&sem_iniciar_viaje_dron);
            sem_post(&sem_iniciar_viaje_dron);
            pthread_mutex_unlock(&mutex_buzon);
            printf(COLOR_AMARILLO "\n BRAZO[%d] envio 2 drones." COLOR_RESET " esperando confirmacion...\n",id_brazo);

            //espera confirmacion en su semaforo privado 
            sem_wait(&sem_fin_viaje_brazo[id_brazo-1]);
            sem_wait(&sem_fin_viaje_brazo[id_brazo-1]);
            //libera los recursos
            sem_post(&sem_drones_carga);
            sem_post(&sem_drones_carga);
            
        }else if(nuevo_pr_saliente.tipo_producto == 2){
            //PRODUCTO ULTRADELICADO - requiere 1 dron + 1 plataforma de levitacion 
            while (1){
                sem_wait(&sem_drones_carga);
                if(sem_trywait(&sem_plataforma_levitacion)==0){
                    printf(COLOR_ROJO "\n BRAZO[%d] " COLOR_RESET " reservo 1 drones y una plataforma.\n", id_brazo);
                    pthread_mutex_lock(&mutex_metricas_levitacion); 
                    usos_plataforma++;        //seccion critica , plataforma de levitacion
                    pthread_mutex_unlock(&mutex_metricas_levitacion);
                    break;
                }else{
                    sem_post(&sem_drones_carga);
                    pthread_mutex_lock(&mutex_metricas); //para la evitacion de bloqueos
                    bloqueos_evitados++; //seccion critca
                    pthread_mutex_unlock(&mutex_metricas);
                    printf(COLOR_ROJO "\n BRAZO[%d] " COLOR_RESET "evito bloqueo por plataforma ocupada. Reintentando...\n",id_brazo);
                    sleep(1);
                    
                }
            }
            //enviar orden al buzon 
            pthread_mutex_lock(&mutex_buzon);
            buzon_id_brazo = id_brazo;
            sem_post(&sem_iniciar_viaje_dron);
            pthread_mutex_unlock(&mutex_buzon);

            printf(COLOR_ROJO "\n BRAZO[%d] "COLOR_RESET "envio 1 dron con la plataforma.Esperando...\n",id_brazo);
            sem_wait(&sem_fin_viaje_brazo[id_brazo-1]);
            sem_post(&sem_drones_carga);
            sem_post(&sem_plataforma_levitacion); //libera la plataforma
        }else{
            //PRODUCTO ESTANDAR
            printf(COLOR_ROJO "\n BRAZO[%d] "COLOR_RESET "procesando producto estandar.\n",id_brazo);
            sleep(1);
        }

}