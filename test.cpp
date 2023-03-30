#include <iostream>
#include <vector>

static void	get_arr(std::string str, std::vector<std::string> &arr)
{
	int i = 0;
	str = str.substr(str.find_first_not_of(" "), str.length());
	str = str.substr(0, str.find_last_not_of(" ") + 1);
	// if ((int)str.find(" ") >= 0)
	// 	return (std::cout << "error key should not have ' '" << std::endl, void()); 
	while (str[i])
	{
		if (str[i] == ',' && str[i + 1] == '\0')
			return (std::cout << "error key should not have ', '" << std::endl, void());
		if (str[i] == ',')
		{
			arr.push_back(str.substr(0, i));
			str = str.substr(i + 1, str.length());
			i = 0;
		}
		i++;
	}
	arr.push_back(str);
}

static void	split_args(std::string args, std::map<std::string, std::string> &new_channels)
{
	std::vector<std::string>	channels;
	std::vector<std::string>	reason;
	args = args.substr(args.find_first_not_of(" "), args.length());
	args = args.substr(0, args.find_last_not_of(" ") + 1);
	get_arr(args.substr(0, args.find_first_of(" ")), channels);
	get_arr(args.substr(args.find_first_of(" "), args.size()), reason);
	if (channels.size() != reason.size())
		return (std::cout << "error each channel should have a reason" << std::endl, void());
	std::vector<std::string>::iterator it2 = reason.begin();
	for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		std::cout <<
		// new_channels.insert(std::pair<std::string, std::string>(*it, *it2));
		if(it2 != reason.end())
			it2++;
	}
}