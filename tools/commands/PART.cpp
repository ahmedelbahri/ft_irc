#include "../../includes/ft_irc.hpp"

static void	get_arr(std::string str, std::vector<std::string> &arr)
{
	int i = 0;
	if ((int)str.find(" ") < 0)
		return (arr.push_back(str), void());
	str = str.substr(str.find_first_not_of(" "), str.length());
	str = str.substr(0, str.find_last_not_of(" ") + 1);
	// if ((int)str.find(" ") >= 0)
	// 	return (std::cout << "error key should not have ' '" << std::endl, void()); 
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

static void	split_args(std::string args, std::map<std::string, std::string> &new_channels)
{
	std::vector<std::string>	channels;
	std::vector<std::string>	reason;
	args = args.substr(args.find_first_not_of(" "), args.length());
	args = args.substr(0, args.find_last_not_of(" ") + 1);
	get_arr(args.substr(0, args.find_first_of(" ")), channels);
	// get_arr(args.substr(args.find_first_of(" "), args.size()), reason);
	// if (channels.size() != reason.size())
	// 	return (std::cout << "error each channel should have a reason" << std::endl, void());
	// std::vector<std::string>::iterator it2 = reason.begin();
	for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		new_channels.insert(std::pair<std::string, std::string>(*it, ""));
	}
}

void	eraseElementFromVector(std::vector<int>& vec, int elem) {
	for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); ++it) {
		if (*it == elem) {
			vec.erase(it);
			break ;
		}
	}
}

void	part_from_channels(std::map<std::string, std::string> &channels_to_part, irc_client client)
{
	std::map<std::string, std::string>::iterator it = channels_to_part.begin();

	for (it = channels_to_part.begin(); it != channels_to_part.end(); it++)
		if (channels.find(it->first) == channels.end())
			send_error(client.get_fd(), ":" + client.get_nick() + " 403 " + it->first + " :No such channel\n");
		else
		{
			if (!isElementInVector(channels[it->first].get_members(), client.get_fd()))
				send_error(client.get_fd(), ":" + client.get_nick() + " 442 " + it->first + " :You're not on that channel\n");
			else
			{
				std::string msg = ":" + client.get_nick() + " PART " + it->first + "\n";
				eraseElementFromVector(channels[it->first].get_members(), client.get_fd());
				if (channels[it->first].get_members().empty())
					channels.erase(it->first);
				send_error(client.get_fd(), msg);
				channels[it->first].inform_members(msg, client.get_fd());
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
