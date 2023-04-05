#include "../../includes/ft_irc.hpp"

// int	check_if_flag_exist(std::string modestring)
// {
// 	std::string	flags = "biklmnopstrv";
// 	int i = 0;
// 	if ((modestring[i] == '+' || modestring[i] == '-') && flags.find())
// 		return (1);
// 	return (0);
// }

void	execute_channel(std::string args, irc_client client)
{
	std::string	mode = "channel";
	if ((int)args.find_first_of(ISSPACE) < 0)
	{
		///execute target alone
		send_error(client.get_fd(), ":" + client.get_nick() + " 461 MODE :target bohda " + mode + "\n");
	}
	else
	{
		args = args.substr(args.find_first_of(ISSPACE));
		args = args.substr(args.find_first_not_of(ISSPACE));
		if ((int)args.find_first_of(ISSPACE) < 0)
		{
			///execute target with only modestring
			// if (check_if_flag_exist())
			// {
			// 	///execute target with modestring and params
			// 	send_error(client.get_fd(), ":" + client.get_nick() + " 461 MODE :target w modestring w params " + mode + "\n");
			// }
			send_error(client.get_fd(), ":" + client.get_nick() + " 461 MODE :target w modestring " + mode + "\n");
		}
		else
		{
			///execute target with modestring and params
			send_error(client.get_fd(), ":" + client.get_nick() + " 461 MODE :target w modestring w params " + mode + "\n");
		}
	}
}

// void	execute_user(std::string args, irc_client client)
// {
// 	std::string	mode = "user";
// 	if ((int)args.find_first_of(ISSPACE) < 0)
// 	{
// 		///execute target alone
// 		send_error(client.get_fd(), ":" + client.get_nick() + " 221 MODE :target bohda " + mode + "\n");
// 	}
// 	else
// 	{
// 		args = args.substr(args.find_first_of(ISSPACE));
// 		args = args.substr(args.find_first_not_of(ISSPACE));
// 		if ((int)args.find_first_of(ISSPACE) < 0)
// 		{
// 			///execute target with only modestring
// 			send_error(client.get_fd(), ":" + client.get_nick() + " 461 MODE :target w modestring " + mode + "\n");
// 		}
// 		else
// 		{
// 			///execute target with modestring and params
// 			send_error(client.get_fd(), ":" + client.get_nick() + " 461 MODE :target w modestring w params " + mode + "\n");
// 		}
// 	}
// }

void	irc_client::MODE(std::string args)
{
	std::cout << "MODE" << args << std::endl;
	if (args.empty())
		send_error(this->fd, ":" + this->nick + " 461 MODE :Not enough parameters\n");
	else
	{
		std::string	target = args.substr(0, args.find_first_of(ISSPACE));
		args = args.substr(args.find_first_of(ISSPACE));
		args = args.substr(args.find_first_not_of(ISSPACE));
		if (args.empty())
			send_error(this->fd, ":" + this->nick + " 461 MODE :Not enough parameters\n");
		else
		{
			if (target[0] == '#' || target[0] == '&')
			{
				if (channels.find(target) == channels.end())
					send_error(this->fd, ":" + this->nick + " 403 " + target + " :No such channel\n");
				else
					execute_channel(args, *this);
			}
			else
			{
				if (clients.find(check_if_user_exist(target)) == clients.end())
					send_error(this->fd, ":" + this->nick + " 406 " + target + " :There was no such nickname\n");
				else if (clients[check_if_user_exist(target)].get_nick() != this->nick)
					send_error(this->fd, ":" + this->nick + " 502 " + target + " :Can't change mode for other users\n");
				else
					;
					// execute_user(args, *this);
			}
		}
	}
}