# Tarea 1 - File System
Conteste las siguientes preguntas:
##### - Cuáles serían los metadatos del filesystem? <- Esos serían atributos del file system
Nombre
Tamaño Total
Tamaño del bloque 
Espacio libre 
Tipo
Identificador
Protección 
Bitmap
nodo raíz

##### - Cuáles serían los metadatos de un archivo? <-Esos serían los atributos del archivo, quizás almacenados en un nodo índice
Nombre
Tamaño
Ubicación
Permisos de acceso 
Ubicación
Fecha de creación
Estado (Abierto, cerrado)
Tipo de archivo
Identificador 
            
##### - Quiero poder direccionar bloques de datos directamente? <- Entonces el nodo del índice qué tendría para esto?
Si, ya que al tener accesos directos a los bloques esto va ayudar a acceder con facilitar, ahora bien existen una cantidad limitada de espacio lo cual puede ser una desventaja. Sin embargo con idirección se puede solventar. 

##### - Quiero poder hacer que los archivos sean más grandes <- Entonces el nodo del índice qué tendría para esto? Cómo se hace para que un archivo pueda ser más grande?
Para esto se deben agregar nuevos bloques de datos y se agregan al indice. Por ende el indice debe de tener un arreglo de punteros a bloques de datos y un espacio para almacenar el tamaño actual del archivo. 

##### - Cómo se implementan los folders? <- Donde se guarda el hecho de que un folder es un folder?
Este se implementa mediante una clase llamada directorio, la cual posee una tabla donde relaciona el nombre de los archivos con con un registro logico. Esta clase como mínimo debe de tener las siguientes funciones crear un archivo, eliminar un archivo, listar un directorio, renombrar archivo y recorrer el system file (dentro del directorio).

##### - Cómo se mantiene el control de permisos de acceso a archivos?
El control de acceso se realiza mediante la dependencia de la identidad del usuario, ya que diversos usuarios pueden necesitar diferentes tipos de acceso. Para esto se utiliza una lista de control de acceso (ACL), en la cual se almacenan los nombres de usuario y los tipos de acceso.

##### - Cómo se controla que no haya errores en el funcionamiento general del file system?

##### - Cómo se pueden modificar los metadatos de un archivo?
Esto se puede realizar mediante funciones que permiten modificar los metadatos según los permisos del usuario en el directorio. Lo cual también se va a reflejar en los metadatos del archivo.

##### - Cómo se puede conocer el tamaño de un archivo?
El tamaño es un atributo que se guarda en el nodo indice como un metadato. Por ende el sistema puede consultar este valor.

##### - Qué estructuras de datos son indispensables?
Arreglos
Hash maps
Nodos indice (inodes)
Directorios

Haga un diseño de un sistema de archivos de tipo indexado que incorpore las respuestas a estas preguntas junto con la especificación del funcamiento, las operaciones primordiales sobre archivos, los metadatos de archivos, los componentes principales, la interacción entre ellos, la especificación de lo qué son y lo que hacen y la definición de lo mínimo requerido para la implementación.
Programe el sistema de archivos los componentes principales, la interacción entre ellos, la especificación de lo qué son y lo que hacen y la definición de lo mínimo requerido para la implementación, en un repositorio de REPL.IT, compartido con el docente, para almacenar datos de dibujos ascii según las especificaciones del proyecto de PI.

Por ej, 

int buscarArchivo() // devuelve el bloque inicial del archivo, -1 en caso de fallo

char[] Leer(archivo, int nbytes) // devuelve msj de error en caso de fallo

bool Escribir(archivo, char [] datos) // devuelve falso en caso de fallo

bool Renombrar(archivo, nombreNuevo) // devuelve falso en caso de fallo
Defina la manera en la que se puede probar la funcionalidad del sistema de archivos.
Prepare un video con la demostración del sistema de archivos incluyendo su funcionalidad, sus componentes dificultades encontradas y deficiencias aún existentes en la entrega.


