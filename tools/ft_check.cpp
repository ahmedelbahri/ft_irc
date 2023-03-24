#include "../includes/ft_irc.hpp"

int	ft_check(int ac, char **av)
{
	if (ac != 3)
		error("The program runs as follows:\n./ircserv <port> <password>");
	for (int i = 0; av[1][i]; i++)
		if (av[1][i] < '0' || av[1][i] > '9')
			error("port must be a number");
	if (atoi(av[1]) <= 1024)
		error("port must be higher than 1024");
	if (!strlen(av[2]))
		error("password must not be empty");
	return (atoi(av[1]));
}