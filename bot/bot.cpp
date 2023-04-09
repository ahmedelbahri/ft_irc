#include"../includes/ft_irc.hpp"

bool check_number(std::string str) {
	for (size_t i = 0; i < str.length(); i++)
		if (isdigit(str[i]) == false)
			return false;
	return true;
}

void	convert(std::string amount)
{

}

void	irc_client::BOT(std::string args)
{
	if (args.empty())
		send_error(this->fd, ":" + this->nick + "give me the amount in MAD to convert it into US Dollar\n");
	else
	{
		if (check_number(args))
			convert(args);
		else
		send_error(this->fd, ":" + this->nick + "give me the amount with digit character only.\n");
	}
}