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
	// socket is a system call that creates a new communication endpoint (or socket)
	// and returns a file descriptor that refers to that endpoint

	// AF_INET is the address family that is used to designate the type of addresses
	// that your socket can communicate with (IPv4) Ex: 10.11.5.4

	// SOCK_STREAM is a connection-based protocol, which means that TCP is used.
	// TCP is a reliable stream protocol that guarantees that all data will be delivered
	// but slow compared to UDP
	if ((this->sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		error("socket() failed");

	// fcntl is a system call that allows the programmer to manipulate file descriptors
	// F_SETFL is a command that sets the file status flags to the value specified by arg
	// O_NONBLOCK is a flag that sets the file status flag to non-blocking mode
	if (fcntl(this->sock_fd, F_SETFL, O_NONBLOCK) == -1)
		error("fcntl() failed");

	// setsockopt is a system call that allows the programmer to manipulate the options
	// SOL_SOCKET is a level that allows the programmer to manipulate options at the socket API level
	// SO_REUSEADDR is a socket option that allows the socket to be bound to an address that is already in use
	// dummy is a variable that is used to set the value of the socket option
	if (setsockopt(this->sock_fd, SOL_SOCKET, SO_REUSEADDR, &(this->dummy), sizeof(this->dummy)) == -1)
		error("setsockopt() failed");

	bzero(&(this->addr), sizeof((this->addr)));
	(this->addr).sin_family = AF_INET;
	// htons is a function that converts a u_short from host to TCP/IP network byte order
	(this->addr).sin_port = htons(this->port);
	// htonl is a function that converts a u_long from host to TCP/IP network byte order
	(this->addr).sin_addr.s_addr = htonl(INADDR_ANY);

	// bind is a system call that assigns a local protocol address to a socket
	if (bind(this->sock_fd, (struct sockaddr *)&(this->addr), addr_len) == -1)
		error("bind() failed");

	// listen is a system call that marks a socket as a passive socket
	// meaning that it will be used to accept incoming connection requests using accept
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

	// accept is a system call that extracts the first connection request on the queue of pending connections
	if ((client_fd = accept(this->sock_fd, (struct sockaddr *)&(client.get_addr()), &addr_len)) < 0)
		std::cout << "accept ret: " << client_fd << std::endl, error("accept() failed.");
	client.set_addr();
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

void	irc_server::fd_is_client(int i)
{
	int			len;
	char		buffer[1024];
	while (true)
	{
		memset(buffer, 0, 1024);
		if ((len = recv(poll_fd[i].fd, buffer, 1024, 0)) < 0)
			break;
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
		if (!(poll_fd[i].revents & POLLIN))
			continue;
		if (poll_fd[i].revents & POLLHUP)
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
