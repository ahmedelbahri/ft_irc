#include "./includes/ft_irc.hpp"

pollfd			poll_fd[MAX_QUEUE];
socklen_t		addr_len = sizeof(struct sockaddr_in);
int				pollfd_size = 1;

int main (int ac, char **av)
{
	irc_server	server(ft_check(ac, av), av[2]);
	int			timeout = (3 * 60 * 1000); // in the following order: minutes, seconds, milliseconds
	int			activity;

	signal(SIGINT, sigint_handler);

	server.init_server();

	memset(&poll_fd, 0, sizeof(poll_fd));
	poll_fd[0].fd = server.fd();
	poll_fd[0].events = POLLIN;

	while (true)
	{
		// std::cout << "waiting on port " << server.get_port() << std::endl;
		if ((activity = poll(poll_fd, pollfd_size, timeout)) == -1) // poll
			error("poll() failed.");
		if (activity == 0)
			error("poll() timed out. end of execution.");

		server.check_pollable_discriptors(pollfd_size);
		server.remove_closed_discriptors(pollfd_size);
	}
	return (0);
}
