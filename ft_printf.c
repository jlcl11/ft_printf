/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocorral <jocorral@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 12:26:09 by jocorral          #+#    #+#             */
/*   Updated: 2026/10/07 16:53:07 by jocorral         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>

int	ft_printf(char const *, ...)
{
	//aquí cogemos los argumentos y vamos recorriendoel string
}

void	parser(char c, va_list args)
{
	if (c == 'c')
		ft_putchar(va_arg(args, int));
	if (c == 's')
		ft_putstr(va_arg(args, char *));
	if (c == 'p')
		puntero(va_arg(args, void *));
	if (c == 'd')
		ft_putnbr_base(va_arg(args, int), "0123456789");
	if (c == 'i')
		ft_putnbr_base(va_arg(args, int), "0123456789");
	if (c == 'u')
		ft_putnbr_base(va_arg(args, unsigned int), "0123456789");
	if (c == 'x')
		ft_putnbr_base(va_arg(args, unsigned int), "0123456789abcdef");
	if (c == 'X')
		ft_putnbr_base(va_arg(args, unsigned int), "0123456789ABCDEF");
	if (c == '%')
		ft_putchar('%');
}

int	ft_base_valida(char *base)
{
	int	i;
	int	j;

	i = 0;
	while (base[i])
	{
		if (base[i] == '+' || base[i] == '-')
			return (0);
		j = i + 1;
		while (base[j])
		{
			if (base[i] == base[j])
				return (0);
			j++;
		}
		i++;
	}
	if (i < 2)
		return (0);
	return (1);
}

void	puntero(void *ptr)
{
}

int	ft_atoi(const char *nptr)
{
	int		i;
	int		signo;
	long	ret;

	i = 0;
	signo = 1;
	ret = 0;
	while (nptr[i] == ' ' || (nptr[i] >= '\t' && nptr[i] <= '\r'))
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			signo = -1;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		ret = ret * 10 + (nptr[i] - '0');
		i++;
	}
	return ((int)(ret * signo));
}

void	ft_putnbr_base(long nbr, char *base)
{
	int	len;

	if (!ft_base_valida(base))
		return ;
	len = 0;
	while (base[len])
		len++;
	if (nbr == -2147483648)
	{
		ft_putchar('-');
		ft_putnbr_base(214748364, base);
		ft_putchar(base[8 % len]);
		return ;
	}
	if (nbr < 0)
	{
		ft_putchar('-');
		nbr = -nbr;
	}
	if (nbr >= len)
		ft_putnbr_base(nbr / len, base);
	ft_putchar(base[nbr % len]);
}

void	ft_putstr(char *str)
{
	while (*str != '\0')
	{
		write(1, str, 1);
		str++;
	}
}

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

int	main(void)
{
	//llamar a la libreía de libft con el .a y el .h
	//asignar los va_start y to el rollo
}
