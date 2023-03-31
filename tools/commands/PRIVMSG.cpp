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



static void	split_args(std::string args, std::vector<std::string> &targets, std::string &msg)
{
	// std::vector<std::string>	channels;
	args = args.substr(args.find_first_not_of(" "), args.length());
	args = args.substr(0, args.find_last_not_of(" ") + 1);
	get_arr(args.substr(0, args.find_first_of(" ")), targets);
	msg = args.substr(args.find_first_of(" ") + 1, args.find_last_not_of(" ") + 1);
	std::cout << "msg: |" << msg << "|" << std::endl;

}

void	send_msg_to_user(std::string target, int flag, irc_client client, std::string msg)
{
	if (flag == 0)
		send_error(check_if_user_exist(target), ":" + client.get_nick() + " PRIVMSG " + target + " :" + msg + "\n");
	else if (flag == 1)
	{
		std::cout << "send msg to channel " << target << std::endl;
		for (std::vector<int>::iterator it = channels[target].get_members().begin(); it != channels[target].get_members().end(); it++)
			if (*it == client.get_fd())
				continue;
			else
				send_error(*it, ":" + client.get_nick() + " PRIVMSG " + target + " :" + msg + "\n");
	}
}

int	check_all_targets(std::vector<std::string> &targets, irc_client client, std::string msg)
{
	for (std::vector<std::string>::iterator it = targets.begin(); it != targets.end(); it++)
		if (channels.find(*it) == channels.end())
			if (check_if_user_exist(*it) == -1)
				return (send_error(client.get_fd(), ":" + client.get_nick() + " 401 " + *it + " :No such nick/channel\n"), 1);
			else
				return (send_msg_to_user(*it, 0, client, msg), 0);
		else 
			if (!check_if_user_in_channel(client.get_fd(), *it))
				send_error(client.get_fd(), ":" + client.get_nick() + " 404 " + *it + " :Cannot send to channel\n");
			else
				send_msg_to_user(*it, 1, client, msg);
	return (1);
}

void	irc_client::PRIVMSG(std::string args)
{
	if (args.empty() || (int)args.find_first_not_of(" \t\v\f\r") < 0)
		send_error(this->fd, ":" + this->nick + " 461 PRIVMSG :Not enough parameters\n");
	else
	{
		std::vector<std::string>	targets;
		std::string					msg;
		if ((int)args.find(" ") < 0)
			send_error(this->fd, ":" + this->nick + " 412 PRIVMSG :No text to send\n");
		else
		{
			split_args(args, targets, msg);
			check_all_targets(targets, *this, msg);
				// send_msg_to_user(targets[0], 0, *this, msg);
		}
	}
}