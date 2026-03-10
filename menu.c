#include "main.h"

// Variable global para controlar ejecución
int sistema_activo = 0;
int probabilidad_valida;

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
    for (int i = 0; i < n_drones_pr; i++) {
        pthread_cancel(drones[i]);
    }
    for (int i = 0; i < BRAZOS; i++) {
        pthread_cancel(brazo[i]);
    }
    for (int i = 0; i < M_DONES_CARGA; i++) {
        pthread_cancel(drones_carga[i]);
    }
    // Mostrar resultados
    free(drones);
    free(ids_drones);
    mostrar_resultados();
    printf("\nSimulación completada. Volviendo al menú...\n");
}

void iniciar_simulacion_productos(int productos){
    if(productos > 0){
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
        for (int i = 0; i < n_drones_pr; i++) {
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
    do{
        printf("\n═══════════════════════════════════════\n");
        printf("       Configuraciones preliminares\n");
        printf("═══════════════════════════════════════\n");
        printf("1. Seguir con la configuracion estandar (Probabilidades equitativas, 25 drones)\n");
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
                n_drones_pr = 25;
                break;
            case 2:
                estandar = 0;
                do{
                    probabilidad_valida = 1;
                    printf("Ingrese la Probabilidad del producto estandar (estandar - refrigerado - ultra delicado):\n");
                    scanf("%d", &prob_standar);
                    printf("Ingrese la Probabilidad del producto refrigerado (estandar - refrigerado - ultra delicado):\n");
                    scanf("%d", &prob_refrigerado);
                    if(prob_standar + prob_refrigerado > 100){
                        printf("Error: La suma de las probabilidades excede el 100%%\n");
                        probabilidad_valida = 0;
                }} while(!probabilidad_valida);
                printf("Probabilidades = estandar %d%%, refrigerado %d%%, ultra delicado %d%%",prob_standar, prob_refrigerado, 100 - (prob_refrigerado + prob_standar));
                break;
            case 3:
                printf("Ingrese la cantidad de drones que entran al recinto (default 25): ");
                scanf("%d", &n_drones_pr);
                printf("Cantidad de drones asignada: %d", n_drones_pr);
                break;
            case 4:
                printf("Cambios aplicados.\n");
                break;
            default:
                printf("Opción no válida\n");
                break;
            }
    } while(opcion != 4);
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