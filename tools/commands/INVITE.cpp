#include "../../includes/ft_irc.hpp"

int	check_if_user_exist(std::string nick)
{
	for (std::map<int, irc_client>::iterator it = clients.begin(); it != clients.end(); it++)
		if (it->second.get_nick() == nick)
			return (it->second.get_fd());
	return (-1);
}

void	irc_client::INVITE(std::string args)
{
	if (args.empty() || (int)args.find_first_not_of(" \t\v\f\r") < 0)
		send_error(this->fd, ":" + this->nick + " 461 INVITE :Not enough parameters\n");
	else
		if ((int)args.find(" ") < 0)
			send_error(this->fd, ":" + this->nick + " 461 INVITE :Not enough parameters\n");
		else
		{
			std::string	nick = args.substr(0, args.find(" "));
			std::string	chan = args.substr(args.find_first_not_of(" ", nick.size()), args.find_last_not_of(" ") + 1);
			if (channels.find(chan) == channels.end())
				send_error(this->fd, ":" + this->nick + " 403 " + chan + " :No such channel\n");
			else if (!isElementInVector(channels[chan].get_opp() , this->fd))
				send_error(this->fd, ":" + this->nick + " 482 " + chan + " :You're not channel operator\n");
			else if (check_if_user_exist(nick) == -1)
				send_error(this->fd, ":" + this->nick + " 401 " + nick + " :No such nick\n");
			else
			{
				send_error(check_if_user_exist(nick), ":" + this->nick + " INVITE you to" + chan + "\n");
				send_error(this->fd, ": 341 you invited " + this->nick + "to" + chan + "\n");
				channels[chan].get_invites().push_back(check_if_user_exist(nick));
			}
		}
}