#include <unistd.h>

int ft_strlen(char *str)
{
	int i = 0;
	while (str[i])
		i++;
	return i;
}

int char_is_fst_time(char c, char *str, int i)
{
	while (--i >= 0)
	{
		if (str[i] == c)
			return (0);
	}
	return (1);
}

int main(int ac, char **av)
{
	char *s1;
	char *s2;
	int i = 0 ;
	if (ac == 3)
	{
		s1 = av[1];
		s2 = av[2];
		while (s1[i])
		{
			if (char_is_fst_time(s1[i], s1, i))
				write(1, &s1[i], 1);
			i++;
		}
		i = 0;
		while (s2[i])
		{
			if (char_is_fst_time(s2[i], s2, i) &&
				char_is_fst_time(s2[i], s1, ft_strlen(s1)))
				write(1, &s2[i], 1);
			i++;
		}
	}
	write(1, "\n", 1);
	return (0);
}
