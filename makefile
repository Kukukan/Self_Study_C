# ====================================================================
# EMBEDDED ENGINEERING BUILD SYSTEM
# Professional GNU Make configuration with multiple build modes
# ====================================================================

# ====================================================================
# TOOLCHAIN CONFIGURATION
# ====================================================================
CC = gcc
CXX = g++
AR = ar
OBJDUMP = objdump
READELF = readelf
NM = nm
STRIP = strip

# ====================================================================
# DETECT OS FOR CROSS-PLATFORM COMMANDS
# ====================================================================
ifeq ($(OS),Windows_NT)
    PLATFORM = Windows
    RM = del /q
    RMDIR = rmdir /s /q
    MKDIR = mkdir
else
    PLATFORM = Unix
    RM = rm -f
    RMDIR = rm -rf
    MKDIR = mkdir -p
endif

# ====================================================================
# BUILD MODE SELECTION
# ====================================================================
# Usage: make MODE=debug   (default)
#        make MODE=release
#        make MODE=minsize (smallest binary)
#        make MODE=profile (with profiling)
#        make MODE=sanitize (with AddressSanitizer)
#        make MODE=coverage (with code coverage)
ifdef MODE
  BUILD_MODE = $(MODE)
else
  BUILD_MODE = debug
endif

# ====================================================================
# BASE COMPILER FLAGS (Common to all modes)
# ====================================================================
COMMON_CFLAGS = -Wall -Wextra -Wpedantic -fvisibility=hidden
COMMON_CFLAGS += -Wstrict-prototypes -Wshadow -Wwrite-strings
COMMON_CFLAGS += -Wpointer-arith -Wcast-qual -Wcast-align
COMMON_CFLAGS += -Werror=implicit-function-declaration
COMMON_CFLAGS += -Wno-unused-parameter

COMMON_CXXFLAGS = -Wall -Wextra -Wpedantic -fvisibility=hidden
COMMON_CXXFLAGS += -Wshadow -Woverloaded-virtual -Weffc++
COMMON_CXXFLAGS += -Wnon-virtual-dtor -Wold-style-cast
COMMON_CXXFLAGS += -Wconversion -Wsign-conversion

# ====================================================================
# BUILD MODE: DEBUG (Development with full debugging)
# ====================================================================
ifeq ($(BUILD_MODE),debug)
  CFLAGS = $(COMMON_CFLAGS) -g3 -O0 -fno-omit-frame-pointer
  CFLAGS += -DDEBUG=1 -D_GLIBCXX_DEBUG
  CXXFLAGS = $(COMMON_CXXFLAGS) -g3 -O0 -fno-omit-frame-pointer
  CXXFLAGS += -DDEBUG=1 -D_GLIBCXX_DEBUG
  LDFLAGS = -Wl,-Map=$(MAPFILE),--cref,--print-memory-usage,--sort-common
  MODE_DESC = DEBUG (full symbols, no optimization)
endif

# ====================================================================
# BUILD MODE: RELEASE (Optimized for speed)
# ====================================================================
ifeq ($(BUILD_MODE),release)
  CFLAGS = $(COMMON_CFLAGS) -g0 -O2 -DNDEBUG
  CFLAGS += -fomit-frame-pointer -flto
  CXXFLAGS = $(COMMON_CXXFLAGS) -g0 -O2 -DNDEBUG
  CXXFLAGS += -fomit-frame-pointer -flto
  LDFLAGS = -Wl,-Map=$(MAPFILE),--cref,--print-memory-usage,--sort-common
  LDFLAGS += -flto -s
  MODE_DESC = RELEASE (O2, LTO, stripped)
endif

