#ifndef IRC_CLIENT_HPP
#define IRC_CLIENT_HPP

#include "ft_irc.hpp"

class irc_client
{
private:
	int			fd;
	std::string	buffer;
	std::string	nick;
	std::string	username;
	int			authenticated;
public:
				irc_client(void);
	std::string	&buff(void);
	void		set_fd(int fd);
	int			get_fd(void);
	int			get_auth(void);
	std::string	get_nick(void);
	void		exec_cmd(void);
	std::string	msg_auth();
	void		PASS(std::string args);
	void		NICK(std::string args);
	void		USER(std::string args);
	void		JOIN(std::string args);
	void		PART(std::string args);
				~irc_client(void);
};

#endif
