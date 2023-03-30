#include "../../includes/ft_irc.hpp"

void	get_arr(std::string str, std::vector<std::string> &arr)
{
	int i = 0;
	str = str.substr(str.find_first_not_of(" "), str.length());
	str = str.substr(0, str.find_last_not_of(" ") + 1);
	std::cout << "get_arr" << "|" << str << "|" << std::endl;
	if ((int)str.find(" ") >= 0)
		return (std::cout << "error key should not have ' '" << std::endl, void()); 
	while (str[i])
	{
		if (str[i] == ',' && str[i + 1] == '\0')
			return (std::cout << "error key should not have ', '" << std::endl, void());
		if (str[i] == ',')
		{
			arr.push_back(str.substr(0, i));
			str = str.substr(i + 1, str.length());
			i = 0;
		}
		i++;
	}
	arr.push_back(str);
}

void	split_args(std::string args, std::map<std::string, std::string> &new_channels, irc_client client)
{
	std::vector<std::string>	channels;
	std::vector<std::string>	keys;

	args = args.substr(args.find_first_not_of(" "), args.length());
	args = args.substr(0, args.find_last_not_of(" ") + 1);
	get_arr(args.substr(0, args.find_first_of(" ")), channels);
	get_arr(args.substr(args.find_first_of(" "), args.size()), keys);
	if (channels.size() != keys.size())
		return (send_error(client.get_fd(), ":" + client.get_nick() + "463 JOIN : each channel should have a key\n"), void());
	std::vector<std::string>::iterator it2, it = keys.begin();
	for (it = channels.begin(); it != channels.end(); it++)
	{
		new_channels.insert(std::pair<std::string, std::string>(*it, *it2));
		it2++;
	}
}

bool	isElementInVector(const std::vector<int>& vec, int elem) {
	for (std::vector<int>::const_iterator it = vec.begin(); it != vec.end(); ++it)
		if (*it == elem)
			return true;
	return false;
}

void	send_msg(int fd, std::string msg)
{
	msg = msg + "\r\n";
	send(fd, msg.c_str(), msg.size(), 0);
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
			(send_msg(client.get_fd(), ": 443 * " + clients[client.get_fd()].get_nick() + " " + it->first + " :is already on channel"), 0);
		else
		{
			if (channels[it->first].get_mode())
			{
				if (isElementInVector(channels[it->first].get_invites(), client.get_fd()))
					if (channels[it->first].get_pass() == it->second)
						channels[it->first].add_member(client.get_fd());
					else
						send_error(client.get_fd(), ":" + clients[client.get_fd()].get_nick() + "475 " + it->first + " :Cannot join channel password error (+k)\n");
				else
					send_error(client.get_fd(), ":" + clients[client.get_fd()].get_nick() + "475 " + it->first + " :Cannot join channel you should be invited (+i)\n");
			}
			else if (!channels[it->first].get_mode() && channels[it->first].get_pass() == it->second)
				channels[it->first].add_member(client.get_fd());
			else
				send_error(client.get_fd(), ":" + clients[client.get_fd()].get_nick() + " 475 " + it->first + " :Cannot join channel (+k)\n");
		}
	}
}


void	irc_client::JOIN(std::string args)
{
	std::map<std::string, std::string>	new_channels;
	if (args.empty() || (int)args.find_first_not_of(" \t\v\f\r") < 0 || (int)args.find(" ") < 0)
		send_error(this->fd, ":" + this->nick + "461 JOIN :Not enough parameters\n");
	else
	{
		split_args(args, new_channels, *this);
		create_channels(new_channels, *this);
	}
}
