# Compiler
CXX = g++
CXXFLAGS_BASE = -Wall -std=c++11

# OS Detection and Compatibility
ifeq ($(OS),Windows_NT)
    RM = del /Q /F
    TARGET_EXT = .exe
    LDFLAGS_EXTRA = -mconsole
    FIX_PATH = $(subst /,\,$1)
else
    RM = rm -f
    TARGET_EXT =
    LDFLAGS_EXTRA =
    FIX_PATH = $1
endif

# Default target
all: help

# Display available commands
help:
	@echo "Available build options:"
	@echo "  make sdl2   - Compile test-sdl2.cpp (SDL2 version)"
	@echo "  make sdl3   - Compile test-sdl3.cpp (SDL3 version)"
	@echo "  make clean  - Remove executable files"

# Build SDL2 version
sdl2: test-sdl2.cpp
	$(CXX) test-sdl2.cpp -o test-sdl2$(TARGET_EXT) \
	$(CXXFLAGS_BASE) $(shell pkg-config --cflags sdl2) \
	$(shell pkg-config --libs sdl2) $(LDFLAGS_EXTRA)
	@echo "Build successful: Run ./test-sdl2$(TARGET_EXT) to start"

# Build SDL3 version
sdl3: test-sdl3.cpp
	$(CXX) test-sdl3.cpp -o test-sdl3$(TARGET_EXT) \
	$(CXXFLAGS_BASE) $(shell pkg-config --cflags sdl3) \
	$(shell pkg-config --libs sdl3) $(LDFLAGS_EXTRA)
	@echo "Build successful: Run ./test-sdl3$(TARGET_EXT) to start"

# Cleanup executables and objects
clean:
	$(RM) test-sdl2$(TARGET_EXT) test-sdl3$(TARGET_EXT) *.o
	@echo "Cleaned up all build files"