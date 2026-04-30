# NOTE: Cheatsheet: https://devhints.io/makefile

CC = gcc
# C_FLAGS=--std=<here> -MMD -Wall -Wextra -pedantic -Wshadow
C_FLAGS=-MMD -Wall -Wextra -Wshadow

SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, build/%.o, $(SRC))
DEP = $(OBJ:%.o=%.d)
EXE = build/main

all: $(EXE)

debug: C_FLAGS += -g
debug: all

$(EXE): $(OBJ)
	@echo "  LD      $@"
	@$(CC) -o $@ $^

build/%.o: src/%.c
	@mkdir -p build
	@echo "  CC      $@"
	@$(CC) $(C_FLAGS) -c -o $@ $<

run: $(EXE)
	@echo "  RUN     ${EXE}"
	@echo
	@echo "=== RESULT ==="
	@./$(EXE)
	@echo
	@echo "=> Returned $$?."

clean:
	@echo "  CLEAN   build/"
	@rm -rf build

-include $(DEP)

.PHONY: all debug run clean
