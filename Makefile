# Compiler and flags
CC				=	cc
CFLAGS			=	-Wall -Wextra -Werror -g
LDFLAGS			=	-lrt -lm -L$(LIBDIR) -lmocks
RM				=	rm -rf

# Color variables
COLOR_RESET		=	\033[0m
COLOR_PURPLE	=	\033[1;35m
COLOR_WHITE		=	\033[0;37m
COLOR_RED		=	\033[1;31m
COLOR_LIGHT_RED	=	\033[0;31m
COLOR_YELLOW	=	\033[1;33m
COLOR_BLUE		=	\033[1;34m
COLOR_GREEN		=	\033[1;32m
COLOR_LIGHT_GREEN = \033[0;32m
COLOR_CYAN		=	\033[1;36m

# Directories and files
TARGET			=	get_next_line
SRCDIR			=	$(TARGET)
INCDIR			=	$(TARGET)
TESTDIR			=	tests
TESTINCDIR		=	$(TESTDIR)
INCDIR			=	$(TARGET)
INCTESTDIR		=	$(TESTDIR)
MOCKDIR			=	mocks
BINDIR			=	build
LIBDIR			=	lib

# Detect the operating system
UNAME_S			= $(shell uname -s)

# Define the shared library extension and linking flags
ifeq ($(UNAME_S), Darwin)
	SHARED_LIB_EXT = dylib
	SHARED_LIB_FLAGS = -dynamiclib
	LIBRARY_PATH_VAR = DYLD_LIBRARY_PATH
else ifeq ($(UNAME_S), Linux)
	SHARED_LIB_EXT = so
	SHARED_LIB_FLAGS = -shared
	LIBRARY_PATH_VAR = LD_LIBRARY_PATH
else ifeq ($(OS), Windows_NT)
	SHARED_LIB_EXT = dll
	SHARED_LIB_FLAGS = -shared
	LIBRARY_PATH_VAR = PATH
else
	$(error Unsupported operating system)
endif

# Mock library and sources
MOCKLIB			=	$(LIBDIR)/libmocks.$(SHARED_LIB_EXT)
MOCK			=	malloc \
					free
MOCKS_SRCS		=	$(addsuffix .c, $(addprefix $(MOCKDIR)/mock_, $(MOCK)))
MOCK_OBJS		=	$(MOCKS_SRCS:.c=.o)

# Functions and sources
FILES			=	get_next_line \
					get_next_line_utils
GNL_UTILS		=	ft_strlen
EXIST_FILE		=	$(foreach file,$(FILES),$(if $(wildcard $(SRCDIR)/$(file).c),$(file),))
MISS_FILE		=	$(foreach file,$(FILES),$(if $(wildcard $(SRCDIR)/$(file).c),,$(file)))
INC				=	$(addprefix -I, $(INCDIR))
TEST_INC		=	$(addprefix -I, $(TESTINCDIR))
HEADER			=	$(INCDIR)/get_next_line.h
TEST_HEADER		=	$(TESTINCDIR)/tests.h
SRCS			=	$(addsuffix .c, $(addprefix $(SRCDIR)/, $(EXIST_FILE)))
OBJS			=	$(SRCS:.c=.o)
TEST			=	$(EXIST_FILE)
TESTS_SRCS		=	$(addsuffix .c, $(addprefix $(TESTDIR)/test_, $(TEST)))
TEST_OBJS		=	$(TESTS_SRCS:.c=.o)
UTILS_SRCS		=	$(TESTDIR)/tests_utils.c
UTILS_OBJS		=	$(TESTDIR)/tests_utils.o
GNL_UTILS_SRCS	=	$(addsuffix .c, $(addprefix $(TESTDIR)/test_, $(GNL_UTILS)))
GNL_UTILS_OBJS	=	$(GNL_UTILS_SRCS:.c=.o)
OUT_FILES		=	$(addsuffix .out, $(addprefix $(BINDIR)/$(TARGET)/test_, $(EXIST_FILE)))

# Phony targets
.PHONY: all single multiple run run-single run-multiple debug debug-single debug-multiple clean fclean re

# Default target: builds the library and all tests
all: $(if $(filter 1,$(words $(FILES))),single,multiple)
	@echo "$(COLOR_PURPLE)Available files:$(COLOR_RESET)"
	@echo "$(COLOR_WHITE)$(EXIST_FILE)$(COLOR_RESET)" | xargs -n 2 | column -t
	@if [ "$(filter $(FILES),$(MISS_FILE))" ]; then \
		echo "$(COLOR_RED)Missing files:$(COLOR_RESET)"; \
		echo "$(COLOR_LIGHT_RED)$(MISS_FILE)$(COLOR_RESET)" | xargs -n 2 | column -t; \
	fi

# Single function build: builds the library and a single test executable
single: $(MOCKLIB) $(BINDIR)/test.out
	@echo "$(COLOR_YELLOW)Test for $(FILES) is available at: $(BINDIR)/test.out$(COLOR_RESET)"

# Build all tests
multiple: $(MOCKLIB) $(OUT_FILES)
	@echo "$(COLOR_YELLOW)All tests are available at: $(BINDIR)/$(TARGET)/$(COLOR_RESET)"

# Run target: runs the appropriate tests based on the number of tests
run: $(if $(filter 1,$(words $(FILES))),run-single,run-multiple)

# Run single test target
run-single: single
	@echo "$(COLOR_BLUE)Running test: $(FILES)$(COLOR_RESET)"; \
	$(LIBRARY_PATH_VAR)=$(LIBDIR) $(BINDIR)/test.out; \

# Run multiple tests target
run-multiple: multiple
	@for bin in $(OUT_FILES); do \
		bin_name=$$(basename $$bin | sed 's/^test_//' | sed 's/\.out$$//'); \
		echo "$(COLOR_BLUE)Running test: $$(echo $$bin_name)$(COLOR_RESET)"; \
		$(LIBRARY_PATH_VAR)=$(LIBDIR) $$bin; \
	done
	@if [ "$(filter $(FILES),$(MISS_FILE))" ]; then \
		echo "$(COLOR_RED)Missing files:$(COLOR_RESET)"; \
		echo "$(COLOR_LIGHT_RED)$(MISS_FILE)$(COLOR_RESET)" | xargs -n 2 | column -t; \
	fi

# Debug target: debugs the appropriate tests based on the number of tests
debug: $(if $(filter 1,$(words $(FILES))),debug-single,debug-multiple)

# Debug single test target
debug-single: single
	@echo "$(COLOR_PURPLE)Debugging test: $(FILES)$(COLOR_RESET)"
	@$(LIBRARY_PATH_VAR)=$(LIBDIR) gdb --args $(BINDIR)/test.out

# Debug multiple tests target
debug-multiple: all
	@for bin in $(OUT_FILES); do \
		bin_name=$$(basename $$bin | sed 's/^test_//' | sed 's/\.out$$//'); \
		echo "$(COLOR_PURPLE)Debugging test: $$(echo $$bin_name)$(COLOR_RESET)"; \
		$(LIBRARY_PATH_VAR)=$(LIBDIR) gdb --args $$bin; \
	done
	@if [ "$(filter $(FILES),$(MISS_FILE))" ]; then \
		echo "$(COLOR_RED)Missing files:$(COLOR_RESET)"; \
		echo "$(COLOR_LIGHT_RED)$(MISS_FILE)$(COLOR_RESET)" | xargs -n 2 | column -t; \
	fi

# Compile the shared library for mocks
$(MOCKLIB): $(MOCK_OBJS)
	@mkdir -p $(LIBDIR)
	@$(CC) $(CFLAGS) $(SHARED_LIB_FLAGS) -o $@ $^ -ldl
	@echo "$(COLOR_GREEN)Build complete: mock library$(COLOR_RESET)"

# Rule to compile .c files in $(MOCKDIR) to .o files
$(MOCKDIR)/%.o: $(MOCKDIR)/%.c
	@$(CC) $(CFLAGS) -fPIC -c -o $@ $<
	@echo "$(COLOR_LIGHT_GREEN)Compiled: $<$(COLOR_RESET)"

# Rule to compile .c files in $(SRCDIR) to .o files
$(SRCDIR)/%.o: $(SRCDIR)/%.c $(HEADER)
	@$(COMPILE.c) $(INC) $(OUTPUT_OPTION) $<
	@echo "$(COLOR_LIGHT_GREEN)Compiled: $<$(COLOR_RESET)"

# Rule to compile .c files in $(TESTDIR) to .o files
$(TESTDIR)/%.o: $(TESTDIR)/%.c $(HEADER) $(TEST_HEADER)
	@$(COMPILE.c) $(TEST_INC) $(OUTPUT_OPTION) $<
	@echo "$(COLOR_LIGHT_GREEN)Compiled: $<$(COLOR_RESET)"

# Build target: compiles and links a test executable
$(BINDIR)/$(TARGET)/test_get_next_line_utils.out: $(UTILS_OBJS) $(GNL_UTILS_OBJS) $(TESTDIR)/test_get_next_line_utils.o $(SRCDIR)/get_next_line_utils.o
	@mkdir -p $(BINDIR)/$(TARGET)
	@$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@
	@echo "$(COLOR_GREEN)Build complete: $@$(COLOR_RESET)"

# Build target: compiles and links a test executable
$(BINDIR)/$(TARGET)/test_get_next_line.out: $(UTILS_OBJS) $(TESTDIR)/test_get_next_line.o $(SRCDIR)/get_next_line.o
	@mkdir -p $(BINDIR)/$(TARGET)
	@$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@
	@echo "$(COLOR_GREEN)Build complete: $@$(COLOR_RESET)"

# Build target to debug a selected test: compiles and links a single test debug executable
$(BINDIR)/test.out: $(if $(filter get_next_line_utils,$(FILES)), $(GNL_UTILS_OBJS)) $(UTILS_OBJS) $(TEST_OBJS) $(OBJS)
	@echo $(EXIST_FILE)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@
	@echo "$(COLOR_GREEN)Build complete: test.out$(COLOR_RESET)"

# Clean target
clean:
	@$(RM) $(OBJS) $(TEST_OBJS) $(UTILS_OBJS) $(GNL_UTILS_OBJS) $(MOCK_OBJS)
	@echo "$(COLOR_CYAN)Clean complete$(COLOR_RESET)"

# Full clean target
fclean: clean
	@$(RM) $(BINDIR) $(LIBDIR)
	@echo "$(COLOR_BLUE)Full clean complete$(COLOR_RESET)"

# Rebuild target
re: fclean all
