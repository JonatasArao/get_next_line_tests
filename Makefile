# Compiler and flags
CC			=	cc
CFLAGS		=	-Wall -Wextra -Werror -g
LDFLAGS		=	-lrt -lm -L$(LIBDIR) -lmocks
RM			=	rm -rf

# Color variables
COLOR_RESET			=	\033[0m
COLOR_PURPLE		=	\033[1;35m
COLOR_WHITE			=	\033[0;37m
COLOR_RED			=	\033[1;31m
COLOR_LIGHT_RED		=	\033[0;31m
COLOR_YELLOW		=	\033[1;33m
COLOR_BLUE			=	\033[1;34m
COLOR_GREEN			=	\033[1;32m
COLOR_LIGHT_GREEN	=	\033[0;32m
COLOR_CYAN			=	\033[1;36m

# Directories and files
SRCDIR	=	get_next_line
TESTDIR	=	tests
MOCKDIR	=	mocks
BINDIR	=	build
LIBDIR	=	lib

# Detect the operating system
UNAME_S	= $(shell uname -s)

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
MOCKLIB		=	$(LIBDIR)/libmocks.$(SHARED_LIB_EXT)
MOCK		=	malloc \
				free
MOCKS_SRCS	=	$(addprefix $(MOCKDIR)/mock_, $(addsuffix .c, $(MOCK)))
MOCK_OBJS	=	$(MOCKS_SRCS:.c=.o)

# Include directories and headers
INC			=	-I$(SRCDIR)
TEST_INC	=	-I$(TESTDIR)
HEADER		=	$(SRCDIR)/get_next_line.h
TEST_HEADER	=	$(TESTDIR)/tests.h

# Source and object files
FILES		=	get_next_line get_next_line_utils
EXIST_FILES	=	$(foreach files,$(FILES),$(if $(wildcard $(SRCDIR)/$(files).c),$(files),))
MISS_FILES	=	$(foreach files,$(FILES),$(if $(wildcard $(SRCDIR)/$(files).c),,$(files)))
SRCS		=	$(addprefix $(SRCDIR)/, $(addsuffix .c, $(EXIST_FILES)))
OBJS		=	$(SRCS:.c=.o)
TEST_SRCS	=	$(addprefix $(TESTDIR)/test_, $(addsuffix .c, $(EXIST_FILES)))
TEST_OBJS	=	$(TEST_SRCS:.c=.o)

# Utility functions for get_next_line
GNL_UTILS				=	ft_strchr ft_substr ft_strjoin
GNL_UTILS_TESTS_SRCS	=	$(addprefix $(TESTDIR)/gnl_utils_functions/test_, $(addsuffix .c, $(GNL_UTILS)))
GNL_UTILS_TESTS_OBJS	=	$(GNL_UTILS_TESTS_SRCS:.c=.o)

define print_files
	@echo "$(COLOR_PURPLE)Available files:$(COLOR_RESET)"
	@echo "$(COLOR_WHITE)$(EXIST_FILES)$(COLOR_RESET)" | xargs -n 6 | column -t
	@if [ "$(filter $(FILES),$(MISS_FILES))" ]; then \
		echo "$(COLOR_RED)Missing files:$(COLOR_RESET)"; \
		echo "$(COLOR_LIGHT_RED)$(MISS_FILES)$(COLOR_RESET)" | xargs -n 6 | column -t; \
	fi
endef

# Phony targets
.PHONY: all single multiple run run-single run-multiple debug debug-single debug-multiple clean fclean re

# Default target: builds the library and all tests
all: $(if $(filter 1,$(words $(EXIST_FILES))),single,multiple)
	$(call print_files)

# Single function build: builds the library and a single test executable
single: $(BINDIR)/test.out
	@echo "$(COLOR_YELLOW)Test for $(EXIST_FILES) is available at: $(BINDIR)/test.out$(COLOR_RESET)"

# Build all tests
multiple: $(BINDIR)/$(SRCDIR)/test_get_next_line.out $(BINDIR)/$(SRCDIR)/test_get_next_line_utils.out
	@echo "$(COLOR_YELLOW)All tests are available at: $(BINDIR)/$(SRCDIR)/$(COLOR_RESET)"

# Run target: runs the appropriate tests based on the number of tests
run: $(if $(filter 1,$(words $(EXIST_FILES))),run-single,run-multiple)
	$(call print_files)

# Run single test target
run-single: single
	@echo "$(COLOR_BLUE)Running test: $(EXIST_FILES)$(COLOR_RESET)"
	@$(LIBRARY_PATH_VAR)=$(LIBDIR) $(BINDIR)/test.out

# Run multiple tests target
run-multiple: multiple
	@for bin in $(BINDIR)/$(SRCDIR)/*.out; do \
		bin_name=$$(basename $$bin | sed 's/^test_//' | sed 's/\.out$$//'); \
		echo "$(COLOR_BLUE)Running test: $$bin_name$(COLOR_RESET)"; \
		$(LIBRARY_PATH_VAR)=$(LIBDIR) $$bin; \
	done

# Debug target: debugs the appropriate tests based on the number of tests
debug: $(if $(filter 1,$(words $(EXIST_FILES))),debug-single,debug-multiple)
	$(call print_files)

# Debug single test target
debug-single: single
	@echo "$(COLOR_PURPLE)Debugging test: $(EXIST_FILES)$(COLOR_RESET)"
	@$(LIBRARY_PATH_VAR)=$(LIBDIR) gdb --args $(BINDIR)/test.out

# Debug multiple tests target
debug-multiple: multiple
	@for bin in $(BINDIR)/$(SRCDIR)/*.out; do \
		bin_name=$$(basename $$bin | sed 's/^test_//' | sed 's/\.out$$//'); \
		echo "$(COLOR_PURPLE)Debugging test: $$bin_name$(COLOR_RESET)"; \
		$(LIBRARY_PATH_VAR)=$(LIBDIR) gdb --args $$bin; \
	done

# Compile the shared library for mocks
$(MOCKLIB): $(MOCK_OBJS)
	@mkdir -p $(LIBDIR)
	@$(CC) $(CFLAGS) $(SHARED_LIB_FLAGS) -o $@ $^ -ldl
	@echo "$(COLOR_GREEN)Build complete: mock library$(COLOR_RESET)"

# Rule to compile .c files in $(MOCKDIR) to .o files
$(MOCKDIR)/%.o: $(MOCKDIR)/%.c
	@$(CC) $(CFLAGS) -fPIC -c -o $@ $<
	@echo "$(COLOR_LIGHT_GREEN)Compiled: $<$(COLOR_RESET)"

# Build target: compiles and links a test executable
$(BINDIR)/$(SRCDIR)/test_get_next_line_utils.out: \
	$(SRCDIR)/get_next_line_utils.o \
	$(TESTDIR)/test_get_next_line_utils.o \
	$(GNL_UTILS_TESTS_OBJS) \
	$(MOCKLIB)
	@mkdir -p $(BINDIR)/$(SRCDIR)
	@$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@
	@echo "$(COLOR_GREEN)Build complete: $@$(COLOR_RESET)"

# Build target: compiles and links a test executable
$(BINDIR)/$(SRCDIR)/test_get_next_line.out: \
	$(TESTDIR)/test_get_next_line.o \
	$(SRCDIR)/get_next_line_utils.o \
	$(OBJS) \
	$(MOCKLIB)
	@mkdir -p $(BINDIR)/$(SRCDIR)
	@$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@
	@echo "$(COLOR_GREEN)Build complete: $@$(COLOR_RESET)"

# Build target to debug a selected test: compiles and links a single test debug executable
$(BINDIR)/test.out: \
	$(if $(filter get_next_line_utils,$(EXIST_FILES)), $(GNL_UTILS_TESTS_OBJS)) \
	$(if $(filter get_next_line,$(EXIST_FILES)), $(SRCDIR)/get_next_line_utils.o) \
	$(TEST_OBJS) \
	$(OBJS) \
	$(MOCKLIB)
	@mkdir -p $(BINDIR)
	@$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@
	@echo "$(COLOR_GREEN)Build complete: test.out$(COLOR_RESET)"

# Rule to compile .c files in $(SRCDIR) to .o files
$(SRCDIR)/%.o: $(SRCDIR)/%.c $(HEADER)
	@$(COMPILE.c) $(INC) $(OUTPUT_OPTION) $<
	@echo "$(COLOR_LIGHT_GREEN)Compiled: $<$(COLOR_RESET)"

# Rule to compile .c files in $(TESTDIR) to .o files
$(TESTDIR)/%.o: $(TESTDIR)/%.c $(HEADER) $(TEST_HEADER)
	@$(COMPILE.c) $(TEST_INC) $(OUTPUT_OPTION) $<
	@echo "$(COLOR_LIGHT_GREEN)Compiled: $<$(COLOR_RESET)"

# Clean target
clean:
	@$(RM) $(OBJS) $(TEST_OBJS) $(GNL_UTILS_TESTS_OBJS) $(MOCK_OBJS)
	@echo "$(COLOR_CYAN)Clean complete$(COLOR_RESET)"

# Full clean target
fclean: clean
	@$(RM) $(BINDIR) $(LIBDIR)
	@echo "$(COLOR_BLUE)Full clean complete$(COLOR_RESET)"

# Rebuild target
re: fclean all
