#include "../../includes/ft_irc.hpp"

static std::string	split_args(std::string args, std::vector<std::string> &part_channels)
{
	std::string	reason;
	if ((int)args.find_first_of(ISSPACE) > 0)
	{
		reason = args.substr(args.find_first_of(" ") + 1);
		reason = " because of reason :" + reason.substr(reason.find_first_not_of(" "));
	}
	args = args.substr(0, args.find_first_of(" "));
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
			part_channels.push_back(res);
	}
	return (reason);
}

void	eraseElementFromVector(std::vector<int>& vec, int elem) {
	for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); ++it) {
		if (*it == elem) {
			vec.erase(it);
			break ;
		}
	}
}

void	part_from_channels(std::vector<std::string> &channels_to_part, std::string reason, irc_client client)
{
	std::vector<std::string>::iterator it = channels_to_part.begin();

	for (it = channels_to_part.begin(); it != channels_to_part.end(); it++)
		if (channels.find(*it) == channels.end())
			send_error(client.get_fd(), ":" + client.get_nick() + " 403 " + *it + " :No such channel\n");
		else
		{
			if (!isElementInVector(channels[*it].get_members(), client.get_fd()))
				send_error(client.get_fd(), ":" + client.get_nick() + " 442 " + *it + " :You're not on that channel\n");
			else
			{
				std::string msg = ":" + client.get_nick() + " PART " + *it + reason + "\n";
				eraseElementFromVector(channels[*it].get_members(), client.get_fd());
				eraseElementFromVector(channels[*it].get_invites(), client.get_fd());
				eraseElementFromVector(channels[*it].get_opp(), client.get_fd());
				if (channels[*it].get_opp().empty())
					if (!channels[*it].get_members().empty())
						channels[*it].get_opp().push_back(channels[*it].get_members()[0]);
				send_error(client.get_fd(), msg);
				channels[*it].inform_members(msg, client.get_fd());
			}
		}
}

void	irc_client::PART(std::string args)
{
	std::vector<std::string> channels_to_part;
	if (args.empty())
		send_error(this->fd, ":" + this->nick + " 461 PART :Not enough parameters\n");
	else
		part_from_channels(channels_to_part, split_args(args, channels_to_part), *this);
}