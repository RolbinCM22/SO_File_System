## Planteamiento inicial:
a. Se utilizará codespaces de github, esto para evitar incompatibilidades entre sistemas y computadoras.

b. Se utilizará el software de virtualización de QEMU por recomendación del profesor y por conveniencia al usar codespace de github. Además, QEMU posibilita cosas que serán útiles para el proyecto:

- Arrancar tu propio núcleo sin tener que instalar un sistema operativo entero

- Simular registros, APIC, MMU y entradas/salidas de bajo nivel

- Ver logs del CPU, disasm y tracer

- Configura tu propio BIOS (OVMF/UEFI, SeaBIOS).

- Simular errores de hardware

- Depurar con GDB directamente sobre el núcleo

c. Para probar las decisiones con los boots dados por el profe se esamblan los .asm con nasm:

- nasm -f bin boot1.asm -o boot_basico.bin

- nasm -f bin boot2.asm -o boot_menu.bin

Esto crea dos archivos binarios sin encabezado listos para bootear y se prueban con QEMU con el siguiente comando:

- qemu-system-x86_64 -drive format=raw,file=boot_(basico o menu).bin

- qemu-system-x86_64 -drive format=raw,file=boot1.bin -nographic (este es para el codespace ya que no cuenta con las herramientas para mostralo con interfaz gráfica)

e. Descripción y análisis de ejemplos:

# S.O mínimo:

El boot sector se carga en memoria en la dirección 0x7c00 por la BIOS al inicio del arranque del sistema.

Comienza deshabilitando interrupciones con el cli y poniendo en cero los registros de segmento ds, es, ss, y establece stack pointer (sp) en 0x7c00. Luego vuelve a habilitar interrupciones con el sti.

El código usa el registro si para apuntar al mensaje msg que contiene la cadena “hi!” terminada en 0.

En el bucle .print_loop, carga byte a byte el contenido apuntado por si en al con lodsb, y verifica si el byte es 0 para terminar. Si no es 0, usa la interrupción BIOS int 0x10 con función 0x0E para imprimir el carácter en texto en pantalla.

Una vez que el mensaje se imprime completo, deshabilita interrupciones de nuevo y entra en un bucle infinito que ejecuta la instrucción hlt para detener la CPU.

Finalmente, llena con ceros hasta los 510 bytes, y en las dos últimas posiciones pone el número mágico 0xAA55 que indica a la BIOS que es un sector de arranque válido.

# S.O con menú: 

El código arranca igual que el anterior, con una deshabilitación de interrupciones (cli), inicializa registros de segmento
y el puntero de pila con valor 0x7c00, vuelve a habilitar interrupciones (sti). Luego limpia la pantalla con la rutina limpiarPantalla. Seguidamente entra al main_loop donde se muestra el menú con las opciones disponibles (dibujar cuadrado, leer texto, imprimir texto, limpiar pantalla, salir).

Opción 1 (dibujeCuadrado): pregunta por el tamaño del lado del cuadrado, y dibuja el cuadrado con * en la pantalla línea por línea, colocando el cursor mediante interrupciones BIOS de video.

Opción 2 (leerTexto): lee texto del teclado carácter por carácter hasta enter, permitiendo backspace para borrar, y guarda el texto en segmento de memoria 0x9000.

Opción 3 (imprimaTexto): imprime el texto previamente guardado desde el segmento 0x9000.

Opción 4: limpia la pantalla.

Opción 5: imprime mensaje de salida y detiene la CPU con hlt. Cada opción regresa al menú principal tras finalizar, permitiendo interacción simple en modo texto.

Termina igual que el anterior, llenando con ceros hasta 510 bytes y finaliza con 0xAA55 para garantizar que sea reconocido como boot sector.


