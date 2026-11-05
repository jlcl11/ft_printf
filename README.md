*This project has been created as part of the 42 curriculum by jocorral.*

# ft_printf

## Description

`ft_printf` is a reimplementation of the `libc` function `printf()`. The goal is to learn how to use variadic functions (those that take a variable
number of arguments) with `<stdarg.h>`. The result is the `libftprintf.a` library, with a single public function:

```c
int	ft_printf(char const *format, ...);
```

Like `printf`, it writes to standard output and returns the number of characters printed.

Supported conversions: `%c`, `%s`, `%p`, `%d`, `%i`, `%u`, `%x`, `%X` and `%%`. Like Linux `printf`, a `NULL` string is printed as `(null)` and a
`NULL` pointer as `(nil)`. If the conversion does not exist, it prints the `%` and the letter as they are.

The bonus adds width, precision and the `-`, `0`, `#`, `+` and space flags (see the [Bonus](#bonus) section).

## Instructions

Build (generates `libftprintf.a` at the root of the repo):

```sh
make
```

Other `Makefile` rules:

```sh
make bonus    # builds the library with the bonus version
make clean    # removes obj/ (both printf's and libft's)
make fclean   # also removes libftprintf.a and libft/libft.a
make re       # fclean + all
```

Libft lives in `libft/` with its own `Makefile`. The `ft_printf` one builds it first and puts its objects inside `libftprintf.a`, so you only need to
link that library:

```c
#include "ft_printf.h"

int	main(void)
{
	ft_printf("Hello %s, you are %d years old\n", "world", 42);
	return (0);
}
```

```sh
cc -Wall -Wextra -Werror main.c -L. -lftprintf && ./a.out
```

### Testing without the Makefile

`ft_printf.c` and `ft_printf_format_bonus.c` have a test `main` at the end, inside a `/* ... */` comment.

1. Uncomment the `main` of the file you want to test.
2. Compile the matching version:

```sh
cc -Wall -Wextra -Werror ft_printf.c ft_printf_utils.c ft_printf_format.c libft/ft_*.c && ./a.out    # mandatory
cc -Wall -Wextra -Werror ft_printf.c ft_printf_utils.c ft_printf_*_bonus.c libft/ft_*.c && ./a.out   # bonus
```

3. Comment the block out again before committing.

You cannot compile with `*.c`: `ft_format` is defined in both `ft_printf_format.c` and `ft_printf_format_bonus.c`, and only one of the two can be linked.

## Algorithm and data structure

`ft_printf` goes through the format string once. If the character is not `%`, it prints it. If it is `%`, it moves to the next one and calls `ft_format`, which
ends up in `ft_parser`: a chain of `if / else if` that pulls the argument of the right type with `va_arg` and calls the function that prints it. Each function
returns how many characters it wrote and `ft_printf` adds them up.

The trickiest parts:

- All numbers are printed with a single recursive function, `ft_putnbr_ubase`: it first calls itself with `nbr / base` and then prints the digit
  `nbr % base`, so the digits come out in order without storing them anywhere. The base is passed as a string (`"0123456789"`, `"0123456789abcdef"`...), which is why
  the same function works for `%u`, `%x`, `%X` and `%p`.
- `ft_putnbr_base` (the one for `%d` and `%i`) takes a `long`, not an `int`, to be able to negate `INT_MIN` without overflowing.
- The `va_list` is passed by pointer (`va_list *`). If it were passed by value, there would be no guarantee that the list had advanced when the function returned.
- There is no buffer and no `malloc`: each character is written right away with `write`. It is less efficient than the real `printf`, but there can be no leaks and the
  character count is always exact.

## Bonus

`make bonus` generates the same library, but accepting the full `%[flags][width][.precision]conversion` format:

| Element | Effect | Example | Output |
|---|---|---|---|
| width | Minimum number of characters; pads with spaces on the left | `"[%5d]", 42` | `[   42]` |
| `-` | Aligns to the left | `"[%-5d]", 42` | `[42   ]` |
| `0` | Pads with zeros instead of spaces | `"[%05d]", -42` | `[-0042]` |
| `.` | Precision: minimum digits for numbers, maximum characters for `%s` | `"[%.3d]", 7` / `"[%.2s]", "hello"` | `[007]` / `[he]` |
| `#` | Adds `0x` / `0X` in `%x` / `%X` if the number is not 0 | `"[%#x]", 255` | `[0xff]` |
| `+` | Always shows the sign in `%d` / `%i` | `"[%+d]", 42` | `[+42]` |
| space | Leaves a space in front of positive numbers | `"[% d]", 42` | `[ 42]` |

As in the real `printf`, `-` and precision override `0`, and `+` overrides space. `+` and space also work for `%p` (`+0x...`, ` 0x...`). A
`NULL` string with a precision below 6 prints nothing, instead of a truncated `(null)`.

Between the mandatory part and the bonus only one function changes, `ft_format`; `ft_printf`, `ft_parser` and `ft_printf_utils.c` are the same files in
both versions. In the mandatory part (`ft_printf_format.c`), `ft_format` only calls `ft_parser`. In the bonus (`ft_printf_format_bonus.c`) it does three things:

1. It reads the flags, width and precision and stores them in a `t_format` structure. It is a local variable, so there is still no `malloc`. Precision
   starts at `-1` to tell "no precision" apart from "precision 0" (`%.0d` with a 0 prints nothing).
2. It moves `ft_printf`'s pointer forward to the conversion letter; that is why it takes the format string as `char const **`.
3. It calls the function for that conversion (`ft_printf_conv_bonus.c`). `%%` and the odd cases are still delegated to `ft_parser`.

The trickiest part is that all numbers (`%d`, `%i`, `%u`, `%x`, `%X` and `%p`) end up in a single function, `ft_put_nbr` (`ft_printf_nbr_bonus.c`). The
conversion functions print nothing: they only choose the base and a *prefix* (`-`, `+`, space, `0x`...) and pass it the number already made positive. `ft_put_nbr`
works out how many digits and how many zeros are needed and always writes in the same order:

```
[spaces] [prefix] [zeros] [digits] [spaces]
```

Because the prefix goes before the zeros, `%05d` with -42 gives `-0042` and not `00-42`. The digits are still printed by `ft_putnbr_ubase`, the same function as in the
mandatory part.

## Resources

- Manual pages: `man 3 printf` and `man 3 stdarg`.
- [cppreference.com](https://en.cppreference.com/w/c/io/fprintf) for the exact behavior of each flag and of variadic functions.
- The 42 Norm (cursus document), for the style rules.

AI was used to fix Norm errors and to write this README. All the code was reviewed, tested and understood before submitting it.

## Structure

```
ft_printf.h               header with the prototypes of the mandatory part
ft_printf.c               ft_printf and ft_parser (common to both versions)
ft_printf_utils.c         ft_putchar, ft_putstr, ft_putnbr_base, ft_putnbr_ubase and pointer
ft_printf_format.c        ft_format of the mandatory part
ft_printf_bonus.h         t_format and the bonus prototypes
ft_printf_format_bonus.c  bonus ft_format: reads flags, width and precision
ft_printf_conv_bonus.c    one function per conversion: chooses base and prefix
ft_printf_nbr_bonus.c     ft_put_nbr and ft_putpad: padding, prefix, zeros and digits
libft/                    libft, with its own Makefile
Makefile                  all / bonus / clean / fclean / re
obj/                      .o files generated by make (not pushed)
libftprintf.a             final library (not pushed)
```
