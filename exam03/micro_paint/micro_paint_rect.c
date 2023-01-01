/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   micro_paint_rect.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelaman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/08 11:21:51 by mbelaman          #+#    #+#             */
/*   Updated: 2020/12/08 14:25:07 by mbelaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

typedef	struct	s_zone
{
	int		width;
	int 	height;
	char	ch;
}				t_zone;

typedef	struct	s_rect
{
	char	type;
	float	w;
	float	h;
	float	x;
	float	y;
	char	c;
}				t_rect;


void	ft_putchar(char c)
{
	write(1, &c, 1);
}

int	ft_putstr(char *str)
{
	while (*str)
	{
		ft_putchar(*str);
		str++;
	}
	return (1);
}

int	draw_shape(FILE *f, t_zone *zone, char *data)
{
	int		i;
	int		j;
	int		ret;
	int		ret2;
	t_rect	rect;

	while ((ret = fscanf(f, "%c %f %f %f %f %c", &rect.type, &rect.w, &rect.h, &rect.x, &rect.y, &rect.c)) == 6)
	{
		if (rect.w < 0 && rect.h < 0 && (rect.type != 'r' || rect.type != 'R'))
			return (0);
		i = 0;
		while (i < zone->height)
		{
			j = 0;
			while (one->width)
			{

			}
		}
	}
}

char	*get_data(FILE *f, t_zone *zone, char *data)
{
	int fs;

	if ((fs = fscanf(f, "%d %d %c", &zone->width, &zone->height, &zone->ch)) != 3)
		return (NULL);
	if (fs == -1)
		return (NULL);
	if (zone->width < 0 && zone->width > 300 && zone->height < 0 && zone->height > 300)
		return (NULL);
	if (!(data = (char *)malloc(zone->width * zone->height)))
		return (NULL);
	int i = 0;
	while (i < zone->width * zone->height)
		data[i++] = zone->ch;
	return (data);
}

int main(int ac, char **av)
{
	FILE 	*fp;
	char	*data;
	t_zone zone;

	if (ac != 2)
		return (ft_putstr("Error: argument\n"));
	if (!(fp = fopen(av[1], "r")))
		return (ft_putstr("Error: Operation file\n"));
	if (!(data = get_data(fd, &zone, &data)))
		return (ft_putstr("Error: Operation file corrupted\n"));
	if (!(draw_shape(fd, &zone, &data)))
		return (ft_putstr("Error: Operation file corrupted\n"));
	int i = 0;
	while (i < zone.height)
	{
		write(1, data + i * zone.width, zone.width);
		write(1, "\n", 1);
		i++;
	}
	return (0);
}
