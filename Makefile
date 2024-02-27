NAME := webserv

RESET := \033[0m
CYAN  := \033[1;36m
CHECK := \342\234\224
LOG   := printf "[$(CYAN)$(CHECK)$(RESET)] %s\n"

OBJ_DIR  := obj
DIRS     := http http/parser events config handlers helpers controllers
SRC_DIRS := $(addprefix src/, $(DIRS))
SRC_DIRS += src
INC_DIRS := $(addprefix include/, $(DIRS))
INC_DIRS += include

vpath %.hpp $(INC_DIRS)
vpath %.cpp $(SRC_DIRS)

HEADERS := Server.hpp EventListener.hpp Connection.hpp
HEADERS += HTTPRequest.hpp HTTPResponse.hpp Message.hpp
HEADERS += HTMLController.hpp CGIController.hpp strings.hpp
HEADERS += HTTPParser.hpp ABodyParser.hpp URLEncodedParser.hpp
HEADERS += FormDataParser.hpp Body.hpp ConfigParser.hpp ConfigSpec.hpp
HEADERS += HTTPConstants.hpp HTTPException.hpp

SOURCES := main.cpp Server.cpp EventListener.cpp Connection.cpp
SOURCES += HTTPRequest.cpp HTTPResponse.cpp Message.cpp
SOURCES += HTMLController.cpp CGIController.cpp strings.cpp
SOURCES += HTTPParser.cpp ABodyParser.cpp URLEncodedParser.cpp
SOURCES += FormDataParser.cpp Body.cpp ConfigParser.cpp ConfigSpec.cpp
SOURCES += HTTPConstants.cpp HTTPException.cpp

OBJS     := $(addprefix $(OBJ_DIR)/, $(SOURCES:.cpp=.o))
CXXFLAGS := -Wall -Werror -Wextra -std=c++98 -g $(addprefix -I ,$(INC_DIRS))

all: $(NAME)

run: $(NAME)
	@ echo "--> Running $@"
	@ ./$(NAME) server.conf

$(NAME): $(OBJS)
	@$(LOG) "Building $@"
	@$(CXX) $^ -o $@

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	@$(LOG) "Compiling $(notdir $<)"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	@$(LOG) "Creating objects directory"
	@mkdir $@

test: $(NAME)
	@make -C test --no-print-directory

leak: $(NAME)
	valgrind --leak-check=full --show-leak-kinds=all ./$(NAME) server.conf

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