# ====================================================================
# BUILD MODE: MINSIZE (Optimized for code size)
# ====================================================================
ifeq ($(BUILD_MODE),minsize)
  CFLAGS = $(COMMON_CFLAGS) -g0 -Os -DNDEBUG
  CFLAGS += -fomit-frame-pointer -flto -ffunction-sections -fdata-sections
  CXXFLAGS = $(COMMON_CXXFLAGS) -g0 -Os -DNDEBUG
  CXXFLAGS += -fomit-frame-pointer -flto -ffunction-sections -fdata-sections
  LDFLAGS = -Wl,-Map=$(MAPFILE),--cref,--print-memory-usage,--sort-common
  LDFLAGS += -flto -s -Wl,--gc-sections
  MODE_DESC = MINSIZE (Os, LTO, section GC)
endif

# ====================================================================
# BUILD MODE: PROFILE (With profiling instrumentation)
# ====================================================================
ifeq ($(BUILD_MODE),profile)
  CFLAGS = $(COMMON_CFLAGS) -g2 -O2 -pg --coverage
  CFLAGS += -fno-omit-frame-pointer -finstrument-functions
  CXXFLAGS = $(COMMON_CXXFLAGS) -g2 -O2 -pg --coverage
  CXXFLAGS += -fno-omit-frame-pointer -finstrument-functions
  LDFLAGS = -Wl,-Map=$(MAPFILE),--cref,--print-memory-usage,--sort-common
  LDFLAGS += -pg --coverage
  MODE_DESC = PROFILE (gprof, code coverage)
endif

# ====================================================================
# BUILD MODE: SANITIZE (With AddressSanitizer & UBSan)
# ====================================================================
ifeq ($(BUILD_MODE),sanitize)
  CFLAGS = $(COMMON_CFLAGS) -g3 -O1
  CFLAGS += -fsanitize=address,undefined -fno-omit-frame-pointer
  CFLAGS += -fsanitize-recover=undefined
  CXXFLAGS = $(COMMON_CXXFLAGS) -g3 -O1
  CXXFLAGS += -fsanitize=address,undefined -fno-omit-frame-pointer
  CXXFLAGS += -fsanitize-recover=undefined
  LDFLAGS = -Wl,-Map=$(MAPFILE),--cref,--print-memory-usage,--sort-common
  LDFLAGS += -fsanitize=address,undefined
  MODE_DESC = SANITIZE (ASan, UBSan)
endif

# ====================================================================
# BUILD MODE: COVERAGE (For code coverage analysis)
# ====================================================================
ifeq ($(BUILD_MODE),coverage)
  CFLAGS = $(COMMON_CFLAGS) -g3 -O0 --coverage -fprofile-arcs -ftest-coverage
  CXXFLAGS = $(COMMON_CXXFLAGS) -g3 -O0 --coverage -fprofile-arcs -ftest-coverage
  LDFLAGS = -Wl,-Map=$(MAPFILE),--cref,--print-memory-usage,--sort-common
  LDFLAGS += --coverage
  MODE_DESC = COVERAGE (gcov instrumentation)
endif

# ====================================================================
# PROJECT DIRECTORIES & OUTPUT
# ====================================================================
BUILD_DIR = build
BUILD_MODE_DIR = $(BUILD_DIR)/$(BUILD_MODE)
MAPFILE = $(BUILD_MODE_DIR)/program.map
PROFILE_DIR = $(BUILD_MODE_DIR)/profile
COVERAGE_DIR = $(BUILD_MODE_DIR)/coverage

# ====================================================================
# BUILD FILE MANAGEMENT
# ====================================================================
# Usage: make FILE=demo.cpp  or  make FILE=demo.c
ifdef FILE
  SRCS = $(FILE)
  ifeq ($(suffix $(FILE)),.c)
    COMPILER = $(CC)
    FFLAGS = $(CFLAGS)
  else
    COMPILER = $(CXX)
    FFLAGS = $(CXXFLAGS)
  endif
  TARGET = $(BUILD_MODE_DIR)/$(basename $(FILE))
