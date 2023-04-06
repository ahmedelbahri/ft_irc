#ifndef IRC_CHANNEL_HPP
#define IRC_CHANNEL_HPP

#include "ft_irc.hpp"

class irc_channel
{
private:
	std::string			name;
	std::string			pass;
	std::string			topic;
	std::vector<int>	opp;
	std::vector<int>	invites;
	std::vector<int>	members;
	bool				invite_only;
	bool				topic_settable_by_op;
	bool				messsage_from_non_member;
public:
						irc_channel();
						irc_channel(std::map<std::string, std::string>::iterator channel, int fd);
	bool				&get_mode();
	bool				&get_topic_settable_by_op();
	bool				&get_message_from_non_member();
	std::string			get_name();
	std::string			&get_pass();
	std::string			&get_topic();
	std::vector<int>	&get_opp();
	std::vector<int>	&get_invites();
	std::vector<int>	&get_members();
	void				add_member(int fd);
	void				inform_members(std::string msg, int fd);
						~irc_channel();
};

#endif
