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

std::string	irc_client::msg_auth()
{
	return (":ahel-bid 001 " + this->nick + " : welcome to the internet relay chat\r\n"
					":ahel-bid 002 " + this->nick + " :Your host is ahel-bid, running version 1.0\r\n"
					":ahel-bid 003 " + this->nick + " :This server was created 28/12/2022\r\n"
					":ahel-bid 004 " + this->nick + " ahel-bid 1.0 - -\r\n"
					":ahel-bid 372 " + this->nick + " 🔨 welcome to ahel-bid 🔨\r\n"
					":ahel-bid 376 " + this->nick + " :End of /MOTD command\r\n");
}

void	irc_client::exec_cmd(void)
{
	int len = this->buffer.find_first_of(" \n");
	std::string	cmd = this->buffer.substr(0, len);
	std::string	args = this->buffer.substr(len + 1, this->buffer.size());
	std::cout << len << "|" << cmd << "|" << args << "|" << std::endl;
	if (args.size() > 0 && args[args.size() - 1] == '\n')
		args.erase(args.find_last_of("\n"), args.size());
	if (cmd == "PASS")
		this->PASS(args);
	else if (cmd == "NICK")
		this->NICK(args);
	else if (cmd == "USER")
		this->USER(args);
}

