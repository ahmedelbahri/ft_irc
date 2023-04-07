#include "../../includes/ft_irc.hpp"

int	get_arr(std::string str, std::vector<std::string> &arr)
{
	if ((int)str.find_first_of(ISSPACE) >= 0)
		return (1);
	std::string res;
	while (!str.empty())
	{
		if ((int)str.find_first_of(",") < 0)
			res = str, str.clear();
		else
		{
			res = str.substr(0, str.find_first_of(","));
			str = str.substr(str.find_first_of(",") + 1);
		}
		if (!res.empty())
			arr.push_back(res);
	}
	return (0);
}

int	split_args(std::string args, std::map<std::string, std::string> &new_channels, irc_client client)
{
	std::vector<std::string>	channels;
	std::vector<std::string>	keys;

	get_arr(args.substr(0, args.find_first_of(ISSPACE)), channels);
	if ((int)args.find_first_of(ISSPACE) >= 0)
	{
		args = args.substr(args.find_first_of(ISSPACE) + 1, args.size());
		if ((int)args.find_first_of(ISSPACE) >= 0)
			return (send_error(client.get_fd(), ":" + client.get_nick() + "461 JOIN :Too many parameters\n"), 1);
		get_arr(args.substr(args.find_first_not_of(ISSPACE), args.size()), keys);
	}
	if (channels.size() < keys.size())
		return (send_error(client.get_fd(), ":" + client.get_nick() + "461 JOIN :Too many keys\n"), 1);
	std::vector<std::string>::iterator it2 = keys.begin();
	for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		if (it2 != keys.end())
			new_channels.insert(std::pair<std::string, std::string>(*it, *it2)), it2++;
		else
			new_channels.insert(std::pair<std::string, std::string>(*it, ""));
	}
	return (0);
}

bool	isElementInVector(const std::vector<int>& vec, int elem) {
	for (std::vector<int>::const_iterator it = vec.begin(); it != vec.end(); ++it)
		if (*it == elem)
			return true;
	return false;
}

void	create_channels(std::map<std::string, std::string> new_channels,irc_client client)
{
	std::map<std::string, std::string>::iterator	it;
	for (it = new_channels.begin(); it != new_channels.end(); it++)
	{
		if (it->first[0] != '#' && it->first[0] != '&')
			return (send_error(client.get_fd(), ":" + client.get_nick() + "460 JOIN : private channel begin with '&' and public with '#'\n"), void());
		if (channels[it->first].get_members().empty())
			channels.erase(it->first);
		channels.insert(std::pair<std::string, irc_channel>(it->first, irc_channel(it, client.get_fd())));
		if (isElementInVector(channels[it->first].get_members(), client.get_fd()))
			(send_error(client.get_fd(), ": 443 * " + clients[client.get_fd()].get_nick() + " " + it->first + " :is already on channel\n"), 0);
		else
		{
			if (channels[it->first].get_mode())
			{
				if (channels[it->first].get_members().empty())
					channels[it->first].add_member(client.get_fd());
				else if (isElementInVector(channels[it->first].get_invites(), client.get_fd()))
					if (channels[it->first].get_pass() == it->second)
						channels[it->first].add_member(client.get_fd());
					else
						send_error(client.get_fd(), ":" + clients[client.get_fd()].get_nick() + "475 " + it->first + " :Cannot join channel password error (+k)\n");
				else
					send_error(client.get_fd(), ":" + clients[client.get_fd()].get_nick() + "473 " + it->first + " :Cannot join channel you should be invited (+i)\n");
			}
			else if (!channels[it->first].get_mode())
				if (channels[it->first].get_pass() == it->second)
					channels[it->first].add_member(client.get_fd());
				else
					send_error(client.get_fd(), ":" + clients[client.get_fd()].get_nick() + "475 " + it->first + " :Cannot join channel password error (+k)\n");
			else
				send_error(client.get_fd(), ":" + clients[client.get_fd()].get_nick() + " 475 " + it->first + " :Cannot join channel (+k)\n");
		}
	}
}


void	irc_client::JOIN(std::string args)
{
	std::map<std::string, std::string>	new_channels;
	if (args.empty())
		send_error(this->fd, ":" + this->nick + "461 JOIN :Not enough parameters\n");
	else
	{
		if (split_args(args, new_channels, *this) == 0)
			create_channels(new_channels, *this);
	}
}
