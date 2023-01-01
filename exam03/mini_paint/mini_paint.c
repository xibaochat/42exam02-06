#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

int width;
int height;
char bg;

char type;
float x;
float y;
float rad;
char c;

char *str;

void output()
{
	int i = -1;
	while (++i < height)
	{
		write(1, str + i * width, width);
		write(1, "\n", 1);
	}
}

int check(float xw, float yh)
{
	float distance = sqrtf(powf(xw - x, 2) + powf(yh - y, 2));
	if (distance <= rad)
	{
		if (distance <= rad - 1 )
			return (1);//inside
		return (2);
	}
	return (0);

}

void drawing()
{
	int xw = -1;
	int yh;
	while (++xw < width)
	{
		yh = -1;
		while (++yh < height)
		{
			int hit = check((float)xw, (float)yh);
			if (hit == 2 || (hit == 1 && type == 'C'))
				str[xw + width * yh] = c;
		}
	}
}

int parse(FILE *file)
{

	int res = fscanf(file, "%d %d %c\n", &width, &height, &bg);
	if (res != 3)
		return 1;
	if (width > 300 || width < 1 || height > 300 || height < 1)
		return 1;
	str = (char *)malloc(sizeof(char) * (width * height));
	if (!str)
		return (1);
	int i = 0;
	while (i < width * height)
		str[i++] = bg;
	while ((res = fscanf(file, "%c %f %f %f %c\n", &type, &x, &y, &rad, &c)) == 5)
	{
		if ((type != 'c' && type != 'C') || rad <= 0)
			return 1;
		drawing();
	}
	if (res != -1)
	{
		return (1);
	}
	output();
	return (0);
}

int main(int ac, char **av)
{
	if (ac != 2)
	{
		write(1, "Error: argument\n", 16);
		return (1);
	}
	FILE *file = fopen(av[1], "r");
	if (!file || parse(file))
	{
		write(1, "Error: Operation file corrupted\n", 32);
		return (1);
	}
	fclose(file);
	return (0);
}
