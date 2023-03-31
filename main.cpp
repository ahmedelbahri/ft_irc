#include "./includes/ft_irc.hpp"

pollfd								poll_fd[MAX_QUEUE];
socklen_t							addr_len = sizeof(struct sockaddr_in);
int									pollfd_size = 1;
irc_server							server;
std::map<std::string, irc_channel>	channels;
std::map<int, irc_client>			clients;


int main (int ac, char **av)
{
	server.set_server(ft_check(ac, av), av[2]);

	int			timeout = (5 * 60 * 1000); // in the following order: minutes, seconds, milliseconds
	int			activity;

	signal(SIGINT, sigint_handler);

	server.init_server();

	memset(&poll_fd, 0, sizeof(poll_fd));
	poll_fd[0].fd = server.fd();
	poll_fd[0].events = POLLIN;

	std::cout << "waiting on port " << server.get_port() << std::endl;
	while (true)
	{
		if ((activity = poll(poll_fd, pollfd_size, timeout)) == -1) // poll
			error("poll() failed.");
		if (activity == 0)
			error("poll() timed out. end of execution.");

		server.check_pollable_discriptors(pollfd_size);
		server.remove_closed_discriptors(pollfd_size);
		check_cmd();
		// std::cout << "++++++++++++++++++" << std::endl; 
		// for (std::map<std::string, irc_channel>::iterator it = channels.begin(); it != channels.end(); it++)
		// {
		// 	std::cout << "------------------" << std::endl;
		// 	std::cout << "name :" << it->second.get_name() << std::endl;
		// 	std::cout << it->second.get_mode() << std::endl;
		// 	for (int i = 0; i < (int)it->second.get_opp().size(); i++)
		// 		std::cout << "operator :" << it->second.get_opp()[i] << std::endl;
		// 	std::cout << it->second.get_pass() << std::endl;
		// 	std::cout << "------------------" << std::endl;
		// }
	}
	return (0);
}
