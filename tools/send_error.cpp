#include "../includes/ft_irc.hpp"

void	send_error(int fd, std::string message)
{
	if (send(fd, message.c_str(), message.size(), 0) < 0)
		error("send() failed.");
}
