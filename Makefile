CPP = main.cpp ./sources/irc_server.cpp ./tools/error.cpp ./tools/ft_check.cpp ./tools/sigint_handler.cpp
HPP = ./includes/irc_server.hpp ./includes/ft_irc.hpp
OBJ = $(CPP:.cpp=.o)
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address
CXX = c++
NAME = ircserv

all: $(NAME)

%.o: %.cpp $(HPP)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all
