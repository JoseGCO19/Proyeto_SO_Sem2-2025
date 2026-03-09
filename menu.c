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

void menu_principal() {
    int opcion; //Indica la opción del usuario
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
            printf("AUN NO IMPLEMENTADO..");
            break;
        case 3:
            printf("Saliendo del sistema...\n");
            break;
        default:
            printf("Opción no válida\n");
        }
    
}