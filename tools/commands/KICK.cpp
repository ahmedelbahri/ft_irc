#include "../../includes/ft_irc.hpp"

bool	check_if_user_in_channel(int user, std::string chan)
{
	for (std::vector<int>::iterator it = channels[chan].get_members().begin(); it != channels[chan].get_members().end(); it++)
		if (*it == user)
			return (1);
	return (0);
}

void	irc_client::KICK(std::string args)
{
	if (args.empty() || (int)args.find_first_not_of(" \t\v\f\r") < 0)
		send_error(this->fd, ":" + this->nick + " 461 KICK :Not enough parameters\n");
	else
		if ((int)args.find(" ") <= 0)
			send_error(this->fd, ":" + this->nick + " 461 KICK :Not enough parameters\n");
		else
		{
			std::string	nick = args.substr(0, args.find(" "));
			std::string	chan = args.substr(args.find_first_not_of(" ", nick.size()), args.find_last_not_of(" ") + 1);
			if (channels.find(chan) == channels.end())
				send_error(this->fd, ":" + this->nick + " 403 " + chan + " :No such channel\n");
			else if (!isElementInVector(channels[chan].get_opp(), this->fd))
				send_error(this->fd, ":" + this->nick + " 482 " + chan + " :You're not channel operator\n");
			else if (check_if_user_exist(nick) == -1)
				send_error(this->fd, ":" + this->nick + " 401 " + nick + " :No such nick\n");
			else if (!check_if_user_in_channel(check_if_user_exist(nick), chan))
				send_error(this->fd, ":" + this->nick + " 442 " + nick + " :user not on that channel\n");
			else
			{
				send_error(check_if_user_exist(nick), ":" + this->nick + " KICK you from " + chan + "\n");
				send_error(this->fd, ": 341 you kicked " + nick + " from " + chan + "\n");
				eraseElementFromVector(channels[chan].get_members(), check_if_user_exist(nick));
			}
		}
}