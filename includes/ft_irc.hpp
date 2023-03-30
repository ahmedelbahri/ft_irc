#ifndef FT_IRC_HPP
#define FT_IRC_HPP

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <cstdlib>
#include <signal.h>
#include <vector>
#include <map>
#include <utility>

#include "irc_server.hpp"
#include "irc_client.hpp"
#include "irc_channel.hpp"

/* PASS Errors */
#define ERR_NEEDMOREPARAMS(command)	"461 * "command": Not enough parameters\n"
#define ERR_ALREADYREGISTERED		"462 * ::You may not reregister\n"
#define ERR_PASSWDMISMATCH			"464 * ERR_PASSWDMISMATCH:Password incorrect\n"

/* NICK Errors */
#define ERR_NONICKNAMEGIVEN			"431 * ERR_NONICKNAMEGIVEN:No nickname given\n"
#define ERR_NICKBEFOREPASS			"ERR_NICKBEFOREPASS:User not authenticated\n"
#define ERR_NICKNAMEINUSE(nick)		"433 * "nick" :Nickname is already in use\n"


class										irc_client;
class										irc_server;
class										irc_channel;
extern pollfd								poll_fd[MAX_QUEUE];
extern socklen_t							addr_len;
extern int									pollfd_size;
extern irc_server							server;
extern std::map<std::string, irc_channel>	channels;
extern std::map<int, irc_client>			clients;

void	send_error(int fd, std::string message);
void	error(std::string str, int code = 0);
int		ft_check(int ac, char **av);
void	sigint_handler(int sig);
void	check_cmd(void);
bool	isElementInVector(const std::vector<int>& vec, int elem);
#endif
