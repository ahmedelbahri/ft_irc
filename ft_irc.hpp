#ifndef FT_IRC_HPP
#define FT_IRC_HPP

#define PORT 80
#define MAX_QUEUE 10

#include <set>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
// #include <netdb.h>

class irc_server
{
private:
	int			port;
	std::string	password;
	int			socket_fd;
public:
				irc_server(int port, std::string password);
	int			get_port(void);
	int			get_socket_fd(void);
	int		set_socket_fd(int socket_fd);
				~irc_server(void);
};

irc_server::irc_server(int port, std::string password)
{
	this->port = port;
	this->password = password;
	return;
}

int	irc_server::get_port(void)
{
	return (this->port);
}

int	irc_server::get_socket_fd(void)
{
	return (this->socket_fd);
}

int	irc_server::set_socket_fd(int socket_fd)
{
	this->socket_fd = socket_fd;
	return (this->socket_fd);
}

irc_server::~irc_server()
{
	return;
}

#endif
