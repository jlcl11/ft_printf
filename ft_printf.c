/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocorral <jocorral@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 12:26:09 by jocorral          #+#    #+#             */
/*   Updated: 2026/10/08 11:22:13 by jocorral         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#ifdef TEST
# include <stdio.h>
# include <limits.h>
#endif

static int	parser(char c, va_list *args)
{
	if (c == 'c')
		return (ft_putchar(va_arg(*args, int)));
	else if (c == 's')
		return (ft_putstr(va_arg(*args, char *)));
	else if (c == 'p')
		return (puntero(va_arg(*args, void *)));
	else if (c == 'd' || c == 'i')
		return (ft_putnbr_base(va_arg(*args, int), "0123456789"));
	else if (c == 'u')
		return (ft_putnbr_ubase(va_arg(*args, unsigned int), "0123456789"));
	else if (c == 'x')
		return (ft_putnbr_ubase(va_arg(*args, unsigned int),
				"0123456789abcdef"));
	else if (c == 'X')
		return (ft_putnbr_ubase(va_arg(*args, unsigned int),
				"0123456789ABCDEF"));
	else if (c == '%')
		return (ft_putchar('%'));
	return (ft_putchar('%') + ft_putchar(c));
}

int	ft_printf(char const *format, ...)
{
	va_list	args;
	int		count;
	int		i;

	if (!format)
		return (-1);
	va_start(args, format);
	count = 0;
	i = 0;
	while (format[i])
	{
		if (format[i] == '%' && format[i + 1])
		{
			i++;
			count += parser(format[i], &args);
		}
		else if (format[i] != '%')
			count += ft_putchar(format[i]);
		i++;
	}
	va_end(args);
	return (count);
}

#ifdef TEST

static void	comparar(int a, int b)
{
	if (a == b)
		printf("\033[32mOK\033[0m (%d)\n\n", a);
	else
		printf("\033[31mKO\033[0m printf=%d ft_printf=%d\n\n", a, b);
	fflush(stdout);
}

static void	probar_c_s_p_d_i(void)
{
	int	a;
	int	x;

	a = printf("c: [%c][%c]\n", 'A', 0);
	fflush(stdout);
	comparar(a, ft_printf("c: [%c][%c]\n", 'A', 0));
	a = printf("s: [%s][%s][%s]\n", "hola", "", (char *) NULL);
	fflush(stdout);
	comparar(a, ft_printf("s: [%s][%s][%s]\n", "hola", "", (char *) NULL));
	a = printf("p: [%p][%p]\n", &x, NULL);
	fflush(stdout);
	comparar(a, ft_printf("p: [%p][%p]\n", &x, NULL));
	a = printf("d: [%d][%d][%d]\n", 0, INT_MAX, INT_MIN);
	fflush(stdout);
	comparar(a, ft_printf("d: [%d][%d][%d]\n", 0, INT_MAX, INT_MIN));
	a = printf("i: [%i][%i]\n", 42, -42);
	fflush(stdout);
	comparar(a, ft_printf("i: [%i][%i]\n", 42, -42));
}

int	main(void)
{
	int	a;

	probar_c_s_p_d_i();
	a = printf("u: [%u][%u]\n", 0, UINT_MAX);
	fflush(stdout);
	comparar(a, ft_printf("u: [%u][%u]\n", 0, UINT_MAX));
	a = printf("x: [%x][%x]\n", 255, -1);
	fflush(stdout);
	comparar(a, ft_printf("x: [%x][%x]\n", 255, -1));
	a = printf("X: [%X][%X]\n", 255, -1);
	fflush(stdout);
	comparar(a, ft_printf("X: [%X][%X]\n", 255, -1));
	a = printf("%%: [%%][%%%%]\n");
	fflush(stdout);
	comparar(a, ft_printf("%%: [%%][%%%%]\n"));
	a = printf("otro: [%k]\n");
	fflush(stdout);
	comparar(a, ft_printf("otro: [%k]\n"));
	return (0);
}

#endif
