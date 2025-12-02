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
 * @struct Mensaje
 * @brief Estructura de mensaje para colas de mensajes System V.
 *
 * La estructura contiene un tipo de mensaje (@c mtype) y un valor entero
 * (@c valor) que será transmitido entre procesos. El campo @c mtype es
 * obligatorio y debe ser mayor que cero para mensajes válidos.
 */
typedef struct {
    long mtype;    /**< Tipo de mensaje (debe ser > 0 para mensajes normales). */
    int valor;     /**< Dato entero a transmitir entre procesos. */
} Mensaje;

/** @brief Clave de proyecto usada por ftok() para generar la clave de la cola. */
#define CLAVE_PROYECTO 'I'

/**
 * @brief Ruta de archivo base utilizada por ftok().
 *
 * La combinación de esta ruta y CLAVE_PROYECTO genera una clave única
 * para identificar la cola de mensajes dentro del sistema.
 */
#define ARCHIVO_CLAVE "/tmp"

/** @brief Tipo de mensaje para mensajes normales de datos. */
#define MTYPE_NORMAL 1

/** @brief Tipo de mensaje para indicar fin de comunicación. */
#define MTYPE_FIN 255

/** @brief Cantidad de mensajes de datos que el padre enviará al hijo. */
#define NUM_MENSAJES 5

/**
 * @brief Lógica del proceso padre (productor).
 *
 * Envía una secuencia de @c NUM_MENSAJES números enteros (del 1 al 5)
 * al proceso hijo a través de la cola de mensajes indicada por @p msqid.
 * Posteriormente envía un mensaje especial de fin con tipo @c MTYPE_FIN
 * para indicar al hijo que no habrá más datos.
 *
 * @param msqid Identificador de la cola de mensajes (devuelto por msgget()).
 */
void proceso_padre(int msqid) {
    printf("[PADRE PID=%d] Iniciando proceso productor\n", getpid());

    Mensaje msg;
    msg.mtype = MTYPE_NORMAL;

    /* Enviar NUM_MENSAJES números al hijo */
    for (int i = 1; i <= NUM_MENSAJES; i++) {
        msg.valor = i;

        /* msgsnd: enviar mensaje a la cola */
        if (msgsnd(msqid, (void *)&msg, sizeof(msg.valor), 0) == -1) {
            perror("[PADRE] Error en msgsnd");
            exit(EXIT_FAILURE);
        }
        printf("Padre envía: %d\n", i);
    }

    /* Enviar mensaje de fin (mtype = MTYPE_FIN) */
    msg.mtype = MTYPE_FIN;
    msg.valor = 0;

    if (msgsnd(msqid, (void *)&msg, sizeof(msg.valor), 0) == -1) {
        perror("[PADRE] Error al enviar mensaje de fin");
        exit(EXIT_FAILURE);
    }
    printf("[PADRE] Mensaje de fin enviado\n");
}

/**
 * @brief Lógica del proceso hijo (consumidor).
 *
 * Recibe mensajes desde la cola identificada por @p msqid. Mientras reciba
 * mensajes con tipo @c MTYPE_NORMAL, procesa el valor entero asociado
 * (multiplicándolo por 2) y muestra el resultado por pantalla. Cuando
 * recibe un mensaje con tipo @c MTYPE_FIN, detiene el bucle y finaliza.
 *
 * @param msqid Identificador de la cola de mensajes (devuelto por msgget()).
 */
void proceso_hijo(int msqid) {
    printf("[HIJO PID=%d] Iniciando proceso consumidor\n", getpid());

    Mensaje msg;

    /* Bucle para recibir mensajes */
    while (1) {
        /*
         * msgrcv: recibir mensaje de la cola
         * mtype = 0 significa recibir el primer mensaje disponible,
         * sin filtrar por tipo específico.
         */
        if (msgrcv(msqid, (void *)&msg, sizeof(msg.valor), 0, 0) == -1) {
            perror("[HIJO] Error en msgrcv");
            exit(EXIT_FAILURE);
        }

        /* Si recibimos el mensaje de fin (mtype = MTYPE_FIN), terminamos */
        if (msg.mtype == MTYPE_FIN) {
            printf("[HIJO] Mensaje de fin recibido. Terminando...\n");
            break;
        }

        /* Procesar mensaje normal (mtype = MTYPE_NORMAL) */
        if (msg.mtype == MTYPE_NORMAL) {
            int resultado = msg.valor * 2;
            printf("Hijo recibe %d, resultado = %d\n", msg.valor, resultado);
        }
    }
}

/**
 * @brief Punto de entrada principal del programa.
 *
 * Genera una clave única con ftok(), crea (o obtiene) una cola de mensajes
 * con msgget() y luego realiza un fork() para crear un proceso hijo. El
 * proceso padre actúa como productor (llamando a proceso_padre()) y el
 * proceso hijo actúa como consumidor (llamando a proceso_hijo()).
 *
 * Una vez que el hijo termina, el padre espera su finalización y elimina
 * la cola de mensajes mediante msgctl() con la opción IPC_RMID.
 *
 * @return EXIT_SUCCESS si el programa termina correctamente,
 *         o EXIT_FAILURE en caso de error.
 */
int main() {
    /* Generar clave única mediante ftok */
    key_t clave = ftok(ARCHIVO_CLAVE, CLAVE_PROYECTO);
    if (clave == -1) {
        perror("Error en ftok");
        exit(EXIT_FAILURE);
    }

    printf("Clave generada: %ld\n", (long) clave);

    /*
     * Crear la cola de mensajes.
     * IPC_CREAT | 0666: crear si no existe, permisos rw para todos.
     */
    int msqid = msgget(clave, IPC_CREAT | 0666);
    if (msqid == -1) {
        perror("Error en msgget");
        exit(EXIT_FAILURE);
    }

    printf("Cola de mensajes creada con ID: %d\n", msqid);

    /* fork: crear proceso hijo */
    pid_t pid = fork();

    if (pid == -1) {
        perror("Error en fork");
        /* Eliminar la cola antes de salir en caso de error */
        msgctl(msqid, IPC_RMID, NULL);
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        /* Proceso hijo (consumidor) */
        proceso_hijo(msqid);
        exit(EXIT_SUCCESS);
    } else {
        /* Proceso padre (productor) */
        proceso_padre(msqid);

        /* Esperar a que el hijo termine */
        wait(NULL);

        /* Limpiar: eliminar la cola de mensajes */
        if (msgctl(msqid, IPC_RMID, NULL) == -1) {
            perror("Error al eliminar la cola con msgctl");
            exit(EXIT_FAILURE);
        }

        printf("[PADRE] Cola de mensajes eliminada. Programa finalizado.\n");
    }

    return EXIT_SUCCESS;
}