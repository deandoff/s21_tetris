OS = $(shell uname -o)
SRC_DIR := src
TETRIS_DIR := brick_game/tetris
TESTS_DIR := tests
GUI_DIR := gui/cli
OBJ_DIR := obj
BIN_DIR := bin

ifeq ($(OS), GNU/Linux)
    TEST_FLAGS = -lcheck -lm -lpthread -lrt -lsubunit
else
    TEST_FLAGS = -lcheck -lm -lpthread
endif

CC := gcc
CFLAGS := -Wall -Werror -Wextra -std=c11
LDFLAGS := -lncurses
MKDIR_P := mkdir -p
SOURCES := $(TETRIS_DIR)/tetris.c $(GUI_DIR)/tetris_interface.c
OBJECTS := $(TETRIS_DIR)/*.o $(GUI_DIR)/*.o

.PHONY: all clean install uninstall dvi dist test gcov_report

all: clean $(BIN_DIR)/tetris

install:
	@$(MKDIR_P) $(BIN_DIR)
	$(CC) $(SOURCES) -o $(BIN_DIR)/tetris $(LDFLAGS) $(CFLAGS)

uninstall:
	rm -rf $(BIN_DIR) $(OBJECTS)

$(BIN_DIR)/tetris:
	@$(MKDIR_P) $(@D)
	$(CC) $(SOURCES) -o $@ $(LDFLAGS) $(CFLAGS)
	./bin/tetris

clean:
	rm -rf $(BIN_DIR) $(OBJECTS) *.info gcov_report score.txt tetris.tar.gz

dvi:
	open doc/index.html

dist: clean
	tar -czvf tetris.tar.gz . Makefile

test:
	@$(MKDIR_P) $(BIN_DIR)
	$(CC) $(TETRIS_DIR)/tetris.c $(TESTS_DIR)/*.c -o $(BIN_DIR)/tetris $(LDFLAGS) $(CFLAGS) $(TEST_FLAGS)
	./bin/tetris

gcov_report: clean
	@$(MKDIR_P) $(BIN_DIR)
	$(CC) $(TETRIS_DIR)/tetris.c $(TESTS_DIR)/*.c -o $(BIN_DIR)/tetris $(LDFLAGS) $(CFLAGS) $(TEST_FLAGS) -fprofile-arcs -ftest-coverage
	./bin/tetris
	lcov -t "$(TETRIS_DIR)/tetris.c" -o tetris.a.info -c -d .
	lcov --extract tetris.a.info '*/tetris.c' -o filtered.info
	genhtml -o gcov_report filtered.info
	open gcov_report/index.html
	rm -rf *.gcno *.gcda

style_fix:
	clang-format -style=Google -i $(SRC) */*/*.c */*/*.h */*.c

style:
	clang-format -style=Google -n $(SRC) */*/*.c */*/*.h