#include "../../includes/ft_irc.hpp"

void	irc_client::USER(std::string args)
{
	std::vector<std::string>	vec;
	int n = 0;
	for (int i = 0;args[i]; i++)
		if (args[i] == ' ')
			n++;
	int len = this->buffer.find_first_of(" ");
	std::string	cmd = this->buffer.substr(0, len);
	if (n < 3)
		send_error(this->fd, ERR_NEEDMOREPARAMS("USER"));
	else if (!this->authenticated)
		send_error(this->fd, ERR_NICKBEFOREPASS);
	else
	{
		int len = this->buffer.find_first_of(" \n");
		std::string	name = this->buffer.substr(0, len);
		this->username = name;
		if (this->nick[0])
		{
			send(this->fd, this->msg_auth().c_str(), this->msg_auth().size(), 0);
			this->authenticated = 2;
		}
	}
}