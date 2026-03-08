# LẬP TRÌNH TRÊN LINUX - TRAINING MATERIALS
*Getting Familiar with Linux Programming*

**Giảng viên:** Lưu An Phú  
**©FPT SOFTWARE - Corporate Training Center - Internal Use**

---

## 📋 MỤC LỤC

### PHẦN I: NỘI DUNG BÀI GIẢNG
- [Giới thiệu khóa học](#giới-thiệu-khóa-học)
- [MCU vs Linux Paradigm Shift](#mcu-vs-linux-paradigm-shift)
- [Challenges và Survival Strategy](#challenges-và-survival-strategy)
- [Command Line Fundamentals](#command-line-fundamentals)
- [Development Tools](#development-tools)
- [Debugging Tools](#debugging-tools)
- [Professional Best Practices](#professional-best-practices)

### PHẦN II: EXAMPLE CODE
- [Configuration Parser](#example-config-parser)
- [File Processor](#example-file-processor)
- [Tool Comparison](#example-tool-comparison)

### PHẦN III: BÀI THỰC HÀNH
- [Bài 1: Buffer Overflow Detection](#bài-1-buffer-overflow-detection)
- [Bài 2: Memory Leak Detection](#bài-2-memory-leak-detection)
- [Bài 3: Tool Deep Understanding](#bài-3-tool-deep-understanding)

---

# PHẦN I: NỘI DUNG BÀI GIẢNG

## Giới thiệu khóa học

### 🎯 Objectives
- Tập trung vào các kỹ thuật lập trình device driver
- Porting Linux cho new platform
- Transition từ embedded MCU sang Linux development

### 📋 Prerequisites  
- C programming language
- Microcontroller experience
- Basic embedded concepts

### 📚 References
- The Linux Programming Interface
- Essential Linux Device Drivers  
- Linux Device Drivers
- Documentation trong thư mục etc_document

### ⏱️ Duration
**60 hours / 20 sessions**

---

## MCU vs Linux Paradigm Shift

### Embedded MCU World
- **Single-tasking**: Một chương trình chính, interrupt handlers
- **Direct hardware access**: Trực tiếp thao tác registers
- **Static memory**: Biết trước memory layout
- **Real-time deterministic**: Timing có thể predict chính xác
- **IDE-centric**: Code, compile, flash, debug trong một tool

### Linux World
- **Multi-tasking OS**: Processes, threads, scheduler
- **Hardware abstraction**: Device drivers, virtual memory
- **Dynamic memory**: malloc/free, virtual address space
- **Soft real-time**: Kernel scheduling overhead
- **Command-line centric**: Separate tools cho từng task

**💡 Key Insight**: Không phải "harder" mà là "different approach"

---

## Challenges và Survival Strategy

### TOP 3 BIGGEST CHALLENGES

#### 🧠 Challenge #1: Mindset Shift (⭐ Khó nhất)
**MCU Approach:**
```c
// Direct register manipulation
#define GPIO_BASE 0x40020000
volatile uint32_t *GPIO_ODR = (uint32_t*)(GPIO_BASE + 0x14);

int main() {
    *GPIO_ODR |= (1 << 5);  // Set pin 5 HIGH - direct control
    while(1) {
        *GPIO_ODR ^= (1 << 5);  // Toggle LED
        delay_ms(500);
    }
}
```

**Linux Approach:**
```c
// System call layers
#include <fcntl.h>
#include <unistd.h>

int main() {
    // 6+ layers: app → glibc → kernel → driver → hardware
    int fd = open("/sys/class/gpio/gpio18/value", O_WRONLY);
    write(fd, "1", 1);  // Indirect through filesystem
    close(fd);
    
    // Or through /dev/mem with mmap (requires root)
    // Still not direct - MMU translation involved
}
```

#### 🔍 Challenge #2: Debugging Paradigm Change
**MCU Debugging:**
```c
// Visual, immediate feedback
void debug_gpio() {
    LED_RED_ON();     // Instant visual confirmation
    
    // Hardware debugger: exact register values
    printf("GPIO_ODR: 0x%08X\n", *GPIO_ODR);  // Physical address
}
```

**Linux Debugging:**
```c
// Abstract, instrumented debugging
void debug_linux() {
    printf("Debug info\n");  // Goes through printf → libc → kernel
    
    // dmesg for kernel messages
    // strace for system call tracing
    // gdb for application debugging
    // /proc filesystem for runtime info
}
```

#### 🛠️ Challenge #3: Tool Ecosystem Overwhelm
**MCU Development:**
```c
// One-click development
// 1. Open IDE (Keil/IAR/STM32Cube)
// 2. New Project → Select Chip → Generated code
// 3. F5 to build and debug
```

**Linux Development:**
```bash
# Command line compilation chaos
gcc -g -O2 -Wall -Wextra -std=c99 -D_GNU_SOURCE \
    -I./include -L./lib -lpthread -lm -o myapp \
    src/main.c src/utils.c

# Multiple tools for different tasks
make        # Build automation
gdb         # Debugging
valgrind    # Memory checking
strace      # System call tracing
```

### Survival Strategy

#### Mental Preparation
1. **Accept Different ≠ Harder**: Linux is not "more difficult MCU"
2. **Embrace Abstraction**: Stop fighting the OS, work with it
3. **Layer-by-layer Understanding**: Don't try to understand everything at once

#### Learning Path
- **Week 1-2**: Command line comfort
- **Week 3-4**: Basic development tools
- **Month 2**: Understanding the full stack

#### Success Mantras
- **"When in doubt, check documentation"**: man, --help, online docs
- **"Start simple, build complexity"**: GPIO → PWM → SPI → Custom drivers
- **"Learn tools progressively"**: Don't master vim + gdb + make simultaneously

---

## Command Line Fundamentals

### Essential Commands

#### Navigation & Files
```bash
# Directory operations
ls -la              # List files with details
cd /path/to/dir     # Change directory  
pwd                 # Print working directory
mkdir newdir        # Create directory
rm -rf olddir       # Remove directory

# File operations
cp source dest      # Copy files
mv old new          # Move/rename files
find . -name "*.c"  # Find C files
```

#### System Information
```bash
# Process management
ps aux              # List all processes
top                 # Real-time process monitor
kill -9 <PID>       # Force kill process

# System info
man command         # Manual pages
which gcc           # Find command location
sudo cmd            # Run as superuser
```

#### Text Processing
```bash
# File viewing
cat file.txt        # Display file content
less file.txt       # Paginated viewing
head -n 10 file     # First 10 lines
tail -f file        # Follow file changes

# Search and filter
grep "pattern" file # Search in file
grep -r "text" .    # Recursive search
```

### Package Management

#### MCU vs Linux Libraries
```bash
# MCU: Manual library download and project integration
# Linux: Centralized package management

# Update package database
sudo apt update

# Install packages
sudo apt install build-essential  # GCC, make, etc.
sudo apt install git             # Version control
sudo apt install gdb             # Debugger
sudo apt install valgrind        # Memory debugging

# Search packages
apt search keyword

# Professional Tip: Always update before install
sudo apt update && sudo apt install package_name
```

---

## Development Tools

### Vim Editor - The Linux Native Editor

#### Why Vim Matters for Linux Developers
- **Ubiquitous**: Available on every Linux system
- **Remote development**: Essential for SSH sessions  
- **Efficiency**: Faster than GUI editors once mastered
- **Lightweight**: Works on resource-constrained systems

#### Essential Vim Commands
```bash
# Modes
i           # Insert mode
Esc         # Command mode  
v           # Visual selection

# Navigation
:N          # Go to line N
/pattern    # Search forward
?pattern    # Search backward
n           # Next search result

# File operations
:w          # Save file
:q          # Quit
:wq         # Save and quit
:q!         # Quit without saving

# Editing
dd          # Delete line
yy          # Copy line
p           # Paste
u           # Undo
```

#### Vim Configuration
```bash
# ~/.vimrc - Basic configuration
set number          " Show line numbers
set autoindent      " Auto indentation
set tabstop=4       " Tab width
set expandtab       " Use spaces instead of tabs
syntax on           " Syntax highlighting
```

### GCC Compiler - The Build Foundation

#### MCU vs Linux Compilation
**MCU Compilation:**
```bash
# IDE handles everything:
# - Chip-specific settings
# - Linker scripts
# - Library paths
# - Optimization for flash/RAM
```

**Linux Compilation:**
```bash
# Manual control over everything:
gcc -g -O2 -Wall -Wextra -std=c99 \
    -I./include \           # Include paths
    -L./lib \              # Library paths
    -lpthread -lm \        # Link libraries
    -o myapp \             # Output name
    main.c utils.c         # Source files
```

#### Essential GCC Options
```bash
# Debug vs Release
gcc -g -O0           # Debug: symbols, no optimization
gcc -O3 -DNDEBUG     # Release: max optimization, no debug

# Warnings (catch bugs early)
gcc -Wall -Wextra -Werror    # Comprehensive warnings

# Standards
gcc -std=c99         # C99 standard
gcc -std=gnu99       # GNU extensions

# Security
gcc -fstack-protector-strong  # Buffer overflow protection
gcc -D_FORTIFY_SOURCE=2       # Runtime checks

# Architecture
gcc -march=native    # Optimize for current CPU
gcc -m32            # 32-bit compilation
```

#### Practical Examples
```bash
# Simple compilation
gcc hello.c -o hello

# With debugging
gcc -g -Wall hello.c -o hello_debug

# With math library
gcc -lm math_program.c -o calc

# Multiple files
gcc main.c utils.c network.c -o server

# Cross-compilation for ARM
arm-linux-gnueabihf-gcc -o arm_app main.c
```

### Shell Scripts - Automation Foundation

#### Build Automation Example
```bash
#!/bin/bash
# build.sh - Automated build script

PROJECT="linux_app"
SRC_DIR="src"
BUILD_DIR="build"

echo "Building $PROJECT..."

# Create build directory
mkdir -p $BUILD_DIR

# Compile with different configurations
echo "Building debug version..."
gcc -g -O0 -Wall -DDEBUG \
    $SRC_DIR/*.c \
    -o $BUILD_DIR/${PROJECT}_debug

echo "Building release version..."  
gcc -O3 -DNDEBUG \
    $SRC_DIR/*.c \
    -o $BUILD_DIR/${PROJECT}_release

echo "Build completed!"

# Run tests
if [ "$1" == "test" ]; then
    echo "Running tests..."
    ./$BUILD_DIR/${PROJECT}_debug --test
fi
```

#### Environment Setup Script
```bash
#!/bin/bash
# setup_dev_env.sh

echo "Setting up Linux development environment..."

# Update system
sudo apt update

# Install essential tools
sudo apt install -y \
    build-essential \
    git \
    vim \
    gdb \
    valgrind \
    cscope \
    ctags

# Install additional libraries
sudo apt install -y \
    libc6-dev \
    linux-headers-$(uname -r)

# Configure git
read -p "Enter your name: " name
read -p "Enter your email: " email

git config --global user.name "$name"
git config --global user.email "$email"

echo "Development environment setup completed!"
```

### Makefile - Intelligent Build System

#### Basic Makefile Structure
```makefile
# Makefile for Linux development

CC = gcc
CFLAGS = -g -Wall -Wextra -std=c99
LDFLAGS = -lpthread -lm

# Directories
SRC_DIR = src
INC_DIR = include  
BUILD_DIR = build

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Target executable
TARGET = $(BUILD_DIR)/myapp

# Default target
all: $(TARGET)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Build executable
$(TARGET): $(OBJECTS) | $(BUILD_DIR)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

# Compile object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

# Debug build
debug: CFLAGS += -DDEBUG -O0
debug: $(TARGET)

# Release build  
release: CFLAGS += -DNDEBUG -O3
release: clean $(TARGET)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)

# Install target
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

# Phony targets
.PHONY: all debug release clean install
```

#### Advanced Makefile Features
```makefile
# Automatic dependency tracking
DEPENDS = $(OBJECTS:.o=.d)

# Include dependency files
-include $(DEPENDS)

# Generate dependency files
$(BUILD_DIR)/%.d: $(SRC_DIR)/%.c
	@$(CC) $(CFLAGS) -I$(INC_DIR) -MM $< | \
	sed 's,\($*\)\.o[ :]*,$(BUILD_DIR)/\1.o $@ : ,g' > $@

# Cross-compilation support
ifeq ($(CROSS_COMPILE),arm)
    CC = arm-linux-gnueabihf-gcc
    CFLAGS += -march=armv7-a
endif

# Conditional compilation
ifdef DEBUG
    CFLAGS += -DDEBUG -O0 -g
else
    CFLAGS += -DNDEBUG -O3
endif
```

---

## Debugging Tools

### GDB Debugger - The Linux Debugging Workhorse

#### Basic GDB Workflow
```bash
# Compile with debug symbols
gcc -g -O0 program.c -o program

# Start debugging
gdb ./program

# Or debug core dump
gdb ./program core

# Or attach to running process
gdb -p <PID>
```

#### Essential GDB Commands
```bash
# Running programs
(gdb) run                    # Start program
(gdb) run arg1 arg2          # Start with arguments
(gdb) continue               # Continue execution
(gdb) kill                   # Kill current execution

# Breakpoints
(gdb) break main             # Break at function
(gdb) break file.c:25        # Break at line
(gdb) break *0x400500        # Break at address
(gdb) info breakpoints       # List breakpoints
(gdb) delete 1               # Delete breakpoint 1

# Stepping
(gdb) step                   # Step into functions
(gdb) next                   # Step over functions  
(gdb) finish                 # Step out of function
(gdb) until 30               # Run until line 30

# Examining data
(gdb) print variable         # Print variable value
(gdb) print *pointer         # Dereference pointer
(gdb) x/10x $sp             # Examine stack memory
(gdb) info registers        # Show CPU registers
(gdb) backtrace             # Show call stack

# Watchpoints
(gdb) watch variable        # Break when variable changes
(gdb) rwatch variable       # Break when variable read
```

#### Advanced GDB Techniques
```bash
# Debugging with core dumps
echo "core.%p" > /proc/sys/kernel/core_pattern
ulimit -c unlimited
# When program crashes:
gdb program core.12345

# Remote debugging
# On target: gdbserver :1234 ./program
# On host: 
(gdb) target remote target_ip:1234

# Scripting GDB
cat > debug.gdb << EOF
break main
run
print argc
print argv[0]
continue
EOF

gdb -x debug.gdb ./program
```

### grep and find - Code Navigation Masters

#### grep - Text Search Master
```bash
# Basic search
grep "function_name" *.c           # Search in C files
grep -n "TODO" *.c                 # Show line numbers
grep -i "error" *.log              # Case insensitive

# Recursive search
grep -r "malloc" src/              # Search in directory
grep -r --include="*.c" "struct" . # Only C files

# Advanced patterns
grep "^#include" *.c               # Lines starting with #include
grep "int.*(" *.c                  # Function declarations
grep -E "(malloc|calloc|free)" *.c # Multiple patterns

# Context
grep -A 3 -B 3 "error" file.c     # 3 lines before/after
grep -C 5 "function" file.c       # 5 lines context

# Practical examples
# Find all function definitions
grep -n "^[a-zA-Z_][a-zA-Z0-9_]*.*(.*{$" *.c

# Find memory allocations
grep -rn --include="*.c" "malloc\|calloc\|realloc" .

# Find error handling
grep -rn --include="*.c" "if.*==.*NULL" .
```

#### find - File System Navigator
```bash
# Find by name
find . -name "*.c"                # All C files
find . -name "main.c"             # Specific file
find . -iname "*.H"               # Case insensitive

# Find by type
find . -type f -name "*.c"        # Files only
find . -type d -name "include"    # Directories only

# Find by size/time
find . -size +100M                # Files larger than 100MB
find . -mtime -7                  # Modified in last 7 days
find . -newer reference_file      # Newer than reference

# Combine with commands
find . -name "*.c" -exec grep -l "main" {} \;
find . -name "*.o" -delete        # Delete object files
find . -name "*.c" | xargs wc -l  # Count lines in C files

# Practical examples
# Find large core dumps
find /tmp -name "core.*" -size +10M -delete

# Find source files modified today
find . -name "*.c" -mtime 0

# Find files with specific permissions
find . -perm 755 -type f
```

### cscope - Code Intelligence Tool

#### Setting up cscope
```bash
# Generate cscope database
find . -name "*.c" -o -name "*.h" > cscope.files
cscope -b -i cscope.files

# Or use recursive build
cscope -Rb

# Start interactive mode
cscope -d
```

#### cscope Capabilities
```bash
# Find symbol definitions
cscope -L -0 function_name

# Find functions calling this function
cscope -L -3 function_name

# Find text string
cscope -L -4 "error message"

# Find files including this file  
cscope -L -8 header.h

# Integration with vim
# Add to ~/.vimrc:
# set cscopetag
# cs add cscope.out

# Vim cscope commands:
:cs find s symbol_name      # Find symbol
:cs find g definition       # Find definition  
:cs find c function         # Find callers
:cs find f filename         # Find file
```

---

## Professional Best Practices

### Workflow Evolution

#### From MCU to Linux Development
**MCU Workflow:**
1. Open IDE → New Project
2. Configure chip settings
3. Write code in integrated editor
4. Click Build → Flash → Debug
5. Use hardware debugger/oscilloscope

**Linux Workflow:**
```bash
# 1. Project setup
mkdir project && cd project
git init
echo "build/" > .gitignore

# 2. Development cycle
vim src/main.c              # Edit code
make debug                  # Build
gdb ./build/app             # Debug
valgrind ./build/app        # Memory check
git commit -am "Feature X"  # Version control

# 3. Testing and deployment
make test                   # Run tests
make install               # Install system-wide
systemctl start myapp     # Service management
```

### Common Career Transition Challenges

#### Technical Challenges
1. **Overwhelming choice**: Too many tools, which to learn?
   - **Solution**: Start with essentials: vim, gcc, make, gdb
   - **Progress**: Add advanced tools gradually

2. **Complexity**: Multi-layered system vs. direct hardware
   - **Solution**: Learn abstractions layer by layer
   - **Progress**: Start with userspace, then kernel

3. **Debugging**: No oscilloscope, different mindset needed
   - **Solution**: Master software instrumentation tools
   - **Progress**: printf → gdb → valgrind → strace

4. **Performance**: Understanding system bottlenecks
   - **Solution**: Learn profiling tools and techniques
   - **Progress**: top → perf → custom benchmarks

#### Success Strategy
- **Master fundamentals first, specialize later**
- **Focus on understanding principles, not memorizing commands**
- **Use AI assistance for research, not as a substitute for learning**
- **Practice consistently with real projects**

---

# PHẦN II: EXAMPLE CODE

## Example: Config Parser

### File: config_parser.c
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Simple configuration parser with vulnerabilities
typedef struct {
    char app_name[16];      // DANGER: small buffer
    char log_level[8];      // DANGER: small buffer  
    char config_path[32];   // DANGER: small buffer
    int port_number;
} app_config_t;

// Unsafe function - buffer overflow vulnerability
void parse_config_line(app_config_t *config, char *line) {
    char key[64], value[256];
    
    // Parse "key=value" format
    if (sscanf(line, "%[^=]=%s", key, value) == 2) {
        printf("Parsing: %s = %s\n", key, value);
        
        // BUG 1: No bounds checking on strcpy!
        if (strcmp(key, "app_name") == 0) {
            strcpy(config->app_name, value);    // Dangerous!
        }
        else if (strcmp(key, "log_level") == 0) {
            strcpy(config->log_level, value);   // Dangerous!
        }
        else if (strcmp(key, "config_path") == 0) {
            strcpy(config->config_path, value); // Dangerous!
        }
        else if (strcmp(key, "port") == 0) {
            config->port_number = atoi(value);
        }
    }
}

// Another unsafe function - stack overflow
void process_user_input(void) {
    char input_buffer[64];                  // Small stack buffer
    char processed_data[64];
    
    printf("Enter configuration data: ");
    fgets(input_buffer, 256, stdin);        // BUG 2: reads more than buffer size!
    
    // BUG 3: No length validation
    strcpy(processed_data, input_buffer);   // Stack overflow potential!
    
    printf("Processed: %s", processed_data);
}

// Unsafe string concatenation
void build_log_message(char *user_data) {
    char log_buffer[128];
    char timestamp[] = "2024-02-24 10:30:45";
    
    // BUG 4: No size checking in sprintf
    sprintf(log_buffer, "[%s] User input: %s", timestamp, user_data);
    
    printf("Log: %s\n", log_buffer);
}

// Main application
int main() {
    printf("=== Configuration Parser Demo ===\n");
    printf("Warning: This code has intentional buffer overflows!\n\n");
    
    app_config_t config;
    memset(&config, 0, sizeof(config));
    
    // Test case 1: Normal usage
    printf("--- Test 1: Normal configuration ---\n");
    parse_config_line(&config, "app_name=MyApp");
    parse_config_line(&config, "log_level=INFO");
    parse_config_line(&config, "port=8080");
    
    printf("Config: %s, %s, port=%d\n", 
           config.app_name, config.log_level, config.port_number);
    
    // Test case 2: Trigger buffer overflow
    printf("\n--- Test 2: Long app name (will overflow!) ---\n");
    parse_config_line(&config, "app_name=VeryLongApplicationNameThatExceedsBufferSize");
    
    // Test case 3: Stack overflow
    printf("\n--- Test 3: Stack overflow test ---\n");
    char long_data[200];
    memset(long_data, 'A', 150);
    long_data[150] = '\0';
    build_log_message(long_data);
    
    // Test case 4: Interactive overflow
    printf("\n--- Test 4: Interactive input ---\n");
    printf("Try entering long strings to trigger buffer overflow\n");
    process_user_input();
    
    return 0;
}
```

## Example: File Processor

### File: file_processor.c
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// File processing application with memory leaks
typedef struct {
    char *filename;
    char *content;
    size_t size;
    int processed;
} file_info_t;

// BUG 1: Memory leak - never freed
char* load_config_file(const char* filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        return NULL;
    }
    
    // Allocate memory for config
    char *config = malloc(1024);
    if (!config) {
        fclose(file);
        return NULL;
    }
    
    fread(config, 1, 1024, file);
    fclose(file);
    
    return config;  // Memory never freed!
}

// BUG 2: Conditional memory leak
file_info_t* create_file_info(const char* filename) {
    file_info_t *info = malloc(sizeof(file_info_t));
    if (!info) {
        return NULL;
    }
    
    info->filename = malloc(strlen(filename) + 1);
    if (!info->filename) {
        // BUG: forget to free 'info' before return
        return NULL;  // Memory leak!
    }
    
    strcpy(info->filename, filename);
    info->content = NULL;
    info->size = 0;
    info->processed = 0;
    
    return info;
}

// BUG 3: Double allocation without freeing first
void update_file_content(file_info_t *info, const char *new_content) {
    if (!info) return;
    
    // BUG: Don't free existing content before new allocation
    info->content = malloc(strlen(new_content) + 1);
    strcpy(info->content, new_content);
    info->size = strlen(new_content);
}

// BUG 4: Incomplete cleanup
void cleanup_file_info(file_info_t *info) {
    if (!info) return;
    
    // Only free filename, forget content!
    free(info->filename);
    // BUG: Missing free(info->content)
    // BUG: Missing free(info)
}

// BUG 5: Loop with accumulating leaks
void process_multiple_files(char **filenames, int count) {
    for (int i = 0; i < count; i++) {
        char *config = load_config_file(filenames[i]);
        if (config) {
            printf("Loaded config from %s: %.50s...\n", filenames[i], config);
            // BUG: Never call free(config)
        }
        
        file_info_t *info = create_file_info(filenames[i]);
        if (info) {
            update_file_content(info, "Sample content for processing");
            printf("Processing file: %s\n", info->filename);
            // BUG: Never call cleanup_file_info(info)
        }
    }
    // All memory leaked at end of loop!
}

// Function that leaks memory when called multiple times
void log_message(const char *message) {
    static int call_count = 0;
    call_count++;
    
    // BUG 7: Allocate memory every call, never free
    char *log_entry = malloc(256);
    snprintf(log_entry, 256, "[%d] %s", call_count, message);
    
    printf("Log: %s\n", log_entry);
    // BUG: Never free log_entry
}

int main() {
    printf("=== File Processor with Memory Leaks ===\n");
    
    // Test 1: Simple memory leak
    printf("\n--- Test 1: Config loading leak ---\n");
    char *config1 = load_config_file("config1.txt");
    char *config2 = load_config_file("config2.txt");
    
    // Test 2: Conditional leak
    printf("\n--- Test 2: Conditional leak ---\n");
    file_info_t *info1 = create_file_info("");
    
    // Test 3: Valid file info but incomplete cleanup
    printf("\n--- Test 3: Incomplete cleanup ---\n");
    file_info_t *info2 = create_file_info("test.txt");
    if (info2) {
        update_file_content(info2, "Initial content");
        update_file_content(info2, "Updated content");
        cleanup_file_info(info2);
    }
    
    // Test 4: Multiple file processing
    printf("\n--- Test 4: Multiple file leaks ---\n");
    char *test_files[] = {"file1.txt", "file2.txt", "file3.txt"};
    process_multiple_files(test_files, 3);
    
    // Test 5: Repeated logging leaks
    printf("\n--- Test 5: Repeated allocation leaks ---\n");
    for (int i = 0; i < 5; i++) {
        log_message("Processing data");
    }
    
    printf("\nProgram completed. Check memory usage!\n");
    return 0;
}
```

## Example: Tool Comparison

### File: tool_comparison.c
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

// Test program to demonstrate different debugging tool behaviors
typedef struct {
    char buffer[64];
    int *dynamic_data;
    size_t data_size;
} test_context_t;

// Performance-sensitive function
void performance_critical_loop(int iterations) {
    printf("=== Performance Test: %d iterations ===\n", iterations);
    
    clock_t start = clock();
    
    for (int i = 0; i < iterations; i++) {
        char *temp = malloc(100);
        if (temp) {
            memset(temp, 0, 100);
            free(temp);
        }
    }
    
    clock_t end = clock();
    double cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Execution time: %.4f seconds\n", cpu_time);
}

// Memory-intensive function  
void memory_stress_test(int allocations) {
    printf("=== Memory Stress Test: %d allocations ===\n", allocations);
    
    char **pointers = malloc(allocations * sizeof(char*));
    
    // Allocate many small blocks
    for (int i = 0; i < allocations; i++) {
        pointers[i] = malloc(256);
        if (pointers[i]) {
            sprintf(pointers[i], "Block %d", i);
        }
    }
    
    // Free all blocks
    for (int i = 0; i < allocations; i++) {
        free(pointers[i]);
    }
    
    free(pointers);
    printf("Allocated and freed %d blocks\n", allocations);
}

// Edge case: Use after free
void use_after_free_test() {
    printf("=== Use After Free Test ===\n");
    
    char *buffer = malloc(100);
    strcpy(buffer, "Valid data");
    printf("Before free: %s\n", buffer);
    
    free(buffer);
    
    // Dangerous: use after free
    printf("After free: %s\n", buffer);
}

// Edge case: Double free
void double_free_test() {
    printf("=== Double Free Test ===\n");
    
    char *data = malloc(50);
    strcpy(data, "Test data");
    
    free(data);
    free(data);  // Double free - undefined behavior
}

int main() {
    printf("=== Debugging Tool Comparison Test Suite ===\n");
    printf("This program contains various scenarios to test ASan vs Valgrind\n\n");
    
    // Test 1: Performance impact comparison
    performance_critical_loop(10000);
    
    // Test 2: Memory overhead comparison  
    memory_stress_test(1000);
    
    // Test 3: Detection capabilities
    use_after_free_test();
    
    // Test 4: Error handling differences
    double_free_test();
    
    printf("\n=== Test suite completed ===\n");
    printf("Compare behavior under different debugging tools!\n");
    
    return 0;
}
```

---

# PHẦN III: BÀI THỰC HÀNH

## Bài 1: Buffer Overflow Detection với Address Sanitizer

### 🎯 Objective
Học cách sử dụng Address Sanitizer để detect và debug buffer overflow vulnerabilities trong C code.

### 📝 Source Code
Sử dụng file `config_parser.c` từ phần Example Code.

### 🔧 Compilation Commands
```bash
# Normal compilation
gcc -o config_normal config_parser.c

# Address Sanitizer compilation  
gcc -g -O1 -fsanitize=address -fno-omit-frame-pointer -o config_asan config_parser.c

# Run tests
./config_normal    # May crash silently or show corruption
./config_asan      # Will show detailed crash report
```

### 📊 Expected ASan Output
```
=================================================================
==12345==ERROR: AddressSanitizer: stack-buffer-overflow on address 0x7fff8f7e4d18
WRITE of size 185 at 0x7fff8f7e4d18 thread T0
    #0 0x7ee56c8ced68 in vsprintf 
    #1 0x7ee56c8d051b in sprintf 
    #2 0x557316f0d990 in build_log_message /config_parser.c:57
    #3 0x557316f0dc95 in main /config_parser.c:88

Address 0x7fff8f7e4d18 is located in stack of thread T0 at offset 224 in frame
    #0 0x557316f0d834 in build_log_message /config_parser.c:52

  This frame has 2 object(s):
    [32, 52) 'timestamp' (line 54)
    [96, 224) 'log_buffer' (line 53) <== Memory access at offset 224 overflows this variable

SUMMARY: AddressSanitizer: stack-buffer-overflow in vsprintf
```

### ✅ Expectations
1. **Tìm hiểu cách sử dụng Address Sanitizer để debug buffer overflow**
2. **Đọc và phân tích ASan error logs**
3. **Hiểu ASan mechanisms và compiler options**
4. **Sử dụng AI để research ASan hoạt động như thế nào**
5. **Fix vulnerabilities và verify bằng ASan**

### 🔨 Fix Example
```c
// Safe version of build_log_message
void build_log_message_safe(const char *user_data) {
    char log_buffer[128];
    char timestamp[] = "2024-02-24 10:30:45";
    
    // Use snprintf with size limit
    int result = snprintf(log_buffer, sizeof(log_buffer), 
                         "[%s] User input: %.80s", timestamp, user_data);
    
    if (result >= sizeof(log_buffer)) {
        printf("Warning: Log message was truncated\n");
    }
    
    printf("Log: %s\n", log_buffer);
}
```

---

## Bài 2: Memory Leak Detection với Valgrind

### 🎯 Objective
Học cách sử dụng Valgrind Memcheck để detect và analyze memory leaks trong C applications.

### 📝 Source Code
Sử dụng file `file_processor.c` từ phần Example Code.

### 🔧 Valgrind Commands
```bash
# Compilation for Valgrind
gcc -g -O0 -o file_processor file_processor.c

# Run with Valgrind
valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes ./file_processor

# Alternative: Quick leak check
valgrind --leak-check=yes ./file_processor
```

### 📊 Expected Valgrind Output
```
==297== Memcheck, a memory error detector
==297== Command: ./file_processor

=== File Processor with Memory Leaks ===
[program output...]

==297== HEAP SUMMARY:
==297==     in use at exit: 1,593 bytes in 19 blocks
==297==   total heap usage: 27 allocs, 8 frees, 8,158 bytes allocated
==297== 
==297== 1,280 bytes in 5 blocks are definitely lost in loss record 9 of 9
==297==    at 0x4846828: malloc 
==297==    by 0x1095C5: log_message file_processor.c:110
==297==    by 0x1097C4: main file_processor.c:158
==297== 
==297== LEAK SUMMARY:
==297==    definitely lost: 1,456 bytes in 11 blocks
==297==    indirectly lost: 137 bytes in 8 blocks
==297==      possibly lost: 0 bytes in 0 blocks
==297==    still reachable: 0 bytes in 0 blocks
==297== 
==297== ERROR SUMMARY: 6 errors from 6 contexts
```

### ✅ Expectations
1. **Tìm hiểu cách sử dụng Valgrind Memcheck để detect memory leaks**
2. **Đọc và phân tích Valgrind leak reports**
3. **Hiểu leak categories và heap summary information**
4. **Sử dụng AI để research memory management best practices**
5. **Fix memory leaks và verify bằng Valgrind**

### 🔨 Fix Example
```c
// Fixed version with proper cleanup
void cleanup_file_info_fixed(file_info_t *info) {
    if (!info) return;
    
    // Free all allocated memory
    free(info->filename);
    free(info->content);
    free(info);  // Don't forget the struct itself!
}

// Fixed log_message function
void log_message_fixed(const char *message) {
    static int call_count = 0;
    call_count++;
    
    char *log_entry = malloc(256);
    if (!log_entry) {
        printf("Memory allocation failed\n");
        return;
    }
    
    snprintf(log_entry, 256, "[%d] %s", call_count, message);
    printf("Log: %s\n", log_entry);
    
    free(log_entry);  // Essential: free the memory!
}
```

---

## Bài 3: Tool Deep Understanding

### 🎯 Objective
Hiểu sâu về cơ chế hoạt động, điểm mạnh/yếu và use cases của Address Sanitizer vs Valgrind.

### 📝 Source Code
Sử dụng file `tool_comparison.c` từ phần Example Code.

### 🔧 Comparison Tests
```bash
# Test with different tools
gcc -o tool_normal tool_comparison.c
gcc -g -O1 -fsanitize=address -o tool_asan tool_comparison.c  
gcc -g -O0 -o tool_debug tool_comparison.c

# Performance comparison
time ./tool_normal
time ./tool_asan  
time valgrind ./tool_debug

# Memory usage comparison
/usr/bin/time -v ./tool_normal
/usr/bin/time -v ./tool_asan
/usr/bin/time -v valgrind ./tool_debug
```

### ✅ Expectations
1. **Hiểu cơ chế hoạt động:**
   - ASan: Shadow memory và compile-time instrumentation
   - Valgrind: Dynamic binary translation và runtime analysis

2. **So sánh điểm mạnh/yếu:**
   - Performance impact, detection capabilities, memory overhead và platform limitations

3. **Khi nào dùng tool nào:**
   - ASan vs Valgrind use cases và trade-offs

4. **Limitations và gotchas:**
   - Practical constraints của mỗi tool

5. **Sử dụng AI để research mechanisms và best practices thay vì học commands**

### 📊 Research Questions for AI
```
Prompt examples:
1. "Explain the shadow memory mechanism in AddressSanitizer vs Valgrind's approach"
2. "When would you choose Valgrind over AddressSanitizer despite the performance penalty?"
3. "What are the fundamental architectural differences between compile-time and runtime memory debugging?"
4. "How does dynamic binary translation in Valgrind work and why is it slower?"
5. "What types of memory bugs can ASan detect that Valgrind cannot, and vice versa?"
```

---

## 🎯 TỔNG KẾT TRAINING

### Key Learning Outcomes
- **Mindset Transition**: Successful shift từ MCU sang Linux development paradigm
- **Tool Mastery**: Command line proficiency và debugging skills
- **Professional Skills**: AI-enhanced learning approach và best practices
- **Security Awareness**: Memory safety và defensive programming techniques

### Next Steps
1. **Practice Daily**: Sử dụng tools trong real projects
2. **Community Engagement**: Join Linux development communities
3. **Continuous Learning**: Keep updated với new tools và techniques
4. **AI Integration**: Leverage AI cho research và problem-solving

### Success Metrics
- Comfortable với command line workflow
- Proficient trong debugging với modern tools
- Understanding của Linux development ecosystem
- Ability to transition existing embedded knowledge sang Linux context

---

**©FPT SOFTWARE - Corporate Training Center - Internal Use**

*Training Duration: 60 hours / 20 sessions*  
*Target Audience: Embedded MCU developers transitioning to Linux*  
*Approach: AI-enhanced learning với hands-on practice*
