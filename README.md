*Este proyecto ha sido creado como parte del currículo de 42 por jocorral.*

# ft_printf

## Descripción

`ft_printf` es una reimplementación de la función `printf()` de la `libc`. El objetivo es aprender a usar funciones variádicas (las que reciben un número
variable de argumentos) con `<stdarg.h>`. El resultado es la librería `libftprintf.a`, con una única función pública:

```c
int	ft_printf(char const *format, ...);
```

Igual que `printf`, escribe en la salida estándar y devuelve el número de caracteres impresos.

Conversiones soportadas: `%c`, `%s`, `%p`, `%d`, `%i`, `%u`, `%x`, `%X` y `%%`. Como el `printf` de Linux, una cadena `NULL` se imprime como `(null)` y un
puntero `NULL` como `(nil)`. Si la conversión no existe, imprime el `%` y la letra tal cual.

El bonus añade el ancho, la precisión y las flags `-`, `0`, `#`, `+` y espacio (ver la sección [Bonus](#bonus)).

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

## Bonus

`make bonus` genera la misma librería, pero aceptando el formato completo `%[flags][ancho][.precisión]conversión`:

| Elemento | Efecto | Ejemplo | Salida |
|---|---|---|---|
| ancho | Mínimo de caracteres; rellena con espacios a la izquierda | `"[%5d]", 42` | `[   42]` |
| `-` | Alinea a la izquierda | `"[%-5d]", 42` | `[42   ]` |
| `0` | Rellena con ceros en vez de espacios | `"[%05d]", -42` | `[-0042]` |
| `.` | Precisión: mínimo de dígitos en números, máximo de caracteres en `%s` | `"[%.3d]", 7` / `"[%.2s]", "hola"` | `[007]` / `[ho]` |
| `#` | Añade `0x` / `0X` en `%x` / `%X` si el número no es 0 | `"[%#x]", 255` | `[0xff]` |
| `+` | Muestra siempre el signo en `%d` / `%i` | `"[%+d]", 42` | `[+42]` |
| espacio | Deja un espacio delante de los positivos | `"[% d]", 42` | `[ 42]` |

Como en el `printf` real, `-` y la precisión anulan a `0`, y `+` anula al espacio. `+` y espacio también valen para `%p` (`+0x...`, ` 0x...`). Una
cadena `NULL` con una precisión menor que 6 no imprime nada, en vez de un `(null)` cortado.

Entre la parte obligatoria y el bonus solo cambia una función, `ft_format`; `ft_printf`, `ft_parser` y `ft_printf_utils.c` son los mismos archivos en las
dos versiones. En la obligatoria (`ft_printf_format.c`) `ft_format` solo llama a `ft_parser`. En el bonus (`ft_printf_format_bonus.c`) hace tres cosas:

1. Lee las flags, el ancho y la precisión y los guarda en una estructura `t_format`. Es una variable local, así que sigue sin haber `malloc`. La precisión
   empieza en `-1` para distinguir "no hay precisión" de "precisión 0" (`%.0d` con un 0 no imprime nada).
2. Avanza el puntero de `ft_printf` hasta la letra de la conversión; por eso recibe la cadena de formato como `char const **`.
3. Llama a la función de esa conversión (`ft_printf_conv_bonus.c`). `%%` y los casos raros se siguen delegando en `ft_parser`.

Lo más delicado es que todos los números (`%d`, `%i`, `%u`, `%x`, `%X` y `%p`) acaban en una sola función, `ft_put_nbr` (`ft_printf_nbr_bonus.c`). Las
funciones de conversión no imprimen nada: solo eligen la base y un *prefijo* (`-`, `+`, espacio, `0x`...) y le pasan el número ya en positivo. `ft_put_nbr`
calcula cuántas cifras y cuántos ceros hacen falta y escribe siempre en el mismo orden:

```
[espacios] [prefijo] [ceros] [dígitos] [espacios]
```

Como el prefijo va antes de los ceros, `%05d` con -42 da `-0042` y no `00-42`. Los dígitos los sigue imprimiendo `ft_putnbr_ubase`, la misma función de la
parte obligatoria.

## Recursos

- Páginas de manual: `man 3 printf` y `man 3 stdarg`.
- [cppreference.com](https://en.cppreference.com/w/c/io/fprintf) para el comportamiento exacto de cada flag y de las funciones variádicas.
- La Norma de 42 (documento del cursus), para las reglas de estilo.

La IA se ha usado para corregir errores de Norma y para redactar este README. Todo el código ha sido revisado, probado y entendido antes de entregarlo.

## Estructura

```
ft_printf.h               cabecera con los prototipos de la parte obligatoria
ft_printf.c               ft_printf y ft_parser (comunes a las dos versiones)
ft_printf_utils.c         ft_putchar, ft_putstr, ft_putnbr_base, ft_putnbr_ubase y puntero
ft_printf_format.c        ft_format de la parte obligatoria
ft_printf_bonus.h         t_format y los prototipos del bonus
ft_printf_format_bonus.c  ft_format del bonus: lee flags, ancho y precisión
ft_printf_conv_bonus.c    una función por conversión: elige base y prefijo
ft_printf_nbr_bonus.c     ft_put_nbr y ft_putpad: relleno, prefijo, ceros y dígitos
libft/                    la libft, con su propio Makefile
Makefile                  all / bonus / clean / fclean / re
obj/                      .o generados por make (no se sube)
libftprintf.a             librería final (no se sube)
```
