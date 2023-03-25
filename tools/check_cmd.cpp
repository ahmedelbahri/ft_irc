#include "../includes/ft_irc.hpp"

void	check_cmd(void)
{
	std::map<int, irc_client>::iterator	it;
	for (it = clients.begin(); it != clients.end(); it++)
	{
		if (it->second.buff()[it->second.buff().size() - 1] == '\n')
		{
			std::cout << "cmd = " << it->second.buff() << std::endl;
			it->second.buff().clear();
		}
	}
}
