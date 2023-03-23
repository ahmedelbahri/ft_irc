#include "./ft_irc.hpp"

socklen_t	addr_len = sizeof(struct sockaddr_in);

void	error(std::string str, int code = 0)
{
	std::cerr << "Error: " << str << std::endl;
	if (code != 0)
		std::cerr << " with error number " << code << std::endl;
	exit(1);
}

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

void	irc_server::init_server(void)
{
	if ((this->sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) // socket
		error("socket() failed");

	if (fcntl(this->sock_fd, F_SETFL, O_NONBLOCK) == -1) // fcntl
		error("fcntl() failed");

	if (setsockopt(this->sock_fd, SOL_SOCKET, SO_REUSEADDR, &(this->dummy), sizeof(this->dummy)) == -1) // setsockopt
		error("setsockopt() failed");

	bzero(&(this->addr), sizeof((this->addr)));
	(this->addr).sin_family = AF_INET; // AF_INET
	(this->addr).sin_port = htons(this->port); // htons
	(this->addr).sin_addr.s_addr = htonl(INADDR_ANY); // htonl

	if (bind(this->sock_fd, (struct sockaddr *)&(this->addr), addr_len) == -1)
		error("bind() failed");

	if (listen(this->sock_fd, MAX_QUEUE) == -1)
		error("listen() failed");
}

// int	accept_connection(irc_server &server, fd_set &readset, std::set<int> &clients)
// {
// 	int	client_fd;
// 	// if (FD_ISSET(server.fd(), &readset)) // FD_ISSET
// 	// {
// 		if ((client_fd = accept(server.fd(), (struct sockaddr *)&server.addr(), &addr_len)) == -1) // accept
// 		{
// 			if (errno != EWOULDBLOCK && errno != EAGAIN)
// 				error("accept() failed", errno);
// 		}
// 		else
// 		{
// 			clients.insert(client_fd);
// 			FD_SET(client_fd, &readset);
// 			std::cout << "Connection accepted" << std::endl;
// 		}
// 	// }
// 	return (client_fd);
// }

void	irc_server::check_pollable_discriptors(int &pollfd_size)
{
	int		size = pollfd_size;
	int		client_fd = 0;
	int		len;
	char	buffer[1024];
	bool	check_poll_fd = false;
	for (int i = 0; i < size; i++)
	{
		memset(buffer, 0, 1024);
		if (poll_fd[i].revents == 0)
			continue;
		if (poll_fd[i].revents & POLLHUP) // check & POLLHUP
		{
			std::cout << "client " << poll_fd[i].fd << " disconnected." << std::endl;
			close(poll_fd[i].fd);
			poll_fd[i].fd = -1;
			check_poll_fd = true;
			continue;
		}
		if (poll_fd[i].revents != POLLIN)
			error("poll() returned unexpected revents value", poll_fd[i].revents);
		if (poll_fd[i].fd == this->sock_fd)
		{
			while (client_fd != -1)
			{
				if ((client_fd = accept(this->sock_fd, (struct sockaddr *)&(this->addr), &addr_len)) < 0
					&& (errno != EWOULDBLOCK))
					error("accept() failed whith error number: ", errno);
				if (client_fd > 0 && pollfd_size < MAX_QUEUE)
				{
					std::cout << "Connection accepted." << std::endl;
					poll_fd[pollfd_size].fd = client_fd;
					poll_fd[pollfd_size].events = POLLIN;
					pollfd_size++;
				}
				else if (client_fd > 0)
					std::cout << "Connection refused." << std::endl, close(client_fd);
			}
		}
		else
		{
			std::cout << "client " << poll_fd[i].fd <<  " " << i << " sent a message." << std::endl;
			while (true)
			{
				if ((len = recv(poll_fd[i].fd, buffer, 1024, 0)) < 0)
				{
					if (errno != EWOULDBLOCK)
						std::cerr << "Error: recv() failed whith error number: " << errno << std::endl;
					break;
				}
				// if (len == 0)
				// {
				// 	std::cout << "client " << poll_fd[i].fd << " disconnected." << std::endl;
				// 	close_connection = true;
				// 	break;
				// }
				std::cout << "message received: " << buffer << std::endl;
				// if ((len = send(poll_fd[i].fd, "message received.", len, 0)) < 0)
				// {
				// 	std::cerr << "send() failed whith error number: " << errno << std::endl;
				// 	close_connection = true;
				// 	break;
				// }
			}
		}
	}
	if (check_poll_fd)
	{
		check_poll_fd = false;
		for (int i = 0; i < pollfd_size; i++)
		{
			if (poll_fd[i].fd == -1)
			{
				for (int j = i; j + 1 < pollfd_size; j++)
					poll_fd[j].fd = poll_fd[j + 1].fd;
				pollfd_size--;
				i--;
			}
		}
	}
	std::cout << "pollfd_size: " << pollfd_size << std::endl;
}

int main (int ac, char **av)
{
	irc_server			server(ft_check(ac, av), av[2]);
	int					timeout = (3 * 60 * 1000); // in the following order: minutes, seconds, milliseconds
	int					pollfd_size = 1;
	int					activity;
	// char				buffer[1024];
	// int					buffer_len;

	server.init_server();

	memset(&poll_fd, 0, sizeof(poll_fd));
	poll_fd[0].fd = server.fd();
	poll_fd[0].events = POLLIN;

	while (true)
	{
		// std::cout << "waiting on port " << server.get_port() << std::endl;
		if ((activity = poll(poll_fd, pollfd_size, timeout)) == -1) // poll
			error("poll() failed.");
		if (activity == 0)
			error("poll() timed out. end of execution.");

		server.check_pollable_discriptors(pollfd_size);

		// server.
	}
	return (0);
}
