#ifndef IRC_SERVER_HPP
#define IRC_SERVER_HPP

#define MAX_QUEUE 20

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <cstdlib>
#include <signal.h>

extern pollfd			poll_fd[MAX_QUEUE];
extern socklen_t		addr_len;
extern int				pollfd_size;

class irc_server
{
private:
	int					dummy; // tan3arfoh o nsemiwh
	int					port;
	std::string			password;
	int					sock_fd;
	struct sockaddr_in	addr;
	bool				check_poll_fd;
public:
						irc_server(int port, std::string password);
	int					set_fd(int socket_fd);
	int					get_port(void);
	int					fd(void);
						~irc_server(void);

	void				init_server(void);
	void				check_pollable_discriptors(int &pollfd_size);
	void				remove_closed_discriptors(int &pollfd_size);
	void				fd_is_socket(int &pollfd_size);
	void				fd_is_client(int i);
};

#endif
