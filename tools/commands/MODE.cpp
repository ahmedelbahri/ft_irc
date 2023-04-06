#include "../../includes/ft_irc.hpp"

void	o_flag(char plumin, irc_client client, std::string target, std::string args)
{
	if (check_if_user_exist(args) != -1)
		if (plumin == '+')
			if (!isElementInVector(channels[target].get_opp(), check_if_user_exist(args)))
				return (channels[target].get_opp().push_back(check_if_user_exist(args)),
					send_error(check_if_user_exist(args), ": 442 " + args + " :You just got oper_privelage at"+ target +"\n")
					, void());
			else
				send_error(client.get_fd(), ":" + client.get_nick() + " 443 " + args + " " + target + " :is already channel opperator\n");
		else
			if (isElementInVector(channels[target].get_opp(), check_if_user_exist(args)))
				return (eraseElementFromVector(channels[target].get_opp(), check_if_user_exist(args)),
				send_error(check_if_user_exist(args), ": 442 " + args + " :You just lost oper_privelage at"+ target +"\n")
				, void());
			else
				send_error(client.get_fd(), ":" + client.get_nick() + " 442 " + args + " " + target + " :is not channel opperator\n");
	else
		return (send_error(client.get_fd(), ":" + client.get_nick() + " 401 " + args + " :No such nick\n"), void());
}

void	i_flag(char plumin, std::string target)
{
	if (plumin == '+')
		channels[target].get_mode() = true;
	else
		channels[target].get_mode() = false;
}

void	t_flag(char plumin, std::string target)
{
	if (plumin == '+')
		channels[target].get_topic_settable_by_op() = true;
	else
		channels[target].get_topic_settable_by_op() = false;
}

void	n_flag(char plumin, std::string target)
{
	if (plumin == '+')
		channels[target].get_message_from_non_member() = true;
	else
		channels[target].get_message_from_non_member() = false;
}

void	k_flag(char plumin, std::string target, std::string args)
{
	if(plumin == '+')
		channels[target].get_pass() = args;
	else
		channels[target].get_pass() = "";
}

void	execute_flag(char flag, char plumin, irc_client client, std::string target, std::string args)
{
	std::cout << "args =|"<<args<<"|\n";
	switch (flag)
	{
		case 'o':
			if (target.empty() || (int)args.find_first_of(ISSPACE) < 0)
				send_error(client.get_fd(), ":" + client.get_nick() + " 461 MODE :Not enough parameters\n");
			else
				o_flag(plumin, client, target, args);
			break;
		case 'i':
			i_flag(plumin, target);
			break;
		case 't':
			t_flag(plumin, target);
			break;
		case 'n':
			n_flag(plumin, target);
			break;
		case 'k':
			if (target.empty() || (args.empty() && plumin == '+'))
				send_error(client.get_fd(), ":" + client.get_nick() + " 461 MODE :Not enough parameters\n");
			else
				k_flag(plumin, target, args);
			break;
		default:
			send_error(client.get_fd(), ":" + client.get_nick() + " 461 MODE :501 * :"+ flag +"Unknown  flag\n");
	}
}

void	execute_flags(std::string modestring, irc_client client, std::string target, std::string args)
{
	if (modestring[0] == '+' || modestring[0] == '-')
		for (int i = 1; i < (int)modestring.size(); i++)
			execute_flag(modestring[i], modestring[0], client, target, args);
	else
		send_error(client.get_fd(), ":" + client.get_nick() + " 461 MODE :501 * :Unknown MODE flag\n");
}

void	list_all_mode_of_channel(std::string target, irc_client client)
{
	std::string	mode = "+";
	if (channels[target].get_mode())
		send_error(client.get_fd(), ":" + target + " channel is on mode Invite-Only\n");
	if (channels[target].get_topic_settable_by_op())
		send_error(client.get_fd(), ":" + target + " channel is on mode Only-Op-Can-Set-Topic\n");
	if (channels[target].get_message_from_non_member())
		send_error(client.get_fd(), ":" + target + " channel is on mode Only member can send private messages\n");
	if (!channels[target].get_pass().empty())
		send_error(client.get_fd(), ":" + target + " channel has a key\n");
}

void	execute_channel(std::string args, irc_client client, std::string target)
{

	if (args.empty())
	{
		list_all_mode_of_channel(target, client);
	}
	else
	{
		std::string	mode = args.substr((int)args.find_first_not_of(ISSPACE) > 0 ? args.find_first_not_of(ISSPACE) : 0, args.find_first_of(ISSPACE));

		args = args.substr(mode.size());
		if (!args.empty())
			args = args.substr(args.find_first_not_of(ISSPACE));
		execute_flags(mode, client, target, args);
	}
}

void	irc_client::MODE(std::string args)
{
	if (args.empty())
		send_error(this->fd, ":" + this->nick + " 461 MODE :Not enough parameters\n");
	else
	{
		std::string	target = args.substr(0, args.find_first_of(ISSPACE));
		args = args.substr((int)args.find_first_of(ISSPACE) > 0 ? args.find_first_of(ISSPACE) : 0);
		args = args.substr((int)args.find_first_not_of(ISSPACE) > 0 ? args.find_first_not_of(ISSPACE) : 0);
		if (channels.find(target) == channels.end())
			send_error(this->fd, ":" + this->nick + " 403 " + target + " :No such channel\n");
		else if (!isElementInVector(channels[target].get_opp(), this->get_fd()))
			send_error(this->fd, ":" + this->nick + " 482 " + target + " :You're not channel operator\n");
		else
			execute_channel(args, *this, target);
	}
}