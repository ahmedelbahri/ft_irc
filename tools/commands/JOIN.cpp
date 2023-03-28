#include "../../includes/ft_irc.hpp"

static int	asign(std::string arg, std::map<std::string, std::string> &new_channels)
{
	arg = arg.substr(arg.find_first_not_of(" "), arg.find_last_not_of(" ") + 1);
	if (arg[0] == '#' && (int)arg.find(" ") < 0)
		new_channels[arg] = "";
	else if (arg[0] == '&' && (int)arg.find(" ") > 0 && arg[1] != ' ')
	{
		arg = arg.substr(arg.find_first_not_of(" "), arg.size());
		std::string	chen = arg.substr(0, arg.find(" "));
		new_channels[chen] = arg.substr(arg.find_first_not_of(" ", chen.size()), arg.find_last_not_of(" ") + 1);
	}
	else
		return (1);
	return (0);
}

static void	split_args(std::string args, std::map<std::string, std::string> &new_channels, irc_client client)
{
	int len;
	while ((len = args.find_first_of(",")) > 0)
	{
		if (asign(args.substr(0, len), new_channels))
			send_error(client.get_fd(), ":" + client.get_nick() + " 407 JOIN :\
public channel has no pass && private should have pass\n");
		args = args.substr(len + 1, args.size());
	}
	if (!args.empty()) // && (int)args.find_first_not_of(" ") > 0
		if (asign(args, new_channels))
			send_error(client.get_fd(), ":" + client.get_nick() + " 407 JOIN :\
public channel has no pass && private should have pass\n");
}

void	create_channels(std::map<std::string, std::string> new_channels, int fd)
{
	std::map<std::string, std::string>::iterator	it;
	for (it = new_channels.begin(); it != new_channels.end(); it++)
		if (channels.insert(std::pair<std::string, irc_channel>(it->first, irc_channel(it, fd))).second)
		{
			if (channels[it->first].get_mode())
			{
				if (std::find(channels[it->first].get_invites().begin(), channels[it->first].get_invites().end(), fd) != channels[it->first].get_invites().end())
					channels[it->first].add_member(fd);
				else if (channels[it->first].get_pass() == it->second)
					channels[it->first].add_member(fd);
				else
					send_error(fd, ":" + clients[fd].get_nick() + " 475 " + it->first + " :Cannot join channel (+k)\n");
			}
			else
				channels[it->first].add_member(fd);
		}
		else
				channels[it->first].add_member(fd);
}

void	irc_client::JOIN(std::string args)
{
	std::map<std::string, std::string>	new_channels;
	if (args.empty() || (int)args.find_first_not_of(" \t\v\f\r") < 0)
		send_error(this->fd, ":" + this->nick + "461 JOIN :Not enough parameters\n");
	else
	{
		split_args(args, new_channels, *this);
		create_channels(new_channels, this->fd);
	}
}
