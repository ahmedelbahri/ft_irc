#ifndef IRC_SERVER_HPP
#define IRC_SERVER_HPP

#define MAX_QUEUE 20

#include "ft_irc.hpp"
#include "irc_client.hpp"

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
						irc_server(void);
	int					set_fd(int socket_fd);
	int					get_port(void);
	int					fd(void);
	void				set_server(int port, std::string password);
	void				init_server(void);
	void				check_pollable_discriptors(int &pollfd_size);
	void				remove_closed_discriptors(int &pollfd_size);
	void				fd_is_socket(int &pollfd_size);
	void				fd_is_client(int i);
	std::string			get_pass(void);
						~irc_server(void);
};

#endif
