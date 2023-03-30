#include "../../includes/irc_channel.hpp"
static void	get_arr(std::string str, std::vector<std::string> &arr)
{
	int i = 0;
	if ((int)str.find(" ") < 0)
		return (arr.push_back(str), void());
	str = str.substr(str.find_first_not_of(" "), str.length());
	str = str.substr(0, str.find_last_not_of(" ") + 1);
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

static void	split_args(std::string args, std::vector<std::string> &targets)
{
	std::vector<std::string>	channels;
	args = args.substr(args.find_first_not_of(" "), args.length());
	args = args.substr(0, args.find_last_not_of(" ") + 1);
	get_arr(args.substr(0, args.find_first_of(" ")), channels);
}

void	check_all_targets(std::vector<std::string> &targets, irc_client client)
{
	for (std::vector<std::string>::iterator it = targets.begin(); it != targets.end(); it++)
	{
		if (channels.find(*it) == channels.end() && check_if_user_exist(*it) == -1)
			send_error(client.get_fd(), ":" + client.get_nick() + " 401 " + *it + " :No such nick/channel\n");
		else if (channels.find(*it) != channels.end() && !check_if_user_in_channel(client.get_fd(), *it))
			send_error(client.get_fd(), ":" + client.get_nick() + " 404 " + *it + " :Cannot send to channel\n");
	}
}

void	irc_client::PRIVMSG(std::string args)
{
	if (args.empty() || (int)args.find_first_not_of(" \t\v\f\r") < 0)
		send_error(this->fd, ":" + this->nick + " 461 PART :Not enough parameters\n");
	else
	{
		std::vector<std::string>	targets;
		if ((int)args.find(" ") < 0)
			send_error(this->fd, ":" + this->nick + " 412 PART :No text to send\n");
		else
		{

		}
	}
}