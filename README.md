*Este proyecto ha sido creado como parte del currículo de 42 por jocorral.*

# ft_printf

## Descripción

`ft_printf` es una reimplementación de la función `printf()` de la `libc`. El objetivo es aprender a usar funciones variádicas (las que reciben un número
variable de argumentos) con `<stdarg.h>`. El resultado es la librería `libftprintf.a`, con una única función pública:

```c
int	ft_printf(char const *format, ...);
```

Igual que `printf`, escribe en la salida estándar y devuelve el número de caracteres impresos.

Conversiones soportadas: `%c`, `%s`, `%p`, `%d`, `%i`, `%u`, `%x`, `%X` y `%%`.

El bonus añade el formato `%[flags][ancho][.precisión]conversión`, con las flags `-`, `0`, `#`, `+` y espacio.

## Instrucciones

Compilar (genera `libftprintf.a` en la raíz del repo):

```sh
make
```

Otras reglas del `Makefile`:

```sh
make bonus    # compila la librería con la versión bonus
make clean    # borra obj/ (la de printf y la de libft)
make fclean   # borra además libftprintf.a y libft/libft.a
make re       # fclean + all
```

La libft está en `libft/` con su propio `Makefile`. El de `ft_printf` la compila primero y mete sus objetos dentro de `libftprintf.a`, así que solo hay que
enlazar esa librería:

```c
#include "ft_printf.h"

int	main(void)
{
	ft_printf("Hola %s, tienes %d años\n", "mundo", 42);
	return (0);
}
```

```sh
cc -Wall -Wextra -Werror main.c -L. -lftprintf && ./a.out
```

### Probar sin Makefile

`ft_printf.c` y `ft_printf_format_bonus.c` tienen al final, dentro de un comentario `/* ... */`, un `main` de prueba.

1. Descomenta el `main` del archivo que quieras probar.
2. Compila la versión que corresponda:

```sh
cc -Wall -Wextra -Werror ft_printf.c ft_printf_utils.c ft_printf_format.c libft/ft_*.c && ./a.out    # obligatoria
cc -Wall -Wextra -Werror ft_printf.c ft_printf_utils.c ft_printf_*_bonus.c libft/ft_*.c && ./a.out   # bonus
```

3. Vuelve a comentar el bloque antes de hacer commit.

No se puede compilar con `*.c`: `ft_format` está definida en `ft_printf_format.c` y en `ft_printf_format_bonus.c`, y solo puede enlazarse una de las dos.

## Algoritmo y estructura de datos

`ft_printf` recorre la cadena de formato una sola vez. Si el carácter no es `%`, lo imprime. Si es `%`, avanza al siguiente y llama a `ft_format`, que
acaba en `ft_parser`: una cadena de `if / else if` que saca el argumento con `va_arg` del tipo adecuado y llama a la función que lo imprime. Cada función
devuelve cuántos caracteres ha escrito y `ft_printf` los va sumando.

Lo más delicado:

- Todos los números se imprimen con una sola función recursiva, `ft_putnbr_ubase`: primero se llama a sí misma con `nbr / base` y después imprime el dígito
  `nbr % base`, así las cifras salen en orden sin guardarlas en ningún sitio. La base se pasa como cadena (`"0123456789"`, `"0123456789abcdef"`...), por eso
  la misma función sirve para `%u`, `%x`, `%X` y `%p`.
- `ft_putnbr_base` (la de `%d` y `%i`) recibe un `long`, no un `int`, para poder negar `INT_MIN` sin desbordar.
- El `va_list` se pasa por puntero (`va_list *`). Si se pasara por valor, al volver de la función no estaría garantizado que la lista hubiera avanzado.
- No hay buffer ni `malloc`: cada carácter se escribe al momento con `write`. Es menos eficiente que el `printf` real, pero no puede haber leaks y el
  contador de caracteres es siempre exacto.

En el bonus, `ft_format` lee antes las flags, el ancho y la precisión y los guarda en una estructura `t_fmt`, que es lo único que necesitan las funciones de
impresión para saber cuánto relleno poner y dónde.

## Recursos

- Páginas de manual: `man 3 printf` y `man 3 stdarg`.
- [cppreference.com](https://en.cppreference.com/w/c/io/fprintf) para el comportamiento exacto de cada flag y de las funciones variádicas.
- La Norma de 42 (documento del cursus), para las reglas de estilo.

La IA se ha usado para corregir errores de Norma y para redactar este README. Todo el código ha sido revisado, probado y entendido antes de entregarlo.
