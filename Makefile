NAME := webserv

RESET := \033[0m
CYAN  := \033[1;36m
CHECK := \342\234\224
LOG   := printf "[$(CYAN)$(CHECK)$(RESET)] %s\n"

OBJ_DIR := obj
DIRS    := config server controllers models views

vpath %.hpp $(DIRS)
vpath %.cpp $(DIRS)

HEADERS := ConfigParser.hpp ConfigSpec.hpp
HEADERS += Server.hpp EventListener.hpp Connection.hpp
HEADERS += Dispatcher.hpp HTTPRequest.hpp HTTPResponse.hpp
HEADERS += CGIScriptController.hpp

SOURCES := ConfigParser.cpp ConfigSpec.cpp
SOURCES += main.cpp Server.cpp EventListener.cpp Connection.cpp
SOURCES += Dispatcher.cpp HTTPRequest.cpp HTTPResponse.cpp
SOURCES += CGIScriptController.cpp

OBJS     := $(addprefix $(OBJ_DIR)/, $(SOURCES:.cpp=.o))
CXXFLAGS := -Wall -Werror -Wextra  $(addprefix -I,$(DIRS))

all: $(NAME)

$(NAME): $(OBJS)
	@$(LOG) "Building $@"
	@$(CXX) $^ -o $@

$(OBJ_DIR)/%.o: %.cpp $(HEADERS) | $(OBJ_DIR)
	@$(LOG) "Compiling $(notdir $<)"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	@$(LOG) "Creating objects directory"
	@mkdir $@

leak: $(NAME)
	valgrind --leak-check=full --show-leak-kinds=all ./$(NAME)

clean:
	@$(RM) -r $(OBJS)
	@$(LOG) "Removing objects"
	@$(RM) -r $(OBJ_DIR)
	@$(LOG) "Removing objects directory"

fclean: clean
	@$(RM) -r $(NAME)
	@$(LOG) "Removing $(NAME)"

re: clean all

.PHONY: all clean fclean re
