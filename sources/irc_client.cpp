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
	return (":ircserv 001 " + this->nick + " : welcome to ircserv\r\n"
			":ircserv 002 " + this->nick + " :Your host is ircserv, running version 1.0\r\n"
			":ircserv 003 " + this->nick + " :This server was created 22/03/2023\r\n"
			":ircserv 004 " + this->nick + " ircserv 1.0 - -\r\n"
			":ircserv 372 " + this->nick + " welcome to ircserv\r\n"
			":ircserv 376 " + this->nick + " :End of /MOTD command\r\n");
}

struct sockaddr_in	&irc_client::get_addr(void)
{
	return (this->addr);
}

void	irc_client::set_addr(void)
{
	this->numeric_address = inet_ntoa(this->addr.sin_addr);
}

std::string	irc_client::get_num_addr(void)
{
	return (this->numeric_address);
}

std::string	irc_client::get_username(void)
{
	return (this->username);
}

void	irc_client::exec_cmd(void)
{
	int len = this->buffer.find_first_of(" ");
	std::string	cmd = this->buffer.substr(0, len);
	std::string	args = len > 0 ? this->buffer.substr(len + 1, this->buffer.length()) : "";
	args = args.substr((int)args.find_first_not_of(ISSPACE) >= 0 ? (int)args.find_first_not_of(ISSPACE) : 0);
	args = args.substr(0, args.find_last_not_of(ISSPACE) + 1);
	if (args[0] == ':')
		args.erase(0, 1);
	for (int i = args.length() - 1; i >= 0; i--)
	{
		if (args[i] == '\n' || (int)args[i] == 13)
			args.pop_back();
		else
			break;
	}
	std::cout << "cmd: |" << cmd << "| args: |" << args << "|" << std::endl;
	if (cmd == "PASS")
		this->PASS(args);
	else if (cmd == "NICK")
		this->NICK(args);
	else if (cmd == "USER")
		this->USER(args);
	else if (cmd == "JOIN" && this->authenticated == 2)
		this->JOIN(args);
	else if (cmd == "PART" && this->authenticated == 2)
		this->PART(args);
	else if (cmd == "INVITE" && this->authenticated == 2)
		this->INVITE(args);
	else if (cmd == "MODE" && this->authenticated == 2)
		this->MODE(args);
	else if (cmd == "KICK" && this->authenticated == 2)
		this->KICK(args);
	else if (cmd == "TOPIC" && this->authenticated == 2)
		this->TOPIC(args);
	else if (cmd == "PRIVMSG" && this->authenticated == 2)
		this->PRIVMSG(args);
	else if (cmd == "NOTICE" && this->authenticated == 2)
		this->NOTICE(args);
}

