#include <unistd.h>

int c_in_s2(char c, char *str)
{
	int i = 0;
	while (str[i])
	{
		if (c == str[i])
			return 1;
		i++;
	}
	return 0;
}

int fst_time_in_str(char c, char *str, int i)
{
	while (--i >= 0)
	{
		if (c == str[i])
			return (0);
	}
	return (1);
}

int main(int ac, char **av)
{
	char *s1;
	char *s2;
	int i = 0;
	if (ac == 3)
	{
		s1 = av[1];
		s2 = av[2];
		while (s1[i])
		{
			if (fst_time_in_str(s1[i], s1, i)
				&& c_in_s2(s1[i], s2))
				write(1, &s1[i], 1);
			i++;
		}
	}
	write(1, "\n", 1);
	return (0);
}
