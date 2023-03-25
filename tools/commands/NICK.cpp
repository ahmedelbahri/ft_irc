#include "../../includes/irc_client.hpp"

bool	check_if_nick_used(std::string arg)
{
	std::map<int, irc_client>::iterator	it;
	for (it = clients.begin(); it != clients.end(); it++)
		if (it->second.get_auth() == 2 && it->second.get_nick() == arg)
			return (1);
	return (0);
}

void	irc_client::NICK(std::string args)
{
	if (args.empty())
		send_error(this->fd, ERR_NONICKNAMEGIVEN);
	else if (!this->authenticated)
		send_error(this->fd, ERR_NICKBEFOREPASS);
	else if (!check_if_nick_used(args))
		this->nick = args;
	else if (check_if_nick_used(args))
		send_error(this->fd, "433 * " + args + " :Nickname is already in use\n");
}