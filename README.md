*Este proyecto ha sido creado como parte del currículo de 42 por jocorral.*

# ft_printf

## Descripción

`ft_printf` es una reimplementación de la función `printf()` de la librería estándar de C. El objetivo del proyecto es aprender a trabajar con **funciones variádicas** (funciones que reciben un número variable de argumentos) mediante `<stdarg.h>`, y escribir código bien estructurado y fácil de ampliar.

El resultado es la librería estática `libftprintf.a`, que expone la función:

```c
int	ft_printf(const char *format, ...);
```

Igual que `printf`, escribe en la salida estándar y devuelve el número de caracteres impresos.

### Conversiones soportadas

| Conversión | Qué imprime | Ejemplo |
|---|---|---|
| `%c` | Un carácter | `ft_printf("%c", 'A')` → `A` |
| `%s` | Una cadena (`(null)` si es `NULL`) | `ft_printf("%s", "hola")` → `hola` |
| `%p` | Un puntero en hexadecimal con `0x` (`(nil)` si es `NULL`) | `ft_printf("%p", &x)` → `0x7ffd…` |
| `%d` | Un entero con signo en base 10 | `ft_printf("%d", -42)` → `-42` |
| `%i` | Un entero con signo en base 10 | `ft_printf("%i", 42)` → `42` |
| `%u` | Un entero sin signo en base 10 | `ft_printf("%u", -1)` → `4294967295` |
| `%x` | Un entero sin signo en hexadecimal en minúsculas | `ft_printf("%x", 255)` → `ff` |
| `%X` | Un entero sin signo en hexadecimal en mayúsculas | `ft_printf("%X", 255)` → `FF` |
| `%%` | El símbolo `%` | `ft_printf("100%%")` → `100%` |

## Instrucciones

### Compilación

```sh
make        # compila libftprintf.a (los .o se guardan en obj/)
make clean  # borra la carpeta obj/
make fclean # borra obj/ y libftprintf.a
make re     # fclean + make
```

Se compila con `cc -Wall -Wextra -Werror` y la librería se crea con `ar rcs`.

### Uso

Incluye la cabecera y enlaza la librería:

```c
#include "ft_printf.h"

int	main(void)
{
	int	n;

	n = ft_printf("Hola %s, tienes %d años\n", "mundo", 42);
	ft_printf("Se han impreso %d caracteres\n", n);
	return (0);
}
```

```sh
cc -Wall -Wextra -Werror main.c -L. -lftprintf -o programa
./programa
```

## Algoritmo y estructura de datos

### Algoritmo

`ft_printf` recorre la cadena de formato **una sola vez**, carácter a carácter:

1. Si el carácter no es `%`, se imprime directamente.
2. Si es `%`, se mira el carácter siguiente (el *especificador*) y se pasa a `parser`, que saca el siguiente argumento con `va_arg` del tipo adecuado y llama a la función que lo imprime.
3. Cada función de impresión devuelve cuántos caracteres ha escrito, y `ft_printf` va sumando esos valores para devolver el total.

Si el `%` está al final de la cadena, se ignora. Si va seguido de un especificador desconocido, se imprimen el `%` y el carácter tal cual.

Todos los números se imprimen con una única función recursiva, `ft_putnbr_ubase(unsigned long nbr, char *base)`:

- Primero imprime recursivamente `nbr / base`, y después el dígito `nbr % base`. Así las cifras salen en el orden correcto sin tener que guardarlas en un buffer ni darles la vuelta.
- La base se pasa como cadena (`"0123456789"`, `"0123456789abcdef"`, `"0123456789ABCDEF"`), de modo que la misma función sirve para `%u`, `%x`, `%X` y `%p`.
- `ft_putnbr_base(long nbr, char *base)` se encarga de los números con signo (`%d`, `%i`): si el número es negativo imprime `-` y llama a la versión sin signo con el valor absoluto. Como trabaja con `long`, `INT_MIN` se puede negar sin desbordamiento y no necesita un caso especial.
- `%p` imprime `0x` y luego la dirección convertida a `unsigned long`, que en sistemas de 64 bits tiene el mismo tamaño que un puntero.

### Estructura de datos

El proyecto no necesita estructuras de datos complejas, y esto es una decisión deliberada:

- **No hay buffer intermedio.** Cada carácter se escribe al momento con `write(1, …)`. Es menos eficiente que agrupar la salida (lo que hace el `printf` real), pero el código es más sencillo, no requiere `malloc` (así que no puede haber fugas de memoria) y el contador de caracteres es exacto en todo momento.
- **El `va_list` se pasa por puntero** (`va_list *`) a `parser`. Si se pasa por valor y la función llamada usa `va_arg`, el estándar de C no garantiza el estado del `va_list` en la función que llama. Pasando un puntero, todas las funciones avanzan sobre la misma lista de argumentos.
- **El despacho se hace con una cadena de `if / else if`** en `parser`. Con solo nueve especificadores es lo más legible, y añadir uno nuevo consiste en añadir una rama.

### Organización de archivos

| Archivo | Contenido |
|---|---|
| `ft_printf.h` | Prototipos e includes (`<stdarg.h>`, `<unistd.h>`) |
| `ft_printf.c` | `ft_printf` (recorre el formato) y `parser` (elige qué imprimir) |
| `ft_printf_utils.c` | `ft_putchar`, `ft_putstr`, `ft_putnbr_base`, `ft_putnbr_ubase` y `puntero` |

## Recursos

- [`man 3 printf`](https://man7.org/linux/man-pages/man3/printf.3.html): comportamiento de referencia de cada conversión.
- [`man 3 stdarg`](https://man7.org/linux/man-pages/man3/stdarg.3.html): `va_start`, `va_arg`, `va_copy` y `va_end`.
- [cppreference: Variadic functions](https://en.cppreference.com/w/c/variadic): explicación de las funciones variádicas en C.
- [cppreference: printf](https://en.cppreference.com/w/c/io/fprintf): especificación de los formatos.
- Norma de 42: reglas de estilo que debe cumplir el código.

#### Uso de IA
- Corregir errores de Norma.
- Redactar este README.

Todo el código ha sido revisado, probado y entendido antes de entregarlo.
