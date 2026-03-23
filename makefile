# Toolchain configuration
CC = gcc
CXX = g++
CFLAGS = -Wall -Wextra -g -O2
CXXFLAGS = -Wall -Wextra -g -O2

# Linker flags for detailed map file
# -Map generates map file
# --cref adds cross-reference information
# --print-memory-usage shows memory usage summary
# --sort-common groups common symbols together
LDFLAGS = -Wl,-Map=$(MAPFILE),--cref,--print-memory-usage,--sort-common

# Project directories
BUILD_DIR = build

# Map file output
MAPFILE = $(BUILD_DIR)/program.map

# Optional: Build specific file by passing FILE=filename.cpp
# Usage: make FILE=demo.cpp  or  make run FILE=demo.cpp
ifdef FILE
  SRCS = $(FILE)
  # Determine if it's C or C++
  ifeq ($(suffix $(FILE)),.c)
    COMPILER = $(CC)
    FFLAGS = $(CFLAGS)
  else
    COMPILER = $(CXX)
    FFLAGS = $(CXXFLAGS)
  endif
  TARGET = $(BUILD_DIR)/$(basename $(FILE))
else
  # Build all source files (default)
  C_SRCS = $(wildcard *.c)
  CPP_SRCS = $(wildcard *.cpp)
  C_OBJS = $(C_SRCS:%.c=$(BUILD_DIR)/%.o)
  CPP_OBJS = $(CPP_SRCS:%.cpp=$(BUILD_DIR)/%.o)
  OBJS = $(C_OBJS) $(CPP_OBJS)
  TARGET = $(BUILD_DIR)/program
  COMPILER = $(CXX)
  FFLAGS = $(CXXFLAGS)
endif

# Default target
all: $(TARGET)

# Build verbose - shows all files being compiled
verbose: clean
	@echo "========================================"
	@echo "Building all files with verbose output"
	@echo "========================================"
	@echo "C sources:"
	@for file in $(C_SRCS); do echo "  - $$file"; done
	@echo "C++ sources:"
	@for file in $(CPP_SRCS); do echo "  - $$file"; done
	@echo ""
	$(MAKE) $(TARGET)

# Build executable
$(TARGET): $(OBJS) | $(BUILD_DIR)
ifdef FILE
	@echo ""
	@echo "[LINK] Linking single file: $(FILE)"
	$(COMPILER) $(FFLAGS) $(LDFLAGS) $(FILE) -o $@
	@echo "[OK] Build complete: $@ (from $(FILE))"
	@echo "     Map file: $(MAPFILE)"
else
	@echo ""
	@echo "[LINK] Linking all object files..."
	@echo "Objects: $(OBJS)"
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJS) -o $@
	@echo "[OK] Build complete: $@"
	@echo "     Output: $(TARGET)"
	@echo "     Map file: $(MAPFILE)"
endif
	@echo ""

# Compile C source files
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	@echo "[CC] Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "     Created: $@"

# Compile C++ source files
$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	@echo "[CXX] Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "     Created: $@"

# Create build directory
$(BUILD_DIR):
	mkdir -p $@

# Clean build artifacts
clean:
	@if exist "$(BUILD_DIR)" rmdir /s /q "$(BUILD_DIR)" || echo "Build directory already removed"
	@echo "✓ Build directory cleaned"

# Rebuild
rebuild: clean all

# Run the program
run: $(TARGET)
	$(TARGET)

# List all source files or show specific file info
info:
	@echo "========================================"
	@echo "Build System Information"
	@echo "========================================"
ifdef FILE
	@echo "Single file mode: $(FILE)"
	@echo "Output: $(TARGET)"
else
	@echo "C sources found:"
	@for file in $(C_SRCS); do echo "  • $$file"; done
	@echo ""
	@echo "C++ sources found:"
	@for file in $(CPP_SRCS); do echo "  • $$file"; done
	@echo ""
	@echo "Default output: $(TARGET)"
endif
	@echo ""
	@echo "========================================"
	@echo "Available targets:"
	@echo "========================================"
	@echo "  make              - Build all files"
	@echo "  make verbose      - Clean and build with full output"
	@echo "  make FILE=name    - Build specific file"
	@echo "  make run          - Build and run all"
	@echo "  make run FILE=... - Build and run specific file"
	@echo ""
	@echo "MAP FILE ANALYSIS:"
	@echo "  make map          - Show full map file with analysis"
	@echo "  make mapstats     - Show statistics about map file"
	@echo "  make symbols      - List symbols sorted by size"
	@echo "  make sections     - Show memory sections layout"
	@echo ""
	@echo "OTHER:"
	@echo "  make summary      - Show build artifacts info"
	@echo "  make sources      - List all source files"
	@echo "  make clean        - Remove build directory"
	@echo "  make rebuild      - Clean and build"
	@echo "  make info         - Show this help"
	@echo "========================================"
	@echo ""

