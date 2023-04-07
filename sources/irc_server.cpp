#include "../includes/ft_irc.hpp"

irc_server::irc_server(void)
{
	this->dummy = 1;
	return;
}
void	irc_server::set_server(int port, std::string password)
{
	this->port = port;
	this->password = password;
	return;
}

int	irc_server::set_fd(int socket_fd)
{
	this->sock_fd = socket_fd;
	return (this->sock_fd);
}

int	irc_server::fd(void)
{
	return (this->sock_fd);
}

int	irc_server::get_port(void)
{
	return (this->port);
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

void	irc_server::remove_closed_discriptors(int &pollfd_size)
{
	if (this->check_poll_fd)
	{
		this->check_poll_fd = false;
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
}

void	irc_server::fd_is_socket(int &pollfd_size)
{
	int			client_fd = 0;
	irc_client	client;

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
			client.set_fd(client_fd);
			clients[client_fd] = client;
		}
		else if (client_fd > 0)
			std::cout << "Connection refused." << std::endl, close(client_fd);
	}
}

void	irc_server::fd_is_client(int i)
{
	int			len;
	char		buffer[1024];
	// std::cout << "client " << poll_fd[i].fd << " " << i << " sent a message." << std::endl;
	while (true)
	{
		memset(buffer, 0, 1024);
		if ((len = recv(poll_fd[i].fd, buffer, 1024, 0)) < 0)
		{
			if (errno != EWOULDBLOCK)
				std::cerr << "Error: recv() failed whith error number: " << errno << std::endl;
			break;
		}
		(clients[poll_fd[i].fd].buff()).append(buffer);
	}
}

void	irc_server::check_pollable_discriptors(int &pollfd_size)
{
	int		size = pollfd_size;
	this->check_poll_fd = false;
	for (int i = 0; i < size; i++)
	{
		if (poll_fd[i].revents == 0)
			continue;
		if (poll_fd[i].revents & POLLHUP) // check & POLLHUP
		{
			std::cout << "client " << poll_fd[i].fd << " disconnected." << std::endl;
			delete_client(poll_fd[i].fd);
			close(poll_fd[i].fd);
			clients.erase(poll_fd[i].fd);
			poll_fd[i].fd = -1;
			check_poll_fd = true;
			continue;
		}
		if (poll_fd[i].revents != POLLIN)
			error("poll() returned unexpected revents value", poll_fd[i].revents);
		if (poll_fd[i].fd == this->sock_fd)
			this->fd_is_socket(pollfd_size);
		else
			this->fd_is_client(i);
	}
}

std::string	irc_server::get_pass(void)
{
	return (this->password);
}

irc_server::~irc_server()
{
	return;
}