else
  C_SRCS = $(wildcard *.c)
  CPP_SRCS = $(wildcard *.cpp)
  C_OBJS = $(C_SRCS:%.c=$(BUILD_MODE_DIR)/%.o)
  CPP_OBJS = $(CPP_SRCS:%.cpp=$(BUILD_MODE_DIR)/%.o)
  OBJS = $(C_OBJS) $(CPP_OBJS)
  TARGET = $(BUILD_MODE_DIR)/program
  COMPILER = $(CXX)
  FFLAGS = $(CXXFLAGS)
endif

# ====================================================================
# TARGETS
# ====================================================================
.PHONY: all clean rebuild run info verbose sources map mapstats symbols
.PHONY: sections crossref config profile coverage sanitize strip

# Default target
all: $(TARGET)

# Build executable
$(TARGET): $(OBJS) | $(BUILD_MODE_DIR)
ifdef FILE
	@echo ""
	@echo "[LINK] Linking single file: $(FILE)"
	@echo "       Mode: $(MODE_DESC)"
	$(COMPILER) $(FFLAGS) $(LDFLAGS) $(FILE) -o $@
	@echo "[OK] Build complete: $@"
	@echo "     Target: $(FILE)"
	@echo "     Mode: $(BUILD_MODE)"
	@echo "     Output: $(TARGET)"
	@echo "     Map file: $(MAPFILE)"
else
	@echo ""
	@echo "[LINK] Linking executable..."
	@echo "       Mode: $(MODE_DESC)"
	@echo "       Objects: $(OBJS)"
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJS) -o $@
	@echo "[OK] Build complete: $@"
	@echo "     Output: $(TARGET)"
	@echo "     Mode: $(BUILD_MODE)"
	@echo "     Map file: $(MAPFILE)"
endif
	@echo ""

# Compile C source files
$(BUILD_MODE_DIR)/%.o: %.c | $(BUILD_MODE_DIR)
	@echo "[CC] Compiling $< ($(BUILD_MODE))..."
	$(CC) $(CFLAGS) -c $< -o $@

# Compile C++ source files
$(BUILD_MODE_DIR)/%.o: %.cpp | $(BUILD_MODE_DIR)
	@echo "[CXX] Compiling $< ($(BUILD_MODE))..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create build directories
$(BUILD_MODE_DIR):
	@$(MKDIR) "$(BUILD_MODE_DIR)"
	@$(MKDIR) "$(PROFILE_DIR)" 2>/dev/null || true
	@$(MKDIR) "$(COVERAGE_DIR)" 2>/dev/null || true

# ====================================================================
# BUILD TARGETS WITH SPECIFIC MODES
# ====================================================================
.PHONY: debug release minsize profile sanitize coverage

debug:
	@$(MAKE) all MODE=debug

release:
	@$(MAKE) all MODE=release

minsize:
	@$(MAKE) all MODE=minsize

profile:
	@$(MAKE) all MODE=profile

sanitize:
	@$(MAKE) all MODE=sanitize

coverage:
	@$(MAKE) all MODE=coverage

# ====================================================================
# UTILITY TARGETS
# ====================================================================
verbose: clean
	@echo "========================================"
	@echo "Building with verbose output"
	@echo "Mode: $(BUILD_MODE) - $(MODE_DESC)"
	@echo "========================================"
	@echo "C sources:"
	@for file in $(C_SRCS); do echo "  - $$file"; done
	@echo "C++ sources:"
	@for file in $(CPP_SRCS); do echo "  - $$file"; done
	@echo ""
	$(MAKE) $(TARGET)

# Show build configuration
config:
	@echo "========================================"
	@echo "Build Configuration"
	@echo "========================================"
	@echo "Platform: $(PLATFORM)"
	@echo "Toolchain: GCC/G++"
	@echo ""
	@echo "Active Build Mode: $(BUILD_MODE)"
	@echo "Description: $(MODE_DESC)"
	@echo ""
	@echo "Directories:"
	@echo "  Build: $(BUILD_MODE_DIR)"
	@echo "  Output: $(TARGET)"
	@echo "  Map file: $(MAPFILE)"
	@echo ""
	@echo "Compiler Flags:"
	@echo "  C: $(CFLAGS)"
	@echo "  C++: $(CXXFLAGS)"
	@echo "  Linker: $(LDFLAGS)"
	@echo ""

