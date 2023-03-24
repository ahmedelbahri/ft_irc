#ifndef FT_IRC_HPP
#define FT_IRC_HPP

#include "irc_server.hpp"

void	error(std::string str, int code = 0);
int		ft_check(int ac, char **av);
void	sigint_handler(int sig);

#endif
