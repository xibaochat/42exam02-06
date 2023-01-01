#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>

void ft_putchar(char c, int *ret)
{
	write(1, &c, 1);
	*ret += 1;
}

void ft_putstr(char *str, int *ret)
{
	int i = 0;
	while (str[i])
		ft_putchar(str[i++], ret);
}

int get_lens(long int nb, int base)
{
	int i = 1;
	if (nb < 0)
	{
		i++;
		nb = -1 * nb;
	}
	while (nb >= base)
	{
		i++;
		nb = nb / base;
	}
	return (i);
}

char *itoa_base(long int nb, int base, char *tab)
{
	int lens = get_lens(nb, base);
	char *str;
	if (nb == 0)
	{
		str = (char *)malloc(sizeof(char) * 2);
		str[0] = '0';
		str[1] = '\0';
		return str;
	}
	str = (char *)malloc(sizeof(char) * (lens + 1));
	str[lens] = '\0';
	if (nb < 0)
	{
		str[0] = '-';
		nb = -1 * nb;
	}
	while (nb != 0)
	{
		str[--lens] = tab[nb % base];
		nb = nb / base;
	}
	return str;
}

void print_int(va_list params, int *ret)
{
	int nb = va_arg(params, int);
	char *str = itoa_base(nb, 10, "0123456789");
	ft_putstr(str, ret);
}

void print_hex(va_list params, int *ret)
{
	unsigned int nb = va_arg(params, unsigned int);
	char *str = itoa_base(nb, 16, "0123456789abcdef");
	ft_putstr(str, ret);
}

void print_str(va_list params, int *ret)
{
	char *str = va_arg(params, char *);
	if (!str)
		str = "(null)";
	ft_putstr(str, ret);
}

void parse_str(va_list params, const char *str, int *ret)
{
	int i = 0;
	while (str[i])
	{
		while (str[i] && str[i] != '%')
			ft_putchar(str[i++], ret);
		if (str[i] == '%')
		{
			i++;
			if (str[i] == 's')
				print_str(params, ret);
			else if (str[i] == 'd')
				print_int(params, ret);
			else if (str[i] == 'x')
				print_hex(params, ret);
			else
				break;
			i++;
		}
	}
}

int ft_printf(const char *str, ... )
{
	va_list params;
	int ret = 0;
	va_start(params, str);
	parse_str(params, str, &ret);
	va_end(params);
	return (ret);
}

/* int main() */
/* { */
/* 	ft_printf("%s\n", "toto"); */

/* } */
