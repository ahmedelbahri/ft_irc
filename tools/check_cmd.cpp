#include "../includes/ft_irc.hpp"

void	check_cmd(void)
{
	std::map<int, irc_client>::iterator	it;
	for (it = clients.begin(); it != clients.end(); it++)
	{
		if (it->second.buff().size() > 0 && it->second.buff()[it->second.buff().size() - 1] == '\n')
		{
			std::string	remaining = it->second.buff();
			while (remaining.empty() == false)
			{
				it->second.buff() = remaining.substr(0, remaining.find_first_of("\n"));
				remaining = remaining.substr(remaining.find_first_of("\n") + 1);
				it->second.exec_cmd();
				it->second.buff().clear();
			}
		}
	}
}