# Clean build artifacts
clean:
	@-$(RMDIR) "$(BUILD_DIR)" 2>/dev/null || -$(RMDIR) $(BUILD_DIR) 2>nul || echo ""
	@-$(RM) *.exe 2>/dev/null || -$(RM) "*.exe" 2>nul || echo ""
	@-$(RM) *.o 2>/dev/null || -$(RM) "*.o" 2>nul || echo ""
	@echo "✓ Build cleaned ($(PLATFORM))"

rebuild: clean all

run: $(TARGET)
	$(TARGET)

# Strip debug symbols (for release builds)
strip: $(TARGET)
	@echo "[STRIP] Removing debug symbols from $(TARGET)..."
	@$(STRIP) $(TARGET)
	@echo "[OK] Symbols stripped"

# ====================================================================
# SOURCE FILE LISTING
# ====================================================================
sources:
	@echo "========================================"
	@echo "All Source Files in Project"
	@echo "========================================"
ifeq ($(PLATFORM),Windows)
	@echo "C Source Files:"
	@dir *.c 2>nul || echo "  (none found)"
	@echo ""
	@echo "C++ Source Files:"
	@dir *.cpp 2>nul || echo "  (none found)"
else
	@echo "C Source Files:"
	@ls -lh *.c 2>/dev/null || echo "  (none found)"
	@echo ""
	@echo "C++ Source Files:"
	@ls -lh *.cpp 2>/dev/null || echo "  (none found)"
endif
	@echo ""

# ====================================================================
# MAP FILE ANALYSIS TARGETS
# ====================================================================

# Full map file display
map: $(TARGET)
ifeq ($(PLATFORM),Windows)
	@powershell -NoProfile -Command "if (Test-Path '$(MAPFILE)') { Write-Host ''; Write-Host '╔════════════════════════════════════════════════════════╗'; Write-Host '║          LINKER MAP FILE ANALYSIS                       ║'; Write-Host '╚════════════════════════════════════════════════════════╝'; Write-Host ''; Write-Host '📍 Map File Contents:'; Write-Host '════════════════════════════════════════════════════════'; Get-Content '$(MAPFILE)' } else { Write-Host '❌ Error: Map file not found'; Write-Host 'Build first with: make' }"
else
	@if [ -f "$(MAPFILE)" ]; then \
		echo "";\
		echo "╔════════════════════════════════════════════════════════╗";\
		echo "║          LINKER MAP FILE ANALYSIS                      ║";\
		echo "╚════════════════════════════════════════════════════════╝";\
		echo "";\
		echo "📍 Full Map File Contents:";\
		echo "════════════════════════════════════════════════════════";\
		cat "$(MAPFILE)";\
	else \
		echo "❌ Error: Map file not found at $(MAPFILE)";\
		echo "Build first with: make";\
	fi
endif

# Symbol table analysis
symbols: $(TARGET)
ifeq ($(PLATFORM),Windows)
	@powershell -NoProfile -Command "if (Test-Path '$(MAPFILE)') { Write-Host ''; Write-Host '════════════════════════════════════════════════════════'; Write-Host 'Symbol Table (top 30 by size)'; Write-Host '════════════════════════════════════════════════════════'; Write-Host ''; Get-Content '$(MAPFILE)' | Select-String -Pattern '^\s{1,}0x' | ForEach-Object { $$_.Line } | Select-Object -First 30; Write-Host '' } else { Write-Host 'Error: Build first with: make' }"
