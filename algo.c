// Variable global para contabilizar
int bloqueos_evitados = 0;
sem_t drones_carga_disponibles; // Inicializado en 4

void* hilo_brazo_clasificador(void* arg) {
    int tipo_producto = (int)arg; 

    if (tipo_producto == REFRIGERADO) {
        // Intenta ver si hay 2 drones antes de bloquearse "para siempre"
        // sem_trywait devuelve -1 si el recurso está ocupado
        if (sem_trywait(&drones_carga_disponibles) != 0) {
            printf("LOG: No hay drones suficientes. Bloqueo evitado.\n");
            bloqueos_evitados++; // <--- AQUÍ LO CONTABILIZAS
            
            // Ahora sí, se bloquea formalmente hasta que se liberen
            sem_wait(&drones_carga_disponibles); 
        }
        sem_wait(&drones_carga_disponibles); // Pide el segundo
        
        // ... realizar movimiento ...
        
        sem_post(&drones_carga_disponibles); // Libera uno
        sem_post(&drones_carga_disponibles); // Libera el otro
    }
}