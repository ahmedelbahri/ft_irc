#include "../includes/irc_client.hpp"

irc_client::irc_client(void)
{
	this->authenticated = 0;
}

irc_client::~irc_client(void)
{
	return;
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

int	irc_client::get_auth(void)
{
	return (this->authenticated);
}

std::string	irc_client::get_nick(void)
{
	return (this->nick);
}

void	irc_client::exec_cmd(void)
{
	int len = this->buffer.find_first_of(" \n");
	std::string	cmd = this->buffer.substr(0, len);
	std::string	args = this->buffer.substr(len + 1, this->buffer.size());
	std::cout << cmd << std::endl;
	if (cmd == "PASS")
		this->PASS(args);
	if (cmd == "NICK")
		this->NICK(args);
}
