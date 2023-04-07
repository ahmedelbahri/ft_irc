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
	if (args.empty() || (int)args.find_first_of(ISSPACE) < 0)
		send_error(this->fd, ":" + this->nick + " 461 INVITE :Not enough parameters\n");
	else
	{
		std::string	nick = args.substr(0, args.find_first_of(ISSPACE));
		args = args.substr((int)args.find_first_of(ISSPACE) > 0 ? args.find_first_of(ISSPACE) : 0);
		std::string	chan = args.substr((int)args.find_first_not_of(ISSPACE) > 0 ? args.find_first_not_of(ISSPACE) : 0);
		if (channels.find(chan) == channels.end())
			send_error(this->fd, ":" + this->nick + " 403 " + chan + " :No such channel\n");
		else if (check_if_user_exist(nick) == -1)
			send_error(this->fd, ":" + this->nick + " 401 " + nick + " :No such nick\n");
		else if (!isElementInVector(channels[chan].get_opp() , this->fd))
			send_error(this->fd, ":" + this->nick + " 482 " + chan + " :You're not channel operator\n");
		else if (isElementInVector(channels[chan].get_members(), check_if_user_exist(nick)))
			send_error(this->fd, ":" + this->nick + " 443 " + nick + " " + chan + " :is already on channel\n");
		else if (!channels[chan].get_mode())
			send_error(this->fd, ":" + this->nick + " 442 :channel not in mode invite\n");
		else
		{
			send_error(check_if_user_exist(nick), ":" + this->nick + " INVITE you to " + chan + "\n");
			send_error(this->fd, ": 341 you invited " + nick + " to " + chan + "\n");
			channels[chan].get_invites().push_back(check_if_user_exist(nick));
		}
	}
}