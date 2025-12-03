# 4. IPC con System V Message Queues

## ¿Qué es IPC (Inter-Process Communication)?

**IPC** es un mecanismo que permite a dos o más procesos independientes comunicarse e intercambiar datos. En sistemas Unix/Linux, existen varios métodos para realizar IPC:

- **Pipes**: Comunicación unidireccional entre procesos relacionados
- **Sockets**: Comunicación bidireccional entre procesos locales o remotos
- **Memoria Compartida**: Acceso directo a una región de memoria común
- **Colas de Mensajes (System V)**: Sistema clásico para intercambio de mensajes discretos entre procesos
- **Signals**: Mecanismo de notificación asincrónico

### ¿Por qué elegimos Colas de Mensajes?

Las **Colas de Mensajes System V** nos proporcionan:
- **Decoupling**: Padre e hijo no necesitan sincronizarse estrictamente
- **Seguridad**: El kernel maneja la sincronización y protección
- **Flexibilidad**: Múltiples procesos pueden leer/escribir en la misma cola
- **Persistencia**: Los mensajes persisten mientras la cola exista
- **Tipificación**: Los mensajes pueden clasificarse por tipo (mtype)

## Cómo se comunican Padre e Hijo

El programa realiza un **fork()** para crear un proceso hijo que comparte la misma cola de mensajes del padre. El padre actúa como **productor** enviando 5 números (1 al 5) mediante **msgsnd()**, mientras que el hijo actúa como **consumidor**, recibiendo los mensajes con **msgrcv()** y procesándolos (multiplicando por 2). La comunicación es **asincrónica**: el padre puede enviar mensajes sin esperar a que el hijo los procese inmediatamente. Cuando el padre termina de enviar datos, envía un mensaje especial (mtype=255) para indicar fin de la transmisión, permitiendo que el hijo cierre su bucle de recepción de forma ordenada.

## Flujo del Programa (generado con IA)

```
┌─────────────────────────────────────────────────────────────┐
│ 1. Programa Principal                                        │
│    - ftok() → Genera clave única                            │
│    - msgget() → Crea cola de mensajes                       │
│    - fork() → Divide en padre e hijo                        │
└─────────────────────────────────────────────────────────────┘
                            │
                  ┌─────────┴─────────┐
                  ▼                   ▼
        ┌─────────────────┐  ┌──────────────────┐
        │ Proceso PADRE   │  │ Proceso HIJO     │
        │ (Productor)     │  │ (Consumidor)     │
        ├─────────────────┤  ├──────────────────┤
        │ Envía (msgsnd): │  │ Recibe (msgrcv): │
        │ 1 (x2 = 2)      │→ │ 1 (x2 = 2)       │
        │ 2 (x2 = 4)      │→ │ 2 (x2 = 4)       │
        │ 3 (x2 = 6)      │→ │ 3 (x2 = 6)       │
        │ 4 (x2 = 8)      │→ │ 4 (x2 = 8)       │
        │ 5 (x2 = 10)     │→ │ 5 (x2 = 10)      │
        │ FIN (255)       │→ │ Termina          │
        │                 │  │                  │
        │ msgctl(RMID)    │  │                  │
        │ Limpia cola     │  │                  │
        └─────────────────┘  └──────────────────┘
```

## Estructura de Datos

```c
typedef struct {
    long mtype;    // Tipo de mensaje (1=normal, 255=fin)
    int valor;     // Dato entero a comunicar
} Mensaje;
```

## Funciones System V Utilizadas

| Función | Propósito |
|---------|-----------|
| `ftok()` | Genera una clave IPC única a partir de un archivo y proyecto |
| `msgget()` | Obtiene o crea una cola de mensajes |
| `msgsnd()` | Envía un mensaje a la cola |
| `msgrcv()` | Recibe un mensaje de la cola |
| `msgctl()` | Controla la cola (eliminar con IPC_RMID) |

## Compilación y Ejecución

### Requisitos
- GCC compilador
- Linux/Unix con soporte para System V IPC
- Make

### Compilar
```bash
make
```

### Ejecutar
```bash
make run
```

### Compilar y ejecutar en un solo comando
```bash
make clean && make run
```

### Limpiar archivos generados
```bash
make clean
```

## Comportamiento Esperado en Pantalla

```
Clave generada: 1111
Cola de mensajes creada con ID: 1049600
[PADRE PID=12345] Iniciando proceso productor
[HIJO PID=12346] Iniciando proceso consumidor
Padre envía: 1
Padre envía: 2
Padre envía: 3
Padre envía: 4
Padre envía: 5
Hijo recibe 1, resultado = 2
Hijo recibe 2, resultado = 4
Hijo recibe 3, resultado = 6
Hijo recibe 4, resultado = 8
Hijo recibe 5, resultado = 10
[PADRE] Mensaje de fin enviado
[HIJO] Mensaje de fin recibido. Terminando...
[PADRE] Cola de mensajes eliminada. Programa finalizado.
```

**Nota**: El orden exacto de los mensajes "Padre envía" e "Hijo recibe" puede variar ligeramente debido a la naturaleza asincrónica de los procesos, pero todos los mensajes se entregarán correctamente.

## Aspectos Importantes del Código

1. **ftok()**: Genera una clave única basada en un archivo existente y un identificador de proyecto
2. **IPC_CREAT | 0666**: Crea la cola si no existe, con permisos de lectura/escritura
3. **mtype = 0 en msgrcv()**: Recibe el primer mensaje de cualquier tipo disponible
4. **IPC_RMID en msgctl()**: Elimina la cola cuando el padre termina
5. **fork()**: Divide el proceso; ambos comparten el msqid automáticamente
6. **wait()**: El padre espera a que el hijo termine antes de limpiar

## Gestión de Errores

El código implementa manejo robusto de errores:
- Validación de `ftok()`, `msgget()`, `fork()`, `msgsnd()` y `msgrcv()`
- Uso de `perror()` para mensajes de error descriptivos
- Limpieza de recursos (cola) incluso en casos de error
- `EXIT_FAILURE` y `EXIT_SUCCESS` para códigos de retorno claros

