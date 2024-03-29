#ifndef IRC_CLIENT_HPP
#define IRC_CLIENT_HPP

#include "ft_irc.hpp"

class irc_client
{
private:
	int					fd;
	std::string			buffer;
	std::string			nick;
	std::string			username;
	std::string 		numeric_address;
	int					authenticated;
	struct sockaddr_in	addr;
public:
						irc_client(void);
	std::string			&buff(void);
	void				set_fd(int fd);
	int					get_fd(void);
	int					get_auth(void);
	std::string			get_nick(void);
	void				exec_cmd(void);
	std::string			msg_auth();
	struct sockaddr_in	&get_addr(void);
	void				set_addr(void);
	std::string			get_num_addr(void);
	std::string			get_username(void);
	void				PASS(std::string args);
	void				NICK(std::string args);
	void				USER(std::string args);
	void				JOIN(std::string args);
	void				PART(std::string args);
	void				INVITE(std::string args);
	void				MODE(std::string args);
	void				KICK(std::string args);
	void				TOPIC(std::string args);
	void				PRIVMSG(std::string args);
	void				NOTICE(std::string args);
	void				BOT(std::string args);
						~irc_client(void);
};

#endif
