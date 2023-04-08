#include "../includes/ft_irc.hpp"

void	delete_client(int fd)
{
	std::map<std::string, irc_channel>::iterator	it_channels;
	for (it_channels = channels.begin(); it_channels != channels.end(); it_channels++)
	{
		if (check_if_user_in_channel(fd, it_channels->first))
			it_channels->second.inform_members(":" + clients[fd].get_nick() + " PART " + it_channels->first + "\r\n", fd);
		eraseElementFromVector(it_channels->second.get_members(), fd);
		eraseElementFromVector(it_channels->second.get_invites(), fd);
		eraseElementFromVector(it_channels->second.get_opp(), fd);
		if (it_channels->second.get_opp().empty() && !it_channels->second.get_members().empty())
			it_channels->second.get_opp().push_back(it_channels->second.get_members()[0]);
	}
	clients.erase(fd);
}