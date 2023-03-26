#include "../../includes/ft_irc.hpp"

void	irc_client:: USER(std::string args)
{
	int n = 0;
	for (int i = 0;args[i]; i++)
		if (args[i] == ' ' && !strchr(" \t\v\f\r", args[i + 1]))
			n++;
	int len = this->buffer.find_first_of(" ");
	std::string	cmd = this->buffer.substr(0, len);
	if (n < 3)
		send_error(this->fd, ERR_NEEDMOREPARAMS("USER"));
	else if (!this->authenticated)
		send_error(this->fd, ERR_NICKBEFOREPASS);
	else if (this->authenticated == 3 || this->authenticated == 2)
		send_error(this->fd, ERR_ALREADYREGISTERED);
	else
	{
		int len = this->buffer.find_first_of(" \n");
		std::string	name = this->buffer.substr(0, len);
		this->username = name;
		this->authenticated = 3;
		if (!this->nick.empty())
		{
			send(this->fd, this->msg_auth().c_str(), this->msg_auth().size(), 0);
			this->authenticated = 2;
		}
	}
}