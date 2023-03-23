#ifndef FT_IRC_HPP
#define FT_IRC_HPP

#define PORT 80
#define MAX_QUEUE 10

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <cstdlib>
// #include <netdb.h>

pollfd					poll_fd[MAX_QUEUE];

class irc_server
{
private:
	int					dummy; // tan3arfoh o nsemiwh
	int					port;
	std::string			password;
	int					sock_fd;
	struct sockaddr_in	addr;
public:
						irc_server(int port, std::string password);
	int					set_fd(int socket_fd);
	void				init_server(void);
	int					fd(void);
	int					get_port(void);
	void				check_pollable_discriptors(int &pollfd_size);
						~irc_server(void);
};

irc_server::irc_server(int port, std::string password)
{
	this->port = port;
	this->dummy = 1;
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

irc_server::~irc_server()
{
	return;
}

#endif
