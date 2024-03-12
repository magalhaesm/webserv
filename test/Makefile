TESTS    = parser
TEST_LIB = doctest.h

SRC_DIR  = ../src
INC_DIR  = ../include
OBJ_DIR  = ../obj
DIRS     = http http/parser events config helpers

SRC_DIRS = $(addprefix $(SRC_DIR)/, $(DIRS))
INC_DIRS = $(addprefix $(INC_DIR)/, $(DIRS))
OBJS     = $(shell find $(OBJ_DIR) -name '*.o')
OBJS     := $(filter-out $(OBJ_DIR)/main.o, $(OBJS))

CXXFLAGS = $(addprefix -I ,$(INC_DIRS))

# NOTE: To include successful assertions in the output
# $(CXX) $(CXXFLAGS) $^ -o $@ && ./$@ -s -ni
define run
 	$(CXX) $(CXXFLAGS) $^ -o $@ && ./$@ -ni
endef

# ===================================================================================

all: $(TEST_LIB) $(TESTS) clean

.PHONY: parser
parser: $(OBJS) HTTPParserTests.cpp
	@$(call run, "$^")

$(TEST_LIB):
	@wget -q https://raw.githubusercontent.com/doctest/doctest/master/doctest/doctest.h

clean:
	@$(RM) -r $(TESTS)
	@$(RM) *.txt

.PHONY: all clean
