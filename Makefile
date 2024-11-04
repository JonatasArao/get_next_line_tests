# Compiler and flags
CC				=	cc
CFLAGS			=	-Wall -Wextra -Werror -g
LDFLAGS			=	-lrt -lm -L$(LIBDIR) -lmocks
RM				=	rm -rf

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
all: $(if $(filter 1,$(words $(TEST))),single,multiple)
	@echo "\033[1;35mAvailable files:\033[0m"
	@echo "\033[0;37m$(EXIST_FILE)\033[0m" | xargs -n 2 | column -t
	@if [ "$(filter $(FILES),$(MISS_FILE))" ]; then \
		echo "\033[1;31mMissing files:\033[0m"; \
		echo "\033[0;31m$(MISS_FILE)\033[0m" | xargs -n 2 | column -t; \
	fi

# Single function build: builds the library and a single test executable
single: $(MOCKLIB) $(BINDIR)/test.out
	@echo "\033[1;33mTest for $(TEST) is available at: $(BINDIR)/test.out\033[0m"

# Build all tests
multiple: $(MOCKLIB) $(OUT_FILES)
	@echo "\033[1;33mAll tests are available at: $(BINDIR)/$(TARGET)/\033[0m"

# Run target: runs the appropriate tests based on the number of tests
run: $(if $(filter 1,$(words $(TEST))),run-single,run-multiple)

# Run single test target
run-single: single
	@echo "\033[1;34mRunning test: $(TEST)\033[0m"; \
	$(LIBRARY_PATH_VAR)=$(LIBDIR) $(BINDIR)/test.out; \

# Run multiple tests target
run-multiple: multiple
	@for bin in $(OUT_FILES); do \
		bin_name=$$(basename $$bin | sed 's/^test_//' | sed 's/\.out$$//'); \
		echo "\033[1;34mRunning test: $$(echo $$bin_name)\033[0m"; \
		$(LIBRARY_PATH_VAR)=$(LIBDIR) $$bin; \
	done
	@if [ "$(filter $(FILES),$(MISS_FILE))" ]; then \
		echo "\033[1;31mMissing files:\033[0m"; \
		echo "\033[0;31m$(MISS_FILE)\033[0m" | xargs -n 2 | column -t; \
	fi

# Debug target: debugs the appropriate tests based on the number of tests
debug: $(if $(filter 1,$(words $(TEST))),debug-single,debug-multiple)

# Debug single test target
debug-single: single
	@echo "\033[1;35mDebugging test: $(TEST)\033[0m"
	@$(LIBRARY_PATH_VAR)=$(LIBDIR) gdb --args $(BINDIR)/test.out

# Debug multiple tests target
debug-multiple: all
	@for bin in $(OUT_FILES); do \
		bin_name=$$(basename $$bin | sed 's/^test_//' | sed 's/\.out$$//'); \
		echo "\033[1;35mDebugging test: $$(echo $$bin_name)\033[0m"; \
		$(LIBRARY_PATH_VAR)=$(LIBDIR) gdb --args $$bin; \
	done
	@if [ "$(filter $(FILES),$(MISS_FILE))" ]; then \
		echo "\033[1;31mMissing files:\033[0m"; \
		echo "\033[0;31m$(MISS_FILE)\033[0m" | xargs -n 2 | column -t; \
	fi

# Compile the shared library for mocks
$(MOCKLIB): $(MOCK_OBJS)
	@mkdir -p $(LIBDIR)
	@$(CC) $(CFLAGS) $(SHARED_LIB_FLAGS) -o $@ $^ -ldl
	@echo "\033[1;32mBuild complete: mock library\033[0m"

# Rule to compile .c files in $(MOCKDIR) to .o files
$(MOCKDIR)/%.o: $(MOCKDIR)/%.c
	@$(CC) $(CFLAGS) -fPIC -c -o $@ $<
	@echo "\033[0;32mCompiled: $<\033[0m"

# Rule to compile .c files in $(SRCDIR) to .o files
$(SRCDIR)/%.o: $(SRCDIR)/%.c $(HEADER)
	@$(COMPILE.c) $(INC) $(OUTPUT_OPTION) $<
	@echo "\033[0;32mCompiled: $<\033[0m"

# Rule to compile .c files in $(TESTDIR) to .o files
$(TESTDIR)/%.o: $(TESTDIR)/%.c $(HEADER) $(TEST_HEADER)
	@$(COMPILE.c) $(TEST_INC) $(OUTPUT_OPTION) $<
	@echo "\033[0;32mCompiled: $<\033[0m"

# Build target: compiles and links a test executable
$(BINDIR)/$(TARGET)/test_get_next_line_utils.out: $(UTILS_OBJS) $(GNL_UTILS_OBJS) $(TESTDIR)/test_get_next_line_utils.o $(SRCDIR)/get_next_line_utils.o
	@mkdir -p $(BINDIR)/$(TARGET)
	@$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@
	@echo "\033[1;32mBuild complete: $@\033[0m"

# Build target: compiles and links a test executable
$(BINDIR)/$(TARGET)/test_get_next_line.out: $(UTILS_OBJS) $(TESTDIR)/test_get_next_line.o $(SRCDIR)/get_next_line.o
	@mkdir -p $(BINDIR)/$(TARGET)
	@$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@
	@echo "\033[1;32mBuild complete: $@\033[0m"

# Build target to debug a selected test: compiles and links a single test debug executable
$(BINDIR)/test.out: $(if $(filter get_next_line_utils,$(TEST)), $(GNL_UTILS_OBJS)) $(UTILS_OBJS) $(TEST_OBJS) $(OBJS)
	@echo $(EXIST_FILE)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@
	@echo "\033[1;32mBuild complete: test.out\033[0m"

# Clean target
clean:
	@$(RM) $(OBJS) $(TEST_OBJS) $(UTILS_OBJS) $(GNL_UTILS_OBJS) $(MOCK_OBJS)
	@echo "\033[1;36mClean complete\033[0m"

# Full clean target
fclean: clean
	@$(RM) $(BINDIR) $(LIBDIR)
	@echo "\033[1;34mFull clean complete\033[0m"

# Rebuild target
re: fclean all