else
	@if [ -f "$(MAPFILE)" ]; then \
		echo "════════════════════════════════════════════════════════";\
		echo "Symbol Table (sorted by size)";\
		echo "════════════════════════════════════════════════════════";\
		grep -E "^\s+0x" "$(MAPFILE)" | awk '{size=$$2; symbol=$$3; if(size!="") print size, symbol}' | sort -rn | awk '{printf "%-10s  %s\n", $$1, $$2}' | head -30;\
		echo "";\
	else \
		echo "Error: Build first with: make";\
	fi
endif

# Memory sections layout
sections: $(TARGET)
ifeq ($(PLATFORM),Windows)
	@powershell -NoProfile -Command "if (Test-Path '$(MAPFILE)') { Write-Host ''; Write-Host '════════════════════════════════════════════════════════'; Write-Host 'Memory Sections Layout'; Write-Host '════════════════════════════════════════════════════════'; Write-Host ''; (Get-Content '$(MAPFILE)' | Select-String -Pattern '\.text|\.data|\.bss|\.rodata' | Select-Object -First 40) -join [Environment]::NewLine | Write-Output; Write-Host '' } else { Write-Host 'Error: Build first with: make' }"
else
	@if [ -f "$(MAPFILE)" ]; then \
		echo "════════════════════════════════════════════════════════";\
		echo "Memory Sections Layout";\
		echo "════════════════════════════════════════════════════════";\
		grep -A 50 "Linker script and memory map" "$(MAPFILE)" | grep -E "^\s+\.text|^\s+\.data|^\s+\.bss|^\s+\.rodata" | head -40;\
		echo "";\
	else \
		echo "Error: Build first with: make";\
	fi
endif

# Map file statistics
mapstats: $(TARGET)
ifeq ($(PLATFORM),Windows)
	@powershell -NoProfile -Command "if (Test-Path '$(MAPFILE)') { Write-Host ''; Write-Host '╔════════════════════════════════════════════════════════╗'; Write-Host '║              BUILD MAP FILE STATISTICS                 ║'; Write-Host '╚════════════════════════════════════════════════════════╝'; Write-Host ''; $$mapSize = (Get-Item '$(MAPFILE)').Length; Write-Host 'Map file: $(MAPFILE)'; Write-Host ('File size: ' + $$mapSize + ' bytes'); Write-Host ''; if (Test-Path '$(TARGET)') { $$exeSize = (Get-Item '$(TARGET)').Length; Write-Host 'Executable: $(TARGET)'; Write-Host ('Executable size: ' + $$exeSize + ' bytes'); Write-Host '' }; Write-Host '(Full analysis requires Unix tools. See README for details)'; Write-Host '' } else { Write-Host 'Error: Build first with: make' }"
else
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
endif

# Cross-reference table
crossref: $(TARGET)
ifeq ($(PLATFORM),Windows)
	@powershell -NoProfile -Command "if (Test-Path '$(MAPFILE)') { Write-Host ''; Write-Host '╔════════════════════════════════════════════════════════╗'; Write-Host '║              CROSS REFERENCE TABLE                     ║'; Write-Host '║        (Which symbols reference which symbols)         ║'; Write-Host '╚════════════════════════════════════════════════════════╝'; Write-Host ''; Write-Host 'Cross reference data available in: $(MAPFILE)'; Write-Host '(Use ''make map'' for full output. Detailed analysis requires Unix tools.)'; Write-Host '' } else { Write-Host 'Error: Build first with: make' }"
else
	@if [ -f "$(MAPFILE)" ]; then \
		echo "";\
		echo "╔════════════════════════════════════════════════════════╗";\
		echo "║              CROSS REFERENCE TABLE                     ║";\
		echo "║        (Which symbols reference which symbols)         ║";\
		echo "╚════════════════════════════════════════════════════════╝";\
		echo "";\
		grep -A 500 "Cross Reference Table" "$(MAPFILE)" | head -100;\
		echo "";\
	else \
		echo "Error: Build first with: make";\
	fi
