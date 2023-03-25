#ifndef IRC_CLIENT_HPP
#define IRC_CLIENT_HPP

#include "ft_irc.hpp"

class irc_client
{
private:
	std::string	buffer;
	std::string	nick;
	int			fd;
public:
	irc_client();
	// void		set_buffer(std::string buff);
	std::string	&buff();
	void		set_fd(int buff);
	int			get_fd();
	~irc_client();
};

#endif
