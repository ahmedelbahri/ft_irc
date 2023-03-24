#include "../includes/ft_irc.hpp"

void	sigint_handler(int sig)
{
	if (sig == SIGINT)
	{
		std::cout << "\nSIGINT received. Exiting..." << std::endl;
		for (int i = 0; i < pollfd_size; i++)
			close(poll_fd[i].fd);
		exit(0);
	}
}