endif

# ====================================================================
# PROFILING & ANALYSIS TARGETS
# ====================================================================

# Profile execution (requires MODE=profile)
profile: profile
	@echo ""
	@echo "╔════════════════════════════════════════════════════════╗"
	@echo "║                   GPROF ANALYSIS                       ║"
	@echo "╚════════════════════════════════════════════════════════╝"
	@echo ""
	@echo "Run the profiled executable first:"
	@echo "  cd $(BUILD_MODE_DIR) && ./program"
	@echo ""
	@echo "Then generate gprof report:"
	@echo "  gprof ./program gmon.out > gprof_report.txt"
	@echo ""

# Generate coverage report (requires MODE=coverage)
coverage-report: coverage
	@echo ""
	@echo "╔════════════════════════════════════════════════════════╗"
	@echo "║                 CODE COVERAGE REPORT                   ║"
	@echo "╚════════════════════════════════════════════════════════╝"
	@echo ""
	@echo "Generate coverage report:"
	@echo "  cd $(BUILD_MODE_DIR) && gcov ../../*.cpp ../../*.c"
	@echo ""
	@echo "Then create HTML report:"
	@echo "  genhtml --output-directory coverage_html *.gcov"
	@echo ""

# ====================================================================
# HELP & INFO
# ====================================================================
info:
	@echo ""
	@echo "════════════════════════════════════════════════════════════"
	@echo "  EMBEDDED ENGINEERING BUILD SYSTEM"
	@echo "════════════════════════════════════════════════════════════"
	@echo ""
	@echo "CURRENT CONFIG:"
	@echo "  Platform:   $(PLATFORM)"
	@echo "  Mode:       $(BUILD_MODE) ($(MODE_DESC))"
	@echo "  Output:     $(TARGET)"
	@echo ""
	@echo "════════════════════════════════════════════════════════════"
	@echo "BUILD COMMANDS:"
	@echo "════════════════════════════════════════════════════════════"
	@echo "  make debug       - Build with full debugging (default)"
	@echo "  make release     - Optimized build (O2, LTO, stripped)"
	@echo "  make minsize     - Minimum size build (Os, LTO, GC)"
	@echo "  make profile     - Build with gprof instrumentation"
	@echo "  make sanitize    - Build with AddressSanitizer + UBSan"
	@echo "  make coverage    - Build with gcov code coverage"
	@echo ""
	@echo "  make verbose     - Show compilation details"
	@echo "  make FILE=name   - Build specific file"
	@echo "  make run         - Build and execute"
	@echo ""
	@echo "════════════════════════════════════════════════════════════"
	@echo "BUILD MODE FEATURES:"
	@echo "════════════════════════════════════════════════════════════"
	@echo "  DEBUG:    -g3 -O0 full symbols"
	@echo "  RELEASE:  -g0 -O2 LTO, stripped"
	@echo "  MINSIZE:  -g0 -Os LTO, section GC"
	@echo "  PROFILE:  -g2 -O2 gprof, code coverage"
	@echo "  SANITIZE: -g3 -O1 AddressSanitizer, UBSan"
	@echo "  COVERAGE: -g3 -O0 gcov instrumentation"
	@echo ""
	@echo "════════════════════════════════════════════════════════════"
	@echo "ANALYSIS TARGETS:"
	@echo "════════════════════════════════════════════════════════════"
	@echo "  make map         - Show full linker map"
	@echo "  make symbols     - List largest symbols"
	@echo "  make sections    - Show memory layout"
	@echo "  make mapstats    - Display statistics"
	@echo "  make crossref    - Cross-reference table"
	@echo ""
	@echo "  make config      - Show build configuration"
	@echo "  make sources     - List all source files"
	@echo "  make strip       - Remove debug symbols"
	@echo ""
	@echo "════════════════════════════════════════════════════════════"
	@echo "EXAMPLES:"
	@echo "════════════════════════════════════════════════════════════"
	@echo "  make                    # Build with current mode"
	@echo "  make MODE=release       # Override mode: make MODE=release"
	@echo "  make FILE=test.c        # Build single file"
	@echo "  make debug && make map  # Debug build + show map"
	@echo ""
	@echo "════════════════════════════════════════════════════════════"
	@echo ""

