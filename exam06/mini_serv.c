#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>

int BUFFER_SIZE = 1024;

int		idtab[FD_SETSIZE]; // returns the client id given a FD -- client id of FD == idtab[FD]
int		id = 0; // id counter, we increment when we add a new client

char	*idmsg[FD_SETSIZE] = { 0 } ; // keeps the complete messages every client has to send

int		listener;
fd_set	master;

int extract_message(char **buf, char **msg)
{
	char	*newbuf;
	int	i;

	*msg = 0;
	if (*buf == 0)
		return (0);
	i = 0;
	while ((*buf)[i])
	{
		if ((*buf)[i] == '\n')
		{
			newbuf = calloc(1, sizeof(*newbuf) * (strlen(*buf + i + 1) + 1));
			if (newbuf == 0)
				return (-1);
			strcpy(newbuf, *buf + i + 1);
			*msg = *buf;
			(*msg)[i + 1] = 0;
			*buf = newbuf;
			return (1);
		}
		i++;
	}
	return (0);
}

char *str_join(char *buf, char *add)
{
	char	*newbuf;
	int		len;

	if (buf == 0)
		len = 0;
	else
		len = strlen(buf);
	newbuf = malloc(sizeof(*newbuf) * (len + strlen(add) + 1));
	if (newbuf == 0)
		return (0);
	newbuf[0] = 0;
	if (buf != 0)
	{
		strcat(newbuf, buf);
		free(buf); // there was an error here in the given main, must be free only if not null.
	}
	strcat(newbuf, add);
	return (newbuf);
}

int				fatal() {

	const char * err = "Fatal error\n";
	write(2, err, strlen(err));
	exit(EXIT_FAILURE);
}

int				get_listener_socket(int port) {

	int fd;
	int yes = 1;

	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    	return fatal();

	struct sockaddr_in	address;

	bzero(&address, sizeof(address));

	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = htonl(2130706433);

	if (bind(fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    	return fatal();

	if (listen(fd, SOMAXCONN) < 0)
    	return fatal();

	return fd;
}

void		broadcast(char *msg, int sender) {

	for (int i = 0; i < FD_SETSIZE; i++) {
		if (FD_ISSET(i, &master) && i != listener && i != sender) {
			send(i, msg, strlen(msg), 0);
		}
	}
}

int main(int ac, char **av) {

	if (ac != 2) {

		const char * err = "Wrong number of arguments\n";
		write(2, err, strlen(err));
		return EXIT_FAILURE;
	}

	char	buf[BUFFER_SIZE + 1];
	int		newfd;
	int		ret;
	fd_set	read_fds;

	listener = get_listener_socket(atoi(av[1]));
	FD_ZERO(&master);
	FD_SET(listener, &master);

	while (1) {

		read_fds = master;
		select(FD_SETSIZE, &read_fds, NULL, NULL, NULL);

		for (int i = 0; i < FD_SETSIZE; i++) {

			if (FD_ISSET(i, &read_fds)) {

				if (i == listener) {
					if ((newfd = accept(i, 0, 0)) < 0)
						exit(1);
					FD_SET(newfd, &master);
					idtab[newfd] = id++;

					sprintf(buf, "server: client %d just arrived\n", idtab[newfd]);
					broadcast(buf, newfd);

				} else {

					if ((ret = recv(i, buf, BUFFER_SIZE, 0)) <= 0) {

						sprintf(buf, "server: client %d just left\n", idtab[i]);
						broadcast(buf, i);
						FD_CLR(i, &master);
						close(i);
						continue ;
					}

					buf[ret] = 0;

					char * msg = NULL;
					idmsg[i] = str_join(idmsg[i], buf);
						while (extract_message(&idmsg[i], &msg)) {

						char * to_send = malloc(100 + strlen(msg));
						sprintf(to_send, "client %d: %s", idtab[i], msg);
						broadcast(to_send, i);
						free(to_send);
						free(msg);
					}
				}
			}
		}
	}
}
