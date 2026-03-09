#include<stdio.h> //libreria para funciones estandares
#include<semaphore.h> //para la inclusion de semaforos
#include<pthread.h> //libreria que proporciona herramientas para manejar hilos , standar POSIX
#include<unistd.h> //para funciones de interaccion con el sistema operativo, ejemplo sleep (para simular tiempos de espera )
#include<time.h>

#include "main.h"

/*
const char tipo_producto_str[3][20] = {"Estándar", "Refrigerado", "Ultradelicado"}; 
                                                    //para imprimir el tipo de producto en texto
int frecuencias_pr[4][12] = {
    {0,0,0,0,1,1,1,1,2,2,2,2}, // Frecuencias normales
    {0,0,0,0,0,0,1,1,1,2,2,2},   // Frecuencias favorables para productos estandar
    {1,1,1,1,1,1,0,0,0,2,2,2},    // Frecuencias favorables para productos refrigerados
    {2,2,2,2,2,2,0,0,0,1,1,1}      // Frecuencias favorables para productos ultradelicados
};*/
int main(int argc, char const *argv[]){
    
    srand(time(NULL));
    //inicializacion de los semaforos para el proceso dron_recolector
    inicializar_sem();
    //Muestra el menú
    menu_principal();
    printf("Sistema Finalizado.\n");
    return 0;
}