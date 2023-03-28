#include "../includes/irc_channel.hpp"

irc_channel::irc_channel()
{
	std::cout << "heere" << std::endl;
}
irc_channel::irc_channel(std::map<std::string, std::string>::iterator channel, int fd)
{
	this->name = channel->first;
	if (channel->second.empty())
		this->PRIVATE =  false;
	else
		this->PRIVATE = true, this->pass = channel->second;
	this->opp = clients[fd].get_nick();
}

bool	irc_channel::get_mode()
{
	return (this->PRIVATE);
}

std::string	irc_channel::get_opp()
{
	return (opp);
}

std::string	irc_channel::get_name()
{
	return (name);
}

std::string	irc_channel::get_pass()
{
	return (pass);
}

std::vector<int>	irc_channel::get_invites()
{
	return (this->invites);
}

std::vector<int>	irc_channel::get_members()
{
	return (this->members);
}

void	irc_channel::inform_members(std::string msg, int fd)
{
	for (std::vector<int>::iterator it = this->members.begin(); it != this->members.end(); it++)
		if(*it != fd)
			send_error(*it, msg);
}

void	irc_channel::add_member(int fd)
{
	this->members.push_back(fd);
	inform_members(":" + clients[fd].get_nick() + " JOIN " + this->name + "\n", fd);
	send_error(fd, ":" + clients[fd].get_nick() + " 332 JOIN :No topic\n");
}

irc_channel::~irc_channel()
{
	return;
}
