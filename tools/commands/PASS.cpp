#include "../../includes/irc_client.hpp"

void	irc_client::PASS(std::string args)
{
	if (args[args.size() - 1] == '\n')
		args.erase(args.find_last_of("\n"), args.size());
	std::cout << this->fd << std::endl;
	std::cout << "|" << args << "|" << std::endl;
	if (args.empty())
		send_error(this->fd, ERR_NEEDMOREPARAMS("PASS"));
	else if (this->authenticated)
		send_error(this->fd, (ERR_ALREADYREGISTERED));
	else if (!this->authenticated && server.get_pass() != args)
		send_error(this->fd, (ERR_PASSWDMISMATCH));
	else if (!this->authenticated && server.get_pass() == args)
		this->authenticated = 1;
}