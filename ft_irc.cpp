#include "./ft_irc.hpp"

int	ft_check(char *str, std::string str2)
{
	for (int i = 0; str[i]; i++)
		if (str[i] < '0' || str[i] > '9')
			std::cerr << "Error: port must be a number" << std::endl, exit(1);
	if (!str2.size())
		std::cerr << "Error: password must not be empty" << std::endl, exit(1);
	return (atoi(str));
}

int main(int ac, char **av)
{
	if (ac == 3)
	{
		int					client_fd, activity;
		struct sockaddr_in	addr_serv;
		socklen_t			addr_len = sizeof(addr_serv);
		char				buffer[1024];
		int					buffer_len;
		irc_server			server(ft_check(av[1], av[2]), av[2]);

		if (server.set_socket_fd(socket(AF_INET, SOCK_STREAM, 0)) == -1)
			std::cerr << "Error: socket() failed" << std::endl, exit(1);
		std::cout << "Socket created" << std::endl;

		bzero(&addr_serv, sizeof(addr_serv));
		addr_serv.sin_family = AF_INET; // AF_INET
		addr_serv.sin_port = htons(server.get_port()); // htons
		addr_serv.sin_addr.s_addr = htonl(INADDR_ANY); // htonl

		if (bind(server.get_socket_fd(), (struct sockaddr *)&addr_serv, sizeof(addr_serv)) == -1)
			std::cerr << "Error: bind() failed" << std::endl, exit(1);
		if (listen(server.get_socket_fd(), MAX_QUEUE) == -1)
			std::cerr << "Error: listen() failed" << std::endl, exit(1);

		fd_set				readset;
		std::set<int>		clients;
		std::cout << "waiting on port " << server.get_port() << std::endl;

		while (true)
		{
			FD_ZERO(&readset); // FD_ZERO
			FD_SET(server.get_socket_fd(), &readset); // FD_SET

			std::cout << "Waiting for connection on port " << server.get_port() << std::endl;

			for (std::set<int>::iterator itter = clients.begin(); itter != clients.end(); ++itter)
			{
				if (*itter >= 0)
					FD_SET(*itter, &readset); // FD_SET
				std::cout << *itter << std::endl;
			}

			if (clients.size() > 0)
				if ((activity = select(*(clients.rend()) + 1, &readset, NULL, NULL, NULL)) == -1) // select
					std::cerr << "Error: select() failed" << std::endl, exit(1);

			if (FD_ISSET(server.get_socket_fd(), &readset)) // FD_ISSET
			{
				if ((client_fd = accept(server.get_socket_fd(), (struct sockaddr *)&addr_serv, &addr_len)) == -1)
				{
					std::cerr << "Error: accept() failed with exit code: " << errno << std::endl;
					exit(0);
				}
			}

			std::cout << "Connection accepted" << std::endl;

			while ((buffer_len = read(client_fd, buffer, 1024)) > 0)
			{
				std::cout << buffer, memset(buffer, 0, 1024);
				if (write(client_fd, "Message received!\n", 18) == -1)
					std::cerr << "Error: write() failed on line 74" << std::endl, exit(1);
				if (buffer[buffer_len - 1] == '\n')
					break;
			}
			if (buffer_len == 0)
			{
				if (write(client_fd, "Message received!\n", 18) == -1)
					std::cerr << "Error: write() failed on line 83" << std::endl, exit(1);
			std::cout << "Client disconnected" << std::endl, close(client_fd);
			}
			if (buffer_len == -1)
				std::cerr << "Error: read() failed" << std::endl, exit(1);
		}
	}
	else
		std::cerr << "Error, The program runs as follows:\n ./ircserv <port> <password>" << std::endl;
	return (0);
}
