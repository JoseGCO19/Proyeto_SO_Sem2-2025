#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#include "main.h"


void* dron_recolector(void *arg){ //el hilo de dron debe tener una idea

    while (1){ //garantiza que el dron nunca se detenga mientras el sistema este activo
        //FASE 1: entrada al recinto , solo 25 drones para la zona de cultivo
        int id_dron = *((int*)arg); //convertir el puntero generico a una direccion real int 
        //hace una llamada a wait para entrar al cultivo
        sem_wait(&sem_cap_recoleccion); //verifica que haya drones disponibles
        //ahora cada dron recolector debe esperar para pasar a la sala de desifeccion para hacer atendido por el agente de desifeccion
        //FASE 2: los drones deben entrar a la sala de espera para desinfeccion
        pthread_mutex_lock(&sem_sala_desinfeccion); //solo permitira el acceso a un dron de recoleccion al la sala de desinfeccion
        printf( COLOR_VERDE"\n Dron[%d]" COLOR_RESET "iniciando proceso de desinfeccion.\n", id_dron);
        //despierta al hilo del agente de desinfeccion para que atienda al dron entrante
        sem_post(&sem_agente_des); 
        //el dron espera a que el agente valide su id y su bateria
        sem_wait(&sem_fin_des); 
        //el dron ya esta limpio, libera el espacio para el siguiente dron
        pthread_mutex_unlock(&sem_sala_desinfeccion);
        //FASE 3: Cosecha
        printf(COLOR_AMARILLO"\n Dron[%d] con autorizacion. recoleccion inicializada...\n",id_dron);
        sleep(rand()%3+1); //simulacion , busqueda y recoleccion del producto
        //se crea el producto, simular recoleccion
        Producto nuevo_producto;
        nuevo_producto.tipo_producto = rand() %3;
        //CLOCK_MONOTONIC asegura que el tiempo sea siempre creciente y preciso
        clock_gettime(CLOCK_MONOTONIC, &nuevo_producto.tiempo_inicio);

        //FASE 4: entrega en la zona de carga
        //esperar a que haya un espacio libre en la zona de descarga
        sem_wait(&sem_espacios_vacios);
        //bloquear el buffer para deposirar de forma segura
        pthread_mutex_lock(&mutex_buffer);
        buffer_descarga[indice_producto]=nuevo_producto; //colocar el producto en el buffer
        printf(COLOR_VERDE "\n Drone[%d]"COLOR_RESET ", deposito producto %d en la posicion [%d] del buffer.\n", id_dron,nuevo_producto.tipo_producto,indice_producto);
        //avanzar al indice de forma circular
        indice_producto=(indice_producto+1)%CAP_ZONA_DESCARGA;
        pthread_mutex_unlock(&mutex_buffer); //dar acceso al siguiente hilo
        //avisa, hay un nuevo producto para clasificar
        sem_post(&sem_elementos_disp); 
        
        //FASE 5: salida del centro
        //el dron termino su fase de ejecucion , sale del recinto liberando un espacio
        printf(COLOR_AMARILLO"\n Drone[%d]" COLOR_RESET", ha salido de la granja. volviendo a la cola externa...\n",id_dron);
        sem_post(&sem_cap_recoleccion);
        sleep(2);
    }
    return NULL;
}
void* agente_desinfeccion(void* arg) {
    while(1) { //el agente siempre esta operando
        // El agente espera a que un dron lo llame (Fase 2 del dron)
        sem_wait(&sem_agente_des); 
        
        printf(COLOR_AZUL "\n [AGENTE]"COLOR_RESET "Recibiendo Dron para desinfección...\n");
        sleep(2); // Simula el tiempo que tarda el agente trabajando
        printf(COLOR_AZUL"\n [AGENTE]" COLOR_VERDE "Dron" COLOR_RESET "desinfectado exitosamente.\n");
        
        // Avisa al dron que ya terminó su proceso
        sem_post(&sem_fin_des); 
    }
    return NULL;
}