#include "../includes/ft_irc.hpp"

void	error(std::string str, int code)
{
	std::cerr << "Error: " << str << std::endl;
	if (code != 0)
		std::cerr << " with error number " << code << std::endl;
	for (int i = 0; i < pollfd_size; i++)
		close(poll_fd[i].fd);
	exit(1);
}