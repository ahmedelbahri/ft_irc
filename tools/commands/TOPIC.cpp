#include "../../includes/ft_irc.hpp"

void	irc_client::TOPIC(std::string args)
{
	if (args.empty() || (int)args.find_first_not_of(" \t\v\f\r") < 0)
		send_error(this->fd, ":" + this->nick + " 461 TOPIC :Not enough parameters\n");
	else
	{
		std::string	chan = args.substr(0, args.find(" "));
		if (channels.find(chan) == channels.end())
			send_error(this->fd, ":" + this->nick + " 403 " + chan + " :No such channel\n");
		else if ((int)args.find(" ") > 0 && channels[chan].get_opp() != this->fd)
			send_error(this->fd, ":" + this->nick + " 482 " + chan + " :You're not channel operator\n");
		else if (!check_if_user_in_channel(this->fd, chan))
			send_error(this->fd, ":" + this->nick + " 442 " + nick + " :you are not on that channel\n");
		else if ((int)args.find(" ") <= 0)
			send_error(fd, ":" + clients[fd].get_nick() + (this->topic.empty() ? "331 JOIN :No topic is set\n" : "332 JOIN :" +this->topic + "\n"));
		else if ((int)args.find(" ") > 0 && channels[chan].get_opp() == this->fd)
		{
			std::string	topic = args.substr(args.find_first_not_of(" ", nick.size()), args.find_last_not_of(" ") + 1);
			channels[chan].set_topic(topic);
			send_error(fd, ":" + clients[fd].get_nick() + " 332 " + chan + " :" + topic + "\n");
		}
	}
}