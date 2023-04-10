#include "../includes/irc_channel.hpp"

irc_channel::irc_channel()
{
	this->topic = "";
	return;
}
irc_channel::irc_channel(std::map<std::string, std::string>::iterator channel, int fd)
{
	this->topic = "";
	this->name = channel->first;
	this->invite_only = false;
	this->topic_settable_by_op = false;
	this->messsage_from_non_member = false;
	this->pass = channel->second;
	opp.push_back(fd);
}

bool	&irc_channel::get_mode()
{
	return (this->invite_only);
}

bool	&irc_channel::get_topic_settable_by_op()
{
	return (this->topic_settable_by_op);
}

bool	&irc_channel::get_message_from_non_member()
{
	return (this->messsage_from_non_member);
}

std::string	irc_channel::get_name()
{
	return (name);
}

std::string	&irc_channel::get_pass()
{
	return (pass);
}

std::vector<int>	&irc_channel::get_opp()
{
	return (opp);
}

std::vector<int>	&irc_channel::get_invites()
{
	return (this->invites);
}

std::vector<int>	&irc_channel::get_members()
{
	return (this->members);
}

std::string	&irc_channel::get_topic()
{
	return (this->topic);
}

void	irc_channel::inform_members(std::string msg, int fd)
{
	for (std::vector<int>::iterator it = this->members.begin(); it != this->members.end(); it++)
	{
		if(*it != fd)
			;
		send_error(*it, msg);
	}
}

std::string	irc_channel::get_members_list(void)
{
	std::string list;
	for (std::vector<int>::iterator it = this->members.begin(); it != this->members.end(); it++)
	{
		if (isElementInVector(this->opp, *it))
			list += "@" + clients[*it].get_nick() + " ";
		else
			list += clients[*it].get_nick() + " ";
	}
	return (list);
}

void	irc_channel::add_member(int fd)
{
	this->members.push_back(fd);
	if (this->members.size() != 1)
	{
		std::string msg = ":" + clients[fd].get_nick() + "!" + clients[fd].get_username() + "@" + clients[fd].get_num_addr() + " JOIN " + name + "\r\n"
		":ircserv 332 " + clients[fd].get_nick() + " " + name + " :" + (this->topic.empty() ? "No topic is set" : this->topic) + "\r\n"
		":ircserv 333 " + clients[fd].get_nick() + " " + name + " " + clients[opp[0]].get_nick() + "!" + clients[fd].get_username() + "@" + clients[fd].get_num_addr() + "\r\n"
		":ircserv 353 " + clients[fd].get_nick() + (this->pass == "" ? " = " : " * ") + name + " :" + get_members_list() + "\r\n"
		":ircserv 366 " + clients[fd].get_nick() + " " + name + " :End of /NAMES list.\r\n";
		send_error(fd, msg);
		inform_members(":" + clients[fd].get_nick() + "!" + clients[fd].get_username() + "@" + clients[fd].get_num_addr() + " JOIN " + this->name + "\r\n", fd);
	}
	else
	{
		std::string msg = ":" + clients[fd].get_nick() + "!" + clients[fd].get_username() + "@" + clients[fd].get_num_addr() + " JOIN " + name + "\r\n"
		":ircserv MODE o +o\r\n"
		":ircserv 353 " + clients[fd].get_nick() + (this->pass == "" ? " = " : " * ") + name + " :@" + clients[fd].get_nick() + "\r\n"
		":ircserv 366 " + clients[fd].get_nick() + " " + name + " :End of /NAMES list.\r\n";
		send_error(fd, msg);
	}
}

irc_channel::~irc_channel()
{
	return;
}
