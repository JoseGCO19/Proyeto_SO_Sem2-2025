#include<stdio.h> //libreria para funciones estandares
#include<semaphore.h> //para la inclusion de semaforos
#include<pthread.h> //libreria que proporciona herramientas para manejar hilos , standar POSIX
#include<unistd.h> //para funciones de interaccion con el sistema operativo, ejemplo sleep (para simular tiempos de espera )
#include<time.h>
#define N_DRONES_PR 25
#define CAP_ZONA_DESCARGA 10 //limite para el buffer de carga


//estructura para el producto
typedef struct {
    int tipo_producto; //0:estandar, 1: regrigerado, 2: ultra_delicado
    //metricas del tiempo
}Producto;
//estructura para el buffer de descarga , asumiendo una capacidad para 10 productos
Producto buffer_descarga[CAP_ZONA_DESCARGA]; //buffer 
int indice_producto = 0; //para desplazarse a traves del buffer 
pthread_t drones[N_DRONES_PR]; //definicion de un array de 25 drones (hilos)
//falta definir si es global o local al main ??
//definicion de los semaforos de interaccion entre drop_recolector y agente_desifeccion
sem_t sem_cap_recoleccion; //semaforo contador de dron recolector (25 disponibles)
pthread_mutex_t sem_sala_desinfeccion; //semaforo para controlar la espera a la sala de desinfeccion , inicializado en 1
sem_t sem_agente_des; //semaforo para el agente de desifeccion inicializado en 0, dado que hay un solo agente
//el agente de desinfeccion debe esperar a que un dron lo llame
sem_t sem_fin_des; //semaforo para validar, que el agente ya proceso el id y bateria del semaforo, inicializado en 0
//semaforos para la zona de carga
sem_t sem_espacios_vacios; //semafor para el buffer , inicializado en CAP_ZONA_DESCARGA
sem_t sem_elementos_disp; // inicializado en 0
pthread_mutex_t mutex_buffer; //iniciarlizar en 1



void* dron_recolector(void *arg){
    while (1){ //garantiza que el dron nunca se detenga mientras el sistema este activo
        //FASE 1: entrada al recinto , solo 25 drones para la zona de cultivo
        int id_dron = *((int*)arg); //convertir el puntero generico a una direccion real int 
        //hace una llamada a wait para entrar al cultivo
        sem_wait(&sem_cap_recoleccion); //verifica que haya drones disponibles
        //ahora cada dron recolector debe esperar para pasar a la sala de desifeccion para hacer atendido por el agente de desifeccion
        //FASE 2: los drones deben entrar a la sala de espera para desinfeccion
        pthread_mutex_lock(&sem_sala_desinfeccion); //solo permitira el acceso a un dron de recoleccion al la sala de desinfeccion
        printf("\nDron[%d] iniciando proceso de desinfeccion.\n", id_dron);
        //despierta al hilo del agente de desinfeccion para que atienda al dron entrante
        sem_post(&sem_agente_des); //lo despierta
        //el dron espera a que el agente valide su id y su bateria
        sem_wait(&sem_fin_des); 
        //el dron ya esta limpio, libera el espacio para el siguiente dron
        pthread_mutex_unlock(&sem_sala_desinfeccion);
        //FASE 3: Cosecha
        printf("\nDron[%d] con autorizacion. recoleccion inicializada...\n",id_dron);
        sleep(rand()%3+1); //simulacion , busqueda y recoleccion del producto
        //se crea el producto, simular recoleccion
        Producto nuevo_producto;
        nuevo_producto.tipo_producto = rand() %3;

        //FASE 4: entrega en la zona de carga
        //esperar a que haya un espacio libre en la zona de descarga
        sem_wait(&sem_espacios_vacios);
        //bloquear el buffer para deposirar de forma segura
        pthread_mutex_lock(&mutex_buffer);
        buffer_descarga[indice_producto]=nuevo_producto; //colocar el producto en el buffer
        printf("\nDrone[%d], deposito producto %d en la posicion [%d] del buffer.\n", id_dron,nuevo_producto.tipo_producto,indice_producto);
        //avanzar al indice de forma circular
        indice_producto=(indice_producto+1)%CAP_ZONA_DESCARGA;
        pthread_mutex_unlock(&mutex_buffer); //dar acceso al siguiente hilo
        //avisa, hay un nuevo producto para clasificar
        sem_post(&sem_elementos_disp); 
        
        //FASE 5: salida del centro
        //el dron termino su fase de ejecucion , sale del recinto liberando un espacio
        printf("\nDrone[%d], ha salido de la granja. volviendo a la cola externa...\n",id_dron);
        sem_post(&sem_cap_recoleccion);
        sleep(2);
    }
    
    return NULL;

}


int main(int argc, char const *argv[]){
    

    //






    return 0;
}
