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
0
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
	int		client_fd;
	bool	close_connection = false;
	int		len;
	char	buffer[1024];
	bool	check_poll_fd = false;
	for (int i = 0; i < size; i++)
	{
		if (poll_fd[i].revents == 0)
			continue;
		if (poll_fd[i].revents != POLLIN)
			error("poll() returned unexpected revents value.");
		if (poll_fd[i].fd == this->sock_fd)
		{
			do
			{
				if ((client_fd = accept(this->sock_fd, (struct sockaddr *)&(this->addr), &addr_len)) < 0
					&& (errno != EWOULDBLOCK))
					error("accept() failed whith error number: ", errno);
				std::cout << "Connection accepted." << std::endl;
				poll_fd[pollfd_size].fd = client_fd;
				poll_fd[pollfd_size].events = POLLIN;
				pollfd_size++;
			} while (client_fd < 0);
		}
		else
		{
			std::cout << "client " << poll_fd[i].fd << " sent a message." << std::endl;
			close_connection = false;
			while (true)
			{
				if ((len = recv(poll_fd[i].fd, buffer, 1024, 0)) < 0)
				{
					if (errno != EWOULDBLOCK)
						std::cerr << "recv() failed whith error number: " << errno << std::endl, close_connection = true;
					break;
				}
				if (len == 0)
				{
					std::cout << "client " << poll_fd[i].fd << " disconnected." << std::endl;
					close_connection = true;
					break;
				}
				std::cout << "message received: " << buffer << std::endl;
				// if ((len = send(poll_fd[i].fd, "message received.", len, 0)) < 0)
				// {
				// 	std::cerr << "send() failed whith error number: " << errno << std::endl;
				// 	close_connection = true;
				// 	break;
				// }
			}
			if (close_connection)
			{
				close(poll_fd[i].fd);
				poll_fd[i].fd = -1;
				// compress_array(poll_fd, pollfd_size, i);
				// i--;
				check_poll_fd = true;
			}
		}
	}
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

	// std::set<int>	clients;
	// clients.insert(server.fd());
	// std::cout << "waiting on port " << server.port() << std::endl;
	// std::set<int>::iterator	itter;

	while (true)
	{
		std::cout << "waiting on port " << server.get_port() << std::endl;
		if ((activity = poll(poll_fd, pollfd_size, timeout)) == -1) // poll
			error("poll() failed.");
		if (activity == 0)
			error("poll() timed out. end of execution.");

		server.check_pollable_discriptors(pollfd_size);

	// 	// if (clients.size() > 0)
	// 	// {
	// 		// std::cout << clients.size() << "|" << *(clients.rend()) << std::endl;
	// 		// std::cout << "this is select" << std::endl;
	// 		if ((activity = select(*(clients.rend()) + 1, &readset, NULL, NULL, NULL)) == -1) // select
	// 			error("select() failed");
	// 		exit(0);
	// 	// }

	// 	// accept_connection(server, client_fd, readset, clients);

	// 	// if (client_fd != -1)
	// 	// {
	// 	// 	FD_SET(client_fd, &readset);
	// 	// 	clients.insert(client_fd);
	// 		// std::cout << "marker" << std::endl;
	// 	// 	clients.insert(client_fd);
	// 	// 	std::cout << "Connection accepted" << std::endl;
	// 	// }
	// 	// if (clients.size() > 0)
	// 	// {
	// 	// 	itter = clients.begin(), itter++;
	// 	// 	while (itter != clients.end())
	// 	// 	{
	// 	// 		if (FD_ISSET(*itter, &readset)) // FD_ISSET
	// 	// 		{
	// 				// std::cout << "this is read" << std::endl;
	// 				for (std::set<int>::iterator i = clients.begin(); i != clients.end(); ++i)
	// 				{
	// 					if (FD_ISSET(*i, &readset))
	// 					{
	// 						if (*i == server.fd())
	// 							client_fd = accept_connection(server, readset, clients);
	// 						else
	// 						{
	// 							if ((buffer_len = read(*itter, buffer, 1024)) > 0)
	// 							{
	// 								std::cout << buffer;
	// 								// std::cout << "in\n";
	// 								// if (buffer[buffer_len - 1] == '\n' && memset(buffer, 0, 1024))
	// 								// 	break;
	// 								// memset(buffer, 0, 1024);
	// 							}
	// 							// std::cout << buffer_len << std::endl;
	// 							if (buffer_len == 0)
	// 							{
	// 								if (write(*itter, "Message received!\n", 18) == -1)
	// 									error("write() failed on section 2");
	// 								std::cout << "Client disconnected" << std::endl, close(*itter);
	// 								// clients.erase(itter++);
	// 							}
	// 							// if (buffer_len == -1)
	// 							// {
	// 							// 	if (errno != EWOULDBLOCK && errno != EAGAIN)
	// 							// 		error("read() failed with exit code: ", errno);
	// 							// 	// exit(0);
	// 							// }
	// 						}
	// 					}
	// 				}
	// 			// }
	// 			// ++itter;
	// 		// }
	// 	// }
	}
	return (0);
}
