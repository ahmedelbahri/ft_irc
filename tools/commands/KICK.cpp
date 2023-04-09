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
	if (args.empty() || (int)args.find_first_of(ISSPACE) < 0)
		send_error(this->fd, ":" + this->nick + " 461 KICK :Not enough parameters\n");
	else
	{
		std::string	chan = args.substr(0, args.find_first_of(ISSPACE));
		args = args.substr(args.find_first_of(ISSPACE));
		args = args.substr(args.find_first_not_of(ISSPACE));
		std::string	nicks = args.substr(0, args.find_first_of(ISSPACE));
		int			pos = (args.find_first_of(" "));
		std::string	reason = " for reason :";
		if (pos < 0)
			reason = "";
		else
			reason += args.substr(pos + 1);
		if (channels.find(chan) == channels.end())
			send_error(this->fd, ":" + this->nick + " 403 " + chan + " :No such channel\n");
		else if (!isElementInVector(channels[chan].get_opp(), this->fd))
			send_error(this->fd, ":" + this->nick + " 482 " + chan + " :You're not channel operator\n");
		else
			while (nicks.size() > 0)
			{
				std::string	nick = nicks.substr(0, nicks.find_first_of(","));
				if (check_if_user_exist(nick) == -1)
					send_error(this->fd, ":" + this->nick + " 401 " + nick + " :No such nick\n");
				else if (nick == this->nick)
					send_error(this->fd, ":" + this->nick + " 431 " + nick + " :You can't kick yourself\n");
				else if (!check_if_user_in_channel(check_if_user_exist(nick), chan))
					send_error(this->fd, ":" + this->nick + " 441 " + nick + " :They aren't on that channel\n");
				else
				{
					send_error(check_if_user_exist(nick), ":" + this->nick + " KICK you from " + chan + reason + "\n");
					channels[chan].inform_members(":" + nick + " PART " + chan + reason + "\n", this->fd);
					send_error(this->fd, ":341 you kicked " + nick + " from " + chan + reason + "\n");
					eraseElementFromVector(channels[chan].get_members(), check_if_user_exist(nick));
					eraseElementFromVector(channels[chan].get_invites(), check_if_user_exist(nick));
					eraseElementFromVector(channels[chan].get_opp(), check_if_user_exist(nick));
				}
				if ((int)nicks.find_first_of(",") < 0)
					break;
				nicks = nicks.substr(nicks.find_first_of(",") + 1);
			}
	}
}