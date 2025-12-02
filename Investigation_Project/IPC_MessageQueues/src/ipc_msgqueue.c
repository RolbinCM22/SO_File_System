#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

/**
 * Estructura de mensaje para System V Message Queues
 * mtype: tipo de mensaje (obligatorio, debe ser > 0)
 * valor: dato entero a comunicar entre procesos
 */
typedef struct {
    long mtype;    // tipo de mensaje
    int valor;     // dato a transmitir
} Mensaje;

#define CLAVE_PROYECTO 'I'    // Clave para ftok
#define ARCHIVO_CLAVE "/tmp"  // Archivo base para ftok
#define MTYPE_NORMAL 1        // Tipo para mensajes normales
#define MTYPE_FIN 255         // Tipo para mensaje de fin
#define NUM_MENSAJES 5        // Cantidad de mensajes a enviar

/**
 * Proceso padre (productor):
 * Envía 5 números enteros (1-5) al hijo mediante la cola de mensajes
 */
void proceso_padre(int msqid) {
    printf("[PADRE PID=%d] Iniciando proceso productor\n", getpid());
    
    Mensaje msg;
    msg.mtype = MTYPE_NORMAL;
    
    // Enviar 5 números al hijo
    for (int i = 1; i <= NUM_MENSAJES; i++) {
        msg.valor = i;
        
        // msgsnd: enviar mensaje a la cola
        if (msgsnd(msqid, (void *)&msg, sizeof(msg.valor), 0) == -1) {
            perror("[PADRE] Error en msgsnd");
            exit(EXIT_FAILURE);
        }
        printf("Padre envía: %d\n", i);
    }
    
    // Enviar mensaje de fin (mtype = 255)
    msg.mtype = MTYPE_FIN;
    msg.valor = 0;
    
    if (msgsnd(msqid, (void *)&msg, sizeof(msg.valor), 0) == -1) {
        perror("[PADRE] Error al enviar mensaje de fin");
        exit(EXIT_FAILURE);
    }
    printf("[PADRE] Mensaje de fin enviado\n");
}

/**
 * Proceso hijo (consumidor):
 * Recibe mensajes de la cola y procesa los datos
 */
void proceso_hijo(int msqid) {
    printf("[HIJO PID=%d] Iniciando proceso consumidor\n", getpid());
    
    Mensaje msg;
    
    // Bucle para recibir mensajes
    while (1) {
        // msgrcv: recibir mensaje de la cola
        // mtype = 0 significa recibir el primer mensaje disponible
        if (msgrcv(msqid, (void *)&msg, sizeof(msg.valor), 0, 0) == -1) {
            perror("[HIJO] Error en msgrcv");
            exit(EXIT_FAILURE);
        }
        
        // Si recibimos el mensaje de fin (mtype = 255), terminamos
        if (msg.mtype == MTYPE_FIN) {
            printf("[HIJO] Mensaje de fin recibido. Terminando...\n");
            break;
        }
        
        // Procesar mensaje normal (mtype = 1)
        if (msg.mtype == MTYPE_NORMAL) {
            int resultado = msg.valor * 2;
            printf("Hijo recibe %d, resultado = %d\n", msg.valor, resultado);
        }
    }
}

int main() {
    // Generar clave única mediante ftok
    key_t clave = ftok(ARCHIVO_CLAVE, CLAVE_PROYECTO);
    if (clave == -1) {
        perror("Error en ftok");
        exit(EXIT_FAILURE);
    }
    
    printf("Clave generada: %ld\n", (long) clave);
    
    // Crear la cola de mensajes
    // IPC_CREAT | 0666: crear si no existe, permisos rw para todos
    int msqid = msgget(clave, IPC_CREAT | 0666);
    if (msqid == -1) {
        perror("Error en msgget");
        exit(EXIT_FAILURE);
    }
    
    printf("Cola de mensajes creada con ID: %d\n", msqid);
    
    // Fork: crear proceso hijo
    pid_t pid = fork();
    
    if (pid == -1) {
        perror("Error en fork");
        // Eliminar la cola antes de salir
        msgctl(msqid, IPC_RMID, NULL);
        exit(EXIT_FAILURE);
    }
    
    if (pid == 0) {
        // Proceso hijo (consumidor)
        proceso_hijo(msqid);
        exit(EXIT_SUCCESS);
    } else {
        // Proceso padre (productor)
        proceso_padre(msqid);
        
        // Esperar a que el hijo termine
        wait(NULL);
        
        // Limpiar: eliminar la cola de mensajes
        if (msgctl(msqid, IPC_RMID, NULL) == -1) {
            perror("Error al eliminar la cola con msgctl");
            exit(EXIT_FAILURE);
        }
        
        printf("[PADRE] Cola de mensajes eliminada. Programa finalizado.\n");
    }
    
    return EXIT_SUCCESS;
}
