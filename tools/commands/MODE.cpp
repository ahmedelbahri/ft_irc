#include "../../includes/ft_irc.hpp"

void	irc_client::MODE(std::string args)
{
	args.pop_back();
	std::cout << "MODE\n";
}