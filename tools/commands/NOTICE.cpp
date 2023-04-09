#include "../../includes/irc_channel.hpp"

static void	split_args(std::string args, std::vector<std::string> &targets, std::string &msg)
{
	msg = args.substr(args.find_first_of(ISSPACE) + 1);
	msg = msg.substr(msg.find_first_not_of(ISSPACE));
	args = args.substr(0, args.find_first_of(ISSPACE));
	std::string res;
	while (!args.empty())
	{
		if ((int)args.find_first_of(",") < 0)
			res = args, args.clear();
		else
		{
			res = args.substr(0, args.find_first_of(","));
			args = args.substr(args.find_first_of(",") + 1);
		}
		if (!res.empty())
			targets.push_back(res);
	}
}

static void	send_msg(std::string target, int flag, irc_client client, std::string msg)
{
	if (flag == 0)
		send_error(check_if_user_exist(target), ":" + client.get_nick() + " NOTICE " + target + " :" + msg + "\n");
	else if (flag == 1)
		for (std::vector<int>::iterator it = channels[target].get_members().begin(); it != channels[target].get_members().end(); it++)
		{
			if (*it == client.get_fd())
				continue;
			else
				send_error(*it, ":" + client.get_nick() + " NOTICE " + target + " :" + msg + "\n");
		}
}

static void	check_all_targets(std::vector<std::string> &targets, irc_client client, std::string msg)
{
	for (std::vector<std::string>::iterator it = targets.begin(); it != targets.end(); it++)
	{
		if (channels.find(*it) == channels.end())
		{
			if (check_if_user_exist(*it) != -1)
				return (send_msg(*it, 0, client, msg), void());
		}
		else
		{
			if (channels[*it].get_message_from_non_member() && !check_if_user_in_channel(client.get_fd(), *it))
				continue;
			else
				send_msg(*it, 1, client, msg);
		}
	}
}

void	irc_client::NOTICE(std::string args)
{
	if (!args.empty())
	{
		std::vector<std::string>	targets;
		std::string					msg;

		if ((int)args.find_first_of(ISSPACE) > 0)
		{
			split_args(args, targets, msg);
			check_all_targets(targets, *this, msg);
		}
	}
}
