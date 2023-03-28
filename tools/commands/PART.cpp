#include "../../includes/ft_irc.hpp"

static void	asign(std::string arg, std::map<std::string, std::string> &channels_to_part)
{
	arg = arg.substr(arg.find_first_not_of(" "), arg.find_last_not_of(" ") + 1);
	// if ((int)arg.find(" ") > 0 && arg[1] != ' ')
	// {
		arg = arg.substr(arg.find_first_not_of(" "), arg.size());
		std::string	chen = arg.substr(0, arg.find(" "));
		channels_to_part[chen] = arg.substr(arg.find_first_not_of(" ", chen.size()), arg.find_last_not_of(" ") + 1);
	// }
	// else
	// 	return (1);
	// return (0);
}

// static void	split_args(std::string args, std::map<std::string, std::string> &channels_to_part, irc_client client)
// {
// 	int len;
// 	while ((len = args.find_first_of(",")) > 0)
// 	{
// 		if (asign(args.substr(0, len), channels_to_part))
// 			send_error(client.get_fd(), ":" + client.get_nick() + " 407 JOIN :\
// public channel has no pass && private should have pass\n");
// 		args = args.substr(len + 1, args.size());
// 	}
// 	if (!args.empty()) // && (int)args.find_first_not_of(" ") > 0
// 		if (asign(args, channels_to_part))
// 			send_error(client.get_fd(), ":" + client.get_nick() + " 407 JOIN :\
// public channel has no pass && private should have pass\n");
// }


void	part_from_channels(std::map<std::string, std::string> &channels_to_part, irc_client client)
{
	std::map<std::string, std::string>::iterator it = channels_to_part.begin();
	for (it = channels_to_part.begin(); it != channels_to_part.end(); it++)
	{
		if (channels.find(it->first) == channels.end())
			send_error(client.get_fd(), ":" + client.get_nick() + " 403 " + it->first + " :No such channel\n");
		else
		{
			if (std::find(channels[it->first].get_members().begin(), channels[it->first].get_members().end(), client.get_fd()) == channels[it->first].get_members().end())
				send_error(client.get_fd(), ":" + client.get_nick() + " 442 " + it->first + " :You're not on that channel\n");
			else
			{
				std::string msg = ":" + client.get_nick() + " PART " + it->first + "\n";
				channels[it->first].get_members().erase(std::find(channels[it->first].get_members().begin(),channels[it->first].get_members().end(), client.get_fd()));
				send_error(client.get_fd(), msg);
				channels[it->first].inform_members(msg, client.get_fd());
			}
		}
	}
}

void	irc_client::PART(std::string args)
{
	std::map<std::string, std::string> channels_to_part;
	if (args.empty() || (int)args.find_first_not_of(" \t\v\f\r") < 0)
		send_error(this->fd, ":" + this->nick + " 461 PART :Not enough parameters\n");
	else
	{
		asign(args, channels_to_part);
		part_from_channels(channels_to_part, *this);
	}
	// std::map<std::string, std::string> channels_to_part;
	// get_channels_to_part(args, channels_to_part);
	// std::string msg = ":" + this->nick + " PART " + channel.get_name() + "\n";
	// channel.members.erase(std::remove(channel.members.begin(), channel.members.end(), this->fd), channel.members.end());
}
