#include "../../includes/ft_irc.hpp"

static int	asign(std::string arg, std::map<std::string, std::string> &channels_to_part)
{
	arg = arg.substr(arg.find_first_not_of(" "), arg.size());
	if (arg.find(" ") == std::string::npos)
		channels_to_part[arg] = "";
	else
	{
		std::string	chen = arg.substr(0, arg.find(" "));
		channels_to_part[chen] = arg.substr(arg.find_first_not_of(" ", chen.size()), arg.find_last_not_of(" ") + 1);
	}
	return (0);
}

static void	split_args(std::string args, std::map<std::string, std::string> &new_channels)
{
	int	len;
	while ((len = args.find_first_of(",")) > 0)
	{
		asign(args.substr(0, len), new_channels);
		args = args.substr(len + 1, args.size());
	}
	if (!args.empty()) // && (int)args.find_first_not_of(" ") > 0
		asign(args, new_channels);
}

void	part_from_channels(std::map<std::string, std::string> &channels_to_part, irc_client client)
{
	std::map<std::string, std::string>::iterator it = channels_to_part.begin();
	for (it = channels_to_part.begin(); it != channels_to_part.end(); it++)
	{
		if (channels.find(it->first) == channels.end())
			send_error(client.get_fd(), ":" + client.get_nick() + " 403 " + it->first + " :No such channel\n");
		else
		{
			std::vector<int>::iterator it2 = std::find(channels[it->first].get_members().begin(), channels[it->first].get_members().end(), client.get_fd());
			if (it2 == channels[it->first].get_members().end())
				send_error(client.get_fd(), ":" + client.get_nick() + " 442 " + it->first + " :You're not on that channel\n");
			else
			{
				std::string msg = ":" + client.get_nick() + " PART " + it->first + "\n";
				channels[it->first].get_members().erase(it2);
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
		split_args(args, channels_to_part);
		part_from_channels(channels_to_part, *this);
	}
}
