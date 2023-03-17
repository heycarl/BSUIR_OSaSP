CC=gcc
CFLAGS=-W -Wall -Wno-unused-parameter -Wno-unused-variable -std=c11 -pedantic
LDFLAGS=-I".."

SRC=./main.c ./func.h ./func.c
SRC_CHILD=./child/main.c
EXE=parent
EXE_CHILD=child
BUILD_DIR=./build

ENV_FILE=./envs


run: build exec clean

exec:
	@echo "Running $(EXE)..."
	CHILD_PATH=./$(BUILD_DIR)/$(EXE_CHILD) ./$(BUILD_DIR)/$(EXE) $(ENV_FILE);

build:
	mkdir build
	$(CC) $(LDFLAGS) $(SRC) -o $(BUILD_DIR)/$(EXE)
	$(CC) $(LDFLAGS) $(SRC_CHILD) -o $(BUILD_DIR)/$(EXE_CHILD)
clean:
	rm -r ./build