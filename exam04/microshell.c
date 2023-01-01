#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define TYPE_END   3
#define TYPE_BREAK 4
#define TYPE_PIPE  5

typedef struct s_base
{
	int type;
	char **argv;
	int fd[2];
	int size;
	struct s_base *prev;
	struct s_base *next;
} t_base;

int ft_strlen(char *str)
{
	int i = 0;
	if (str)
	{
		while (str[i])
			i++;
	}
	return i;
}

char *ft_strdup(char *str)
{
	int lens = ft_strlen(str);
	char *new = (char *)malloc(sizeof(char) * (lens + 1));
	new[lens] = '\0';
	while (--lens >= 0)
		new[lens] = str[lens];
	return new;
}

int cd_err_1()
{
	char *str = "error: cd: bad arguments\n";
	write(2, str, ft_strlen(str));
	return (EXIT_FAILURE);
}

int cd_err_2(char *cmd)
{
	char *str = "error: cd: cannot change directory to ";
	write(2, str, ft_strlen(str));
	write(2, cmd, ft_strlen(cmd));
	write(2, "\n", 1);
	return (EXIT_FAILURE);
}

void exit_fatal()
{
	char *str = "error: fatal\n";
	write(2, str, ft_strlen(str));
	exit(EXIT_FAILURE);
}

void exit_execve(char *cmd)
{
	char *str = "error: cannot execute ";
	write(2, str, ft_strlen(str));
	write(2, cmd, ft_strlen(cmd));
	write(2, "\n", 1);
	exit(EXIT_FAILURE);
}


int get_size(char **av)
{
	int i = 0;
	while (av[i] && strcmp(av[i], "|") && strcmp(av[i], ";"))
		i++;
	return i;
}

int get_type(char *s)
{
	if (!s)
		return TYPE_END;
	if (!strcmp(s, "|"))
		return TYPE_PIPE;
	if (!strcmp(s, ";"))
		return TYPE_BREAK;
	return 0;
}

void ft_lstadd_back(t_base **ptr, t_base *new)
{
	t_base *temp;

	if (!(*ptr))
		*ptr = new;
	else
	{
		temp = *ptr;
		while (temp->next)
			temp = temp->next;
		temp->next = new;
		new->prev = temp;
	}
}

int parse(t_base **ptr, char **av)
{
	int size = get_size(av);
	t_base *new = (t_base *)malloc(sizeof(t_base));
	if (!new)
		exit_fatal();
	new->size  = size;
	new->argv = (char **)malloc(sizeof(char *) * (size + 1));
	if (!new->argv)
		exit_fatal();
	new->argv[size] = NULL;
	while (--size >= 0)
		new->argv[size] = ft_strdup(av[size]);
	new->type = get_type(av[new->size]);
	new->prev = NULL;
	new->next = NULL;
	ft_lstadd_back(ptr, new);
	return new->size;
}

void exec_cmd(t_base *temp, char **env)
{
	int pipe_open = 0;
	int status;
	pid_t pid;

	if ((temp->type == TYPE_PIPE)
		|| (temp->prev && temp->prev->type == TYPE_PIPE))
	{
		pipe_open = 1;
		if (pipe(temp->fd))
			exit_fatal();
	}
	pid = fork();
	if (pid < 0)
		exit_fatal();
	if (pid == 0)
	{
		if (temp->type == TYPE_PIPE && dup2(temp->fd[1], 1) < 0)
			exit_fatal();
		if (temp->prev && temp->prev->type == TYPE_PIPE &&
			dup2(temp->prev->fd[0], 0) < 0)
			exit_fatal();
		if (execve(temp->argv[0], temp->argv, env) < 0)
		{
			exit_execve(temp->argv[1]);
		}
		exit(EXIT_SUCCESS);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (pipe_open)
		{
			close(temp->fd[1]);
			if (!temp->next || temp->type == TYPE_BREAK)
				close(temp->fd[0]);
		}
		if (temp->prev && temp->prev->type)
			close(temp->prev->fd[0]);
	}
}

void exec_cmds(t_base *ptr, char **env)
{
	t_base *temp = ptr;
	while (temp)
	{
		if (!strcmp(temp->argv[0], "cd"))
		{
			if (temp->size != 2)
				cd_err_1();
			else if (chdir(temp->argv[1]))
				cd_err_2(temp->argv[1]);
		}
		else
			exec_cmd(temp, env);
		temp = temp->next;
	}
}

void free_all(t_base *ptr)
{
	t_base *temp;
	int size;

	while (ptr)
	{
		temp = ptr->next;
		size = ptr->size;
		while (--size >= 0)
			free(ptr->argv[size]);
		free(ptr->argv);
		free(ptr);
		ptr = temp;
	}
	ptr = NULL;
}

int main(int ac, char **av, char **env)
{
	t_base *ptr = NULL;
	int i = 1;
	if (ac > 1)
	{
		while (av[i])
		{
			if (!strcmp(av[i], ";"))
			{
				i++;
				continue;
			}
			i += parse(&ptr, &av[i]);
			if (!av[i])
				break;
			else
				i++;
		}
	if (ptr)
			exec_cmds(ptr, env);
		free_all(ptr);
	}
	return 0;
}
