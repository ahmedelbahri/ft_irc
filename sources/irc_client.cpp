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

// std::string	irc_client::chan_verif()
// {
// 	return (":Rαɠɳαɾöƙ 001 " + server.clients[index]->nick + " : welcome to the internet relay chat\r\n"
// 			":Rαɠɳαɾöƙ 002 " + server.clients[index]->nick + " :Your host is Rαɠɳαɾöƙ, running version 1.0\r\n"
// 			":Rαɠɳαɾöƙ 003 " + server.clients[index]->nick + " :This server was created 28/12/2022\r\n"
// 			":Rαɠɳαɾöƙ 004 " + server.clients[index]->nick + " Rαɠɳαɾöƙ 1.0 - -\r\n"
// 			":Rαɠɳαɾöƙ 372 " + server.clients[index]->nick + " 🔨 𝔚𝔢𝔩𝔠𝔬𝔪𝔢 𝔗𝔬 Rαɠɳαɾöƙ 🔨\r\n"
// 			":Rαɠɳαɾöƙ 376 " + server.clients[index]->nick + " :End of /MOTD command\r\n");
// }

void	irc_client::exec_cmd(void)
{
	int len = this->buffer.find_first_of(" ");
	// this->buffer.erase(remove(this->buffer.begin(), this->buffer.end(), '\r'), this->buffer.end()); // clear args form \r
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
	else if (cmd == "JOIN") // authenticate
		this->JOIN(args);
	else if (cmd == "PART") // authenticate
		this->PART(args);
	else if (cmd == "INVITE") // authenticate
		this->INVITE(args);
	else if (cmd == "MODE") // authenticate
		this->MODE(args);
	else if (cmd == "KICK") // authenticate
		this->KICK(args);
	else if (cmd == "TOPIC") // authenticate
		this->TOPIC(args);
	else if (cmd == "PRIVMSG") // authenticate
		this->PRIVMSG(args);
}

