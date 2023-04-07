#include "../../includes/irc_client.hpp"

void	irc_client::PASS(std::string args)
{
	if (args.empty())
		send_error(this->fd, ERR_NEEDMOREPARAMS("PASS"));
	else if (this->authenticated)
		send_error(this->fd, (ERR_ALREADYREGISTERED));
	else if (!this->authenticated && server.get_pass() != args)
		send_error(this->fd, (ERR_PASSWDMISMATCH));
	else if (!this->authenticated && server.get_pass() == args)
		this->authenticated = 1;
}