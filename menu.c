#include "main.h"

// Variable global para controlar ejecución
int sistema_activo = 0;

void iniciar_simulacion_segundos(int segundos) {
    printf("\nINICIANDO SIMULACIÓN POR %d SEGUNDOS...\n", segundos);
    //Inicializar semaforos
    inicializar_sem();
    // Crear todos los hilos
    inicializar_hilos();
    // Esperar el tiempo indicado
    sleep(segundos);
    // Cancelar todos los hilos
    printf("\nTIEMPO CUMPLIDO. DETENIENDO SIMULACIÓN...\n");
    pthread_cancel(hilo_agente);
    pthread_cancel(operador_almacen);
    for (int i = 0; i < N_DRONES_PR; i++) {
        pthread_cancel(drones[i]);
    }
    for (int i = 0; i < BRAZOS; i++) {
        pthread_cancel(brazo[i]);
    }
    for (int i = 0; i < M_DONES_CARGA; i++) {
        pthread_cancel(drones_carga[i]);
    }
    // Mostrar resultados
    mostrar_resultados();
    printf("\nSimulación completada. Volviendo al menú...\n");
}

void iniciar_simulacion_productos(int productos){
    if(productos!=0){
        printf("\nINICIANDO SIMULACIÓN POR %d PRODUCTOS...\n", productos);
        //Inicializar semaforos
        inicializar_sem();
        // Crear todos los hilos
        inicializar_hilos();
        // Esperar a que se cumpla la cantidad de productos necesarios
        sem_wait(&sem_finalizo_producto);
        // Cancelar todos los hilos
        printf("\nPRODUCTOS COMPLETADOS. DETENIENDO SIMULACIÓN...\n");
        pthread_cancel(hilo_agente);
        pthread_cancel(operador_almacen);
        for (int i = 0; i < N_DRONES_PR; i++) {
            pthread_cancel(drones[i]);
        }
        for (int i = 0; i < BRAZOS; i++) {
            pthread_cancel(brazo[i]);
        }
        for (int i = 0; i < M_DONES_CARGA; i++) {
            pthread_cancel(drones_carga[i]);
        }
    }
    // Mostrar resultados
    mostrar_resultados();
    printf("\nSimulación completada. Volviendo al menú...\n");
}

void menu_principal() {
    printf("\n═══════════════════════════════════════\n");
    printf("       Configuraciones preliminares\n");
    printf("═══════════════════════════════════════\n");
    printf("1. Seguir con la configuracion estandar (Probabilidades equitativas, 10 drones)\n");
    printf("2. Cambiar la probabilidad de los productos\n");
    printf("3. Cambiar la cantidad de drones que entran al recinto\n");
    printf("4. Aplicar Cambios\n");
    printf("═══════════════════════════════════════\n");
    printf("Opción: ");
    //Pide opción al usuario y ejecuta
    scanf("%d", &opcion);
    switch(opcion) {
        case 1:
            printf("Aplicando configuracion estandar\n");
            estandar = 1;
            break;
        case 2:
            printf("Ingrese la Probabilidad del producto estandar (estandar - refrigerado - ultra delicado):\n");
            scanf("%d", &prob_standar);
            printf("Ingrese la Probabilidad del producto refrigerado (estandar - refrigerado - ultra delicado):\n");
            scanf("%d", &prob_ultradelicado);
            break;
        case 3:
            printf("AUN NO (POR AHORA SE ESTA TRABAJANDO EN ELLO\n");
            break;
        case 4:
            printf("Cambios aplicados.\n");
            break;
        default:
            printf("Opción no válida\n");
            break;
        }
    int segundos; //Segundos que tardará la simulación
    //Muestra el menú principal
    printf("\n═══════════════════════════════════════\n");
    printf("            MENU PRINCIPAL\n");
    printf("═══════════════════════════════════════\n");
    printf("1. Iniciar simulación por tiempo (Segundos)\n");
    printf("2. Iniciar simulación por producto\n");
    printf("3. Salir\n");
    printf("═══════════════════════════════════════\n");
    printf("Opción: ");
    //Pide opción al usuario y ejecuta
    scanf("%d", &opcion);
    switch(opcion) {
        case 1:
            printf("Ingrese duración en segundos: ");
            scanf("%d", &segundos);
            //Inicia la simulación
            iniciar_simulacion_segundos(segundos);
            break;
        case 2:
            printf("Ingrese la cantidad de productos a procesar:");
            scanf("%d", &productos_necesarios);
            iniciar_simulacion_productos(productos_necesarios);
            break;
        case 3:
            printf("Saliendo del sistema...\n");
            break;
        default:
            printf("Opción no válida\n");
        }
}