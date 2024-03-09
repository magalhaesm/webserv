NAME := webserv

RESET := \033[0m
CYAN  := \033[1;36m
CHECK := \342\234\224
LOG   := printf "[$(CYAN)$(CHECK)$(RESET)] %s\n"

OBJ_DIR  := obj
DIRS     := http http/parser events config handlers helpers controllers
DIRS     += logger
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
HEADERS += DynamicContentHandler.hpp AccessControlHandler.hpp
HEADERS += InternalErrorException.hpp

SOURCES := main.cpp Server.cpp EventListener.cpp Connection.cpp
SOURCES += Request.cpp Response.cpp Message.cpp HTTPConstants.cpp
SOURCES += HTTPParser.cpp ConfigParser.cpp ConfigSpec.cpp
SOURCES += Logger.cpp strings.cpp filesystem.cpp
SOURCES += ARequestHandler.cpp LocationHandler.cpp StaticContentHandler.cpp
SOURCES += DynamicContentHandler.cpp AccessControlHandler.cpp
SOURCES += InternalErrorException.cpp

OBJS     := $(addprefix $(OBJ_DIR)/, $(SOURCES:.cpp=.o))
CXXFLAGS := -Wall -Werror -Wextra -std=c++98 -g $(addprefix -I ,$(INC_DIRS))

all: $(NAME)

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

test: $(NAME)
	@make -C test --no-print-directory

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

.PHONY: all clean fclean re test