# Phony targets
.PHONY: all clean rebuild run info verbose sources map summary mapstats symbols sections

# Show all detected source files with full paths
sources:
	@echo "========================================"
	@echo "All Source Files in Project"
	@echo "========================================"
	@echo "C Source Files:"
	@ls -lh *.c 2>/dev/null || echo "  (none found)"
	@echo ""
	@echo "C++ Source Files:"
	@ls -lh *.cpp 2>/dev/null || echo "  (none found)"
	@echo ""

# View the generated map file with analysis
map: $(TARGET)
	@if [ -f "$(MAPFILE)" ]; then \
		echo "";\
		echo "╔════════════════════════════════════════════════════════╗";\
		echo "║          LINKER MAP FILE ANALYSIS - $(TARGET)           ║";\
		echo "╚════════════════════════════════════════════════════════╝";\
		echo "";\
		echo "📍 Full Map File Contents:";\
		echo "════════════════════════════════════════════════════════";\
		cat "$(MAPFILE)";\
		echo "";\
		echo "";\
		echo "📊 Symbol Analysis:";\
		echo "════════════════════════════════════════════════════════";\
		echo "Largest symbols (by size):";\
		grep -E "^\s+0x" "$(MAPFILE)" | awk '{print $$2, $$1}' | sort -rn | head -20 || echo "(no symbols found)";\
		echo "";\
		echo "📈 Memory Usage Summary:";\
		grep -E "Memory region|Name|Origin|Length|Attributes" "$(MAPFILE)" | head -20;\
	else \
		echo "❌ Error: Map file not found at $(MAPFILE)";\
		echo "Build first with: make";\
	fi

# Symbol analysis - show symbols sorted by size
symbols: $(TARGET)
	@if [ -f "$(MAPFILE)" ]; then \
		echo "════════════════════════════════════════════════════════";\
		echo "Symbol Table (sorted by size)";\
		echo "════════════════════════════════════════════════════════";\
		grep -E "^\s+0x" "$(MAPFILE)" | awk '{size=$$2; symbol=$$3; if(size!="") print size, symbol}' | sort -rn | awk '{printf "%-10s  %s\n", $$1, $$2}' | head -30;\
		echo "";\
	else \
		echo "Error: Build first with: make";\
	fi

# Section analysis - show memory layout
sections: $(TARGET)
	@if [ -f "$(MAPFILE)" ]; then \
		echo "════════════════════════════════════════════════════════";\
		echo "Memory Sections Layout";\
		echo "════════════════════════════════════════════════════════";\
		grep -A 50 "Linker script and memory map" "$(MAPFILE)" | grep -E "^\s+\.text|^\s+\.data|^\s+\.bss|^\s+\.rodata" | head -40;\
		echo "";\
	else \
		echo "Error: Build first with: make";\
	fi

# Generate a human-readable summary from map file
mapstats: $(TARGET)
	@if [ -f "$(MAPFILE)" ]; then \
		echo "";\
		echo "╔════════════════════════════════════════════════════════╗";\
		echo "║              BUILD MAP FILE STATISTICS                 ║";\
		echo "╚════════════════════════════════════════════════════════╝";\
		echo "";\
		echo "Map file: $(MAPFILE)";\
		echo "File size: $$(wc -c < $(MAPFILE)) bytes";\
		echo "Lines: $$(wc -l < $(MAPFILE))";\
		echo "";\
		echo "Executable: $(TARGET)";\
		if [ -f "$(TARGET)" ]; then \
			echo "Executable size: $$(wc -c < $(TARGET)) bytes";\
		fi;\
		echo "";\
		echo "Archive members included:";\
		grep "^Archive member included" "$(MAPFILE)" | wc -l | awk '{print $$1, "libraries"}';\
		echo "";\
		echo "Symbols defined:";\
		grep -c "^\s+0x" "$(MAPFILE)" | awk '{print $$1, "symbols"}';\
		echo "";\
	else \
		echo "Error: Build first with: make";\
	fi