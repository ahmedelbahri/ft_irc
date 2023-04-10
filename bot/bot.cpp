#include"../includes/ft_irc.hpp"
#include <sstream>
#include<cstring>

bool	check_number(std::string value)
{
	bool	decimal = true;
	if (value.length() == 0)
			return false;
	if (value[0] == '-')
			return false;
	if (value[0] == '.' || value[value.length() - 1] == '.')
			return false;
	for (size_t i = 0; i < value.length(); i++)
	{
		if (value[i] == '.')
		{
			if (decimal)
				decimal = false;
			else
			return false;
		}
		else if (!isdigit(value[i]))
			return false;
	}
	if (atol(value.c_str()) > 2147483647)
			return false;
	return (true);
}

double	convert_usd(double amount, std::string towards)
{
	if (towards == "MAD")
		return (amount * 10.11);
	else if (towards == "EUR")
		return (amount * 0.92);
	else if (towards == "CAD")
		return (amount * 1.35);
	return (amount);
}

double	convert_mad(double amount, std::string towards)
{
	if (towards == "USD")
		return (amount * 0.099);
	else if (towards == "EUR")
		return (amount * 0.091);
	else if (towards == "CAD")
		return (amount * 0.13);
	return (amount);
}

double	convert_eur(double amount, std::string towards)
{
	if (towards == "USD")
		return (amount * 1.09);
	else if (towards == "MAD")
		return (amount * 11.03);
	else if (towards == "CAD")
		return (amount * 1.47);
	return (amount);
}

double	convert_cad(double amount, std::string towards)
{
	if (towards == "USD")
		return (amount * 0.74);
	else if (towards == "MAD")
		return (amount * 7.48);
	else if (towards == "EUR")
		return (amount * 0.68);
	return (amount);
}

double	convert(std::string amount, std::string	of, std::string towards)
{
	std::stringstream ss; 
	double num;

	ss << amount;
	ss >> num;
	if (of == "USD")
		num = convert_usd(num, towards);
	else if (of == "MAD")
		num = convert_mad(num, towards);
	else if (of == "EUR")
		num = convert_eur(num, towards);
	else if (of == "CAD")
		num = convert_cad(num, towards);
	return(num);
}



void	irc_client::BOT(std::string args)
{
	if (args.empty())
		send_error(this->fd, ":" + this->nick + " give me {AMOUNT OF TOWARDS}\n--currency available in OF and TOWARDS: \n--USD\n--MAD\n--EUR\n--CAD\n");
	else
	{
		int	empty = 0;
		std::string	amount = args.substr(0, args.find_first_of(ISSPACE));
		args = args.substr((int)args.find_first_of(ISSPACE) > 0 ? args.find_first_of(ISSPACE) : 0);
		args = args.substr((int)args.find_first_not_of(ISSPACE) > 0 ? args.find_first_not_of(ISSPACE) : empty++);
		std::string	of = args.substr(0, (int)args.find_first_of(ISSPACE) > 0 ? args.find_first_of(ISSPACE) : args.size());
		args = args.substr((int)args.find_first_of(ISSPACE) > 0 ? args.find_first_of(ISSPACE) : 0);
		args = args.substr((int)args.find_first_not_of(ISSPACE) > 0 ? args.find_first_not_of(ISSPACE) : empty++);
		std::string	towards = args.substr(0, (int)args.find_first_of(ISSPACE) > 0 ? args.find_first_of(ISSPACE) : args.size());
		if (empty == 0)
		{
			if (check_number(amount))
			{
				std::vector<std::string>	currency;
				currency.push_back("USD");
				currency.push_back("MAD");
				currency.push_back("EUR");
				currency.push_back("CAD");
				if ((std::find(currency.begin(), currency.end(), of) == currency.end()) || (std::find(currency.begin(), currency.end(), towards) == currency.end()))
					send_error(this->fd, ":" + this->nick + " --currency available in OF and TOWARDS \n--USD\n--MAD\n--EUR\n--CAD\n");
				else
				{
					std::ostringstream s;
					s << convert(amount, of, towards);
					send_error(this->fd, ":" + this->nick + " " + amount +" in " + of + " = " + s.str() + " in "+ towards +"\n");
				}
			}
			else
				send_error(this->fd, ":" + this->nick + " give me the amount with digit character only.\n");
		}
		else
			send_error(this->fd, ":" + this->nick + " give me {AMOUNT OF TOWARDS} 3 params\n");
	}
}