# Default target
all: $(TARGET)

# Build verbose - shows all files being compiled
verbose: clean
	@echo "========================================"
	@echo "Building all files with verbose output"
	@echo "Platform: $(PLATFORM)"
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

# Create build directory (cross-platform)
$(BUILD_DIR):
	@$(MKDIR) "$(BUILD_DIR)"

# Clean build artifacts (cross-platform)
.PHONY: clean
clean:
	@-$(RMDIR) "$(BUILD_DIR)" 2>/dev/null || -$(RMDIR) $(BUILD_DIR) 2>nul || echo ""
	@-$(RM) *.exe 2>/dev/null || -$(RM) "*.exe" 2>nul || echo ""
	@-$(RM) *.o 2>/dev/null || -$(RM) "*.o" 2>nul || echo ""
	@echo "✓ Build directory and temporary files cleaned ($(PLATFORM))"

# Rebuild
rebuild: clean all

# Run the program
run: $(TARGET)
	$(TARGET)

# List all source files or show specific file info
info:
	@echo "========================================"
	@echo "Build System Information"
	@echo "Platform: $(PLATFORM)"
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
	@echo "  make crossref     - Show cross-reference table"
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
.PHONY: all clean rebuild run info verbose sources map summary mapstats symbols sections crossref

# Show all detected source files with full paths
sources:
	@echo "========================================"
	@echo "All Source Files in Project"
	@echo "========================================"
ifeq ($(PLATFORM),Windows)
	@echo "C Source Files:"
	@dir *.c 2>nul || echo "  (none found)"
	@echo ""
	@echo "C++ Source Files:"
	@dir *.cpp 2>nul || echo "  (none found)"
else
	@echo "C Source Files:"
	@ls -lh *.c 2>/dev/null || echo "  (none found)"
	@echo ""
	@echo "C++ Source Files:"
	@ls -lh *.cpp 2>/dev/null || echo "  (none found)"
endif
	@echo ""

# View the generated map file with analysis
map: $(TARGET)
ifeq ($(PLATFORM),Windows)
	@powershell -NoProfile -Command "if (Test-Path '$(MAPFILE)') { Write-Host ''; Write-Host '╔════════════════════════════════════════════════════════╗'; Write-Host '║          LINKER MAP FILE ANALYSIS                       ║'; Write-Host '╚════════════════════════════════════════════════════════╝'; Write-Host ''; Write-Host '📍 Map File Contents:'; Write-Host '════════════════════════════════════════════════════════'; Get-Content '$(MAPFILE)' } else { Write-Host '❌ Error: Map file not found'; Write-Host 'Build first with: make' }"
else
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
endif

# Symbol analysis - show symbols sorted by size
symbols: $(TARGET)
ifeq ($(PLATFORM),Windows)
	@powershell -NoProfile -Command "if (Test-Path '$(MAPFILE)') { Write-Host ''; Write-Host '════════════════════════════════════════════════════════'; Write-Host 'Symbol Table (top 30 by size)'; Write-Host '════════════════════════════════════════════════════════'; Write-Host ''; Get-Content '$(MAPFILE)' | Select-String -Pattern '^\s{1,}0x' | ForEach-Object { $$_.Line } | Select-Object -First 30; Write-Host '' } else { Write-Host 'Error: Build first with: make' }"
