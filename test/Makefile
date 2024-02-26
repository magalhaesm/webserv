TESTS     := parser
TEST_LIB  := doctest.h

SRC_DIR   := ..
OBJ_DIR   := ../obj
SRC_DIRS  := http http/parser event config helpers
SRC_DIRS  := $(addprefix $(SRC_DIR)/, $(SRC_DIRS))
OBJS      := $(shell find $(OBJ_DIR) -name '*.o')
OBJS      := $(filter-out $(OBJ_DIR)/main.o, $(OBJS))

CXXFLAGS  := $(addprefix -I,$(SRC_DIRS))

define run
 	$(CXX) $(CXXFLAGS) $^ -o $@ && ./$@ -s -ni -nv
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

.PHONY: all clean
