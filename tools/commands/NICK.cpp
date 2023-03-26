#include "../../includes/irc_client.hpp"

int	check_if_nick_used(std::string arg)
{
	std::map<int, irc_client>::iterator	it;
	for (it = clients.begin(); it != clients.end(); it++)
	{
		if (it->second.get_auth() == 2 && it->second.get_nick() == arg)
			return (1);
		else if (it->second.get_auth() == 1 && it->second.get_nick() == arg)
			return (2);
	}
	return (0);
}

void	irc_client::NICK(std::string args)
{
	if (args.empty())
		send_error(this->fd, ERR_NONICKNAMEGIVEN);
	else if (!this->authenticated)
		send_error(this->fd, ERR_NICKBEFOREPASS);
	else if (!check_if_nick_used(args))
	{
		this->nick = args;
		if (this->username[0])
		{
			send(this->fd, this->msg_auth().c_str(), this->msg_auth().size(), 0);
			this->authenticated = 2;
		}
	}
	else if (check_if_nick_used(args) == 1)
		send_error(this->fd, "433 * " + args + " :Nickname is already in use\n");
	else if (check_if_nick_used(args) == 2)
		send_error(this->fd, "436 * " + args + " :Nickname is already in use\n");
}