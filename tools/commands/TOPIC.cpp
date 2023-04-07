#include "../../includes/ft_irc.hpp"

void	irc_client::TOPIC(std::string args)
{
	if (args.empty())
		send_error(this->fd, ":" + this->nick + " 461 TOPIC :Not enough parameters\n");
	else
	{
		std::string	chan = args.substr(0, args.find_first_of(ISSPACE));
		args = args.substr(chan.size());
		args = args.substr((int)args.find_first_not_of(ISSPACE) > 0 ? (int)args.find_first_not_of(ISSPACE) : 0);
		if (channels.find(chan) == channels.end())
			send_error(this->fd, ":" + this->nick + " 403 " + chan + " :No such channel\n");
		else if (!args.empty() && !isElementInVector(channels[chan].get_opp() ,this->fd) && channels[chan].get_topic_settable_by_op())
			send_error(this->fd, ":" + this->nick + " 482 " + chan + " :You're not channel operator\n");
		else if (!check_if_user_in_channel(this->fd, chan))
			send_error(this->fd, ":" + this->nick + " 442 " + nick + " :you are not on that channel\n");
		else if (args.empty())
			send_error(fd, ":" + clients[fd].get_nick() + (channels[chan].get_topic().empty() ? "331 TOPIC :No topic is set\n" : "332 TOPIC :" + channels[chan].get_topic() + "\n"));
		else if (!args.empty() && ((channels[chan].get_topic_settable_by_op() && isElementInVector(channels[chan].get_opp() ,this->fd)) || !channels[chan].get_topic_settable_by_op()))
		{
			std::string	topic = args.substr(args.find_first_not_of(" ", nick.size()), args.find_last_not_of(" ") + 1);
			channels[chan].get_topic() = topic;
			send_error(fd, ":" + clients[fd].get_nick() + " 332 " + chan + " :" + topic + "\n");
		}
	}
}