else
	@if [ -f "$(MAPFILE)" ]; then \
		echo "════════════════════════════════════════════════════════";\
		echo "Symbol Table (sorted by size)";\
		echo "════════════════════════════════════════════════════════";\
		grep -E "^\s+0x" "$(MAPFILE)" | awk '{size=$$2; symbol=$$3; if(size!="") print size, symbol}' | sort -rn | awk '{printf "%-10s  %s\n", $$1, $$2}' | head -30;\
		echo "";\
	else \
		echo "Error: Build first with: make";\
	fi
endif

# Section analysis - show memory layout
sections: $(TARGET)
ifeq ($(PLATFORM),Windows)
	@powershell -NoProfile -Command "if (Test-Path '$(MAPFILE)') { Write-Host ''; Write-Host '════════════════════════════════════════════════════════'; Write-Host 'Memory Sections Layout'; Write-Host '════════════════════════════════════════════════════════'; Write-Host ''; (Get-Content '$(MAPFILE)' | Select-String -Pattern '\.text|\.data|\.bss|\.rodata' | Select-Object -First 40) -join [Environment]::NewLine | Write-Output; Write-Host '' } else { Write-Host 'Error: Build first with: make' }"
else
	@if [ -f "$(MAPFILE)" ]; then \
		echo "════════════════════════════════════════════════════════";\
		echo "Memory Sections Layout";\
		echo "════════════════════════════════════════════════════════";\
		grep -A 50 "Linker script and memory map" "$(MAPFILE)" | grep -E "^\s+\.text|^\s+\.data|^\s+\.bss|^\s+\.rodata" | head -40;\
		echo "";\
	else \
		echo "Error: Build first with: make";\
	fi
endif

# Generate a human-readable summary from map file
mapstats: $(TARGET)
ifeq ($(PLATFORM),Windows)
	@powershell -NoProfile -Command "if (Test-Path '$(MAPFILE)') { Write-Host ''; Write-Host '╔════════════════════════════════════════════════════════╗'; Write-Host '║              BUILD MAP FILE STATISTICS                 ║'; Write-Host '╚════════════════════════════════════════════════════════╝'; Write-Host ''; $$mapSize = (Get-Item '$(MAPFILE)').Length; Write-Host 'Map file: $(MAPFILE)'; Write-Host ('File size: ' + $$mapSize + ' bytes'); Write-Host ''; if (Test-Path '$(TARGET)') { $$exeSize = (Get-Item '$(TARGET)').Length; Write-Host 'Executable: $(TARGET)'; Write-Host ('Executable size: ' + $$exeSize + ' bytes'); Write-Host '' }; Write-Host '(Full analysis requires Unix tools. See README for details)'; Write-Host '' } else { Write-Host 'Error: Build first with: make' }"
else
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
endif

# Cross-reference table - shows symbol dependencies
crossref: $(TARGET)
ifeq ($(PLATFORM),Windows)
	@powershell -NoProfile -Command "if (Test-Path '$(MAPFILE)') { Write-Host ''; Write-Host '╔════════════════════════════════════════════════════════╗'; Write-Host '║              CROSS REFERENCE TABLE                     ║'; Write-Host '║        (Which symbols reference which symbols)         ║'; Write-Host '╚════════════════════════════════════════════════════════╝'; Write-Host ''; Write-Host 'Cross reference data available in: $(MAPFILE)'; Write-Host '(Use ''make map'' for full output. Detailed analysis requires Unix tools.)'; Write-Host '' } else { Write-Host 'Error: Build first with: make' }"
else
	@if [ -f "$(MAPFILE)" ]; then \
		echo "";\
		echo "╔════════════════════════════════════════════════════════╗";\
		echo "║              CROSS REFERENCE TABLE                     ║";\
		echo "║        (Which symbols reference which symbols)         ║";\
		echo "╚════════════════════════════════════════════════════════╝";\
		echo "";\
		grep -A 500 "Cross Reference Table" "$(MAPFILE)" | head -100;\
		echo "";\
	else \
		echo "Error: Build first with: make";\
	fi
endif