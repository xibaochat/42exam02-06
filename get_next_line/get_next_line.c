#include "get_next_line.h"

int no_newline_in_str(char *str)
{
	int i = 0;
	if (!str)
		return (1);
	while (str[i])
	{
		if (str[i] == '\n')
			return (0);
		i++;
	}
	return (1);
}

int ft_strlen(char *str)
{
	int i = 0;
	if (str)
	{
		while (str[i])
			i++;
	}
	return (i);
}

char *ft_strnew(int n)
{
	char *str = (char *)malloc(sizeof(char) * n);
	while(--n >= 0)
		str[n] = '\0';
	return (str);
}

char *ft_strncat(char *dest, char *src, int nb)
{
	int lens = ft_strlen(dest);
	int i = 0;
	while (src[i] && i < nb)
		dest[lens++] = src[i++];
	dest[lens] = '\0';
	return (dest);
}

char *extract_line(char **str)
{
	int i = 0;
	int j = 0;
	int new_line = 0;
	char *line;
	char *s;

	while((*str)[i] && (*str)[i] != '\n')
		i++;
	if ((*str)[i] == '\n')
		new_line = 1;
	line = ft_strnew(i + new_line + 1);
	ft_strncat(line, *str, i + new_line);
	j = ft_strlen(*str) - i;
	if (!j)
	{
		free(*str);
		*str = NULL;
		return (line);
	}
	s = ft_strnew(j);
	ft_strncat(s, *str + i + 1, j - 1);
	free(*str);
	*str = s;
	return line;
}

int get_content_from_file(int fd, char **str)
{
	int nb_read;
	char *buff;
	char *s;

	buff = ft_strnew(BUFFER_SIZE + 1);
	nb_read = read(fd, buff, BUFFER_SIZE);
	if (nb_read <= 0)
	{
		free(buff);
		return (nb_read);
	}
	s = ft_strnew(nb_read + ft_strlen(*str) + 1);
	if (*str)
	{
		ft_strncat(s, *str, ft_strlen(*str));
		free(*str);
		*str = NULL;
	}
	*str = ft_strncat(s, buff, nb_read);
	free(buff);
	if (no_newline_in_str(*str))
		return get_content_from_file(fd, str);
	return 1;
}

char    *get_next_line(int fd)
{
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	static char *str = NULL;
	if (no_newline_in_str(str))
	{
		int res = get_content_from_file(fd, &str);
		if (res < 0)
			return NULL;
		if (!str)
			return NULL;
		if (!str[0])
		{
			free(str);
			return (NULL);
		}
	}
	return (extract_line(&str));
}
/* #include <fcntl.h> */
/* int main() */
/* { */
/* 	int fd = open("get_next_line.c", O_RDONLY); */
/* 	char *line = get_next_line(fd); */
/* 	while (line) */
/* 	{ */
/* 		write(1, line, ft_strlen(line)); */
/* 		line = get_next_line(fd); */
/* 	} */
/* 	return (0); */
/* } */
