NAME := webserv

RESET := \033[0m
CYAN  := \033[1;36m
CHECK := \342\234\224
LOG   := printf "[$(CYAN)$(CHECK)$(RESET)] %s\n"

OBJ_DIR  := obj
DIRS     := http http/parser events config handlers helpers logger
SRC_DIRS := $(addprefix src/, $(DIRS))
SRC_DIRS += src
INC_DIRS := $(addprefix include/, $(DIRS))
INC_DIRS += include

vpath %.hpp $(INC_DIRS)
vpath %.cpp $(SRC_DIRS)

HEADERS := Server.hpp EventListener.hpp Connection.hpp
HEADERS += Request.hpp Response.hpp Message.hpp HTTPConstants.hpp
HEADERS += HTTPParser.hpp ConfigParser.hpp ConfigSpec.hpp
HEADERS += Logger.hpp strings.hpp filesystem.hpp
HEADERS += ARequestHandler.hpp LocationHandler.hpp StaticContentHandler.hpp
HEADERS += DynamicContentHandler.hpp AccessControlHandler.hpp IndexHandler.hpp
HEADERS += InternalErrorException.hpp

SOURCES := main.cpp $(HEADERS:.hpp=.cpp)

OBJS     := $(addprefix $(OBJ_DIR)/, $(SOURCES:.cpp=.o))
CXXFLAGS := -Wall -Werror -Wextra -std=c++98 -O2 $(addprefix -I ,$(INC_DIRS))

run: $(NAME)
	@ echo "==> Running $(NAME)"
	@ ./$(NAME) server.conf

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
	valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes \
		./$(NAME) server.conf

clean:
	@$(RM) -r $(OBJS)
	@$(LOG) "Removing objects"
	@$(RM) -r $(OBJ_DIR)
	@$(LOG) "Removing objects directory"

fclean: clean
	@$(RM) -r $(NAME)
	@$(LOG) "Removing $(NAME)"

re: clean all

.PHONY: all clean fclean re run
