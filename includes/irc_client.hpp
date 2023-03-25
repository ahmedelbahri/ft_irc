#ifndef IRC_CLIENT_HPP
#define IRC_CLIENT_HPP

#include "ft_irc.hpp"

class irc_client
{
private:
	int			fd;
	std::string	buffer;
	std::string	nick;
	bool		authenticated;
public:
				irc_client(void);
	std::string	&buff(void);
	void		set_fd(int fd);
	int			get_fd(void);
	void		exec_cmd(void);
	void		PASS(std::string args);
	void		NICK(std::string args);
				~irc_client(void);
};

#endif
