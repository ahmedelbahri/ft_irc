#include "../includes/irc_client.hpp"

irc_client::irc_client()
{
}

irc_client::~irc_client()
{
}

std::string	&irc_client::buff(void)
{
	return (this->buffer);
}

void	irc_client::set_fd(int fd)
{
    this->fd = fd;
}

int	irc_client::get_fd(void)
{
	return (this->fd);
}
