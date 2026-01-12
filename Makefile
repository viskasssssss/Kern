# Compiler
CXX = g++
CC  = gcc

# Source and directories
SRC_DIR = src
INC_DIR = libraries/include
LIB_DIR = libraries/bin
OBJ_DIR = build/obj
TARGET  = build/app.exe

# Recursively find all .cpp and .c source files (works in Git Bash, Linux, macOS)
CPP_SRCS = $(shell find $(SRC_DIR) -name "*.cpp")
C_SRCS   = $(shell find $(SRC_DIR) -name "*.c")

# Generate object files with path mirroring: src/backends/OpenGL/file.cpp → build/obj/src/backends/OpenGL/file.o
OBJS = $(CPP_SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o) \
       $(C_SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Flags
CXXFLAGS = -I$(INC_DIR) -I$(SRC_DIR) -Wall -Wextra -std=c++17
CFLAGS   = -I$(INC_DIR) -I$(SRC_DIR) -Wall -Wextra -std=c99
LDFLAGS  = -L$(LIB_DIR) -l:libglfw3.a -lopengl32 -lgdi32 -lwinmm

# Default target
all: clean $(TARGET)

# Link executable
$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $^ -o $@ $(LDFLAGS)

# Compile C++ files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile C files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build
clean:
	rm -rf build/

# Phony targets
.PHONY: all clean
