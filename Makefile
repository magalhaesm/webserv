NAME := webserv

RESET := \033[0m
CYAN  := \033[1;36m
CHECK := \342\234\224
LOG   := printf "[$(CYAN)$(CHECK)$(RESET)] %s\n"

OBJ_DIR := obj
DIRS    := server controllers services views config
DIRS    += services/http services/events helpers

vpath %.hpp $(DIRS)
vpath %.cpp $(DIRS)

HEADERS := Server.hpp EventListener.hpp Connection.hpp
HEADERS += HTTPRequest.hpp HTTPResponse.hpp Message.hpp BodyParser.hpp
HEADERS += HTMLController.hpp CGIController.hpp HTTPParser.hpp strings.hpp
HEADERS += definitions.hpp

SOURCES := main.cpp Server.cpp EventListener.cpp Connection.cpp
SOURCES += HTTPRequest.cpp HTTPResponse.cpp Message.cpp BodyParser.cpp
SOURCES += HTMLController.cpp CGIController.cpp HTTPParser.cpp strings.cpp

OBJS     := $(addprefix $(OBJ_DIR)/, $(SOURCES:.cpp=.o))
CXXFLAGS := -Wall -Werror -Wextra -std=c++98 -O2 $(addprefix -I,$(DIRS))

all: $(NAME)

run: $(NAME)
	@ echo "--> Running $@"
	@ ./$(NAME)

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

.PHONY: all clean fclean re tests
