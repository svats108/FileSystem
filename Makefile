# Compiler and Flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

# Name of the final program
TARGET = fsystem

# Directories
SRCDIR = src
INCDIR = include
OBJDIR = obj

# Files
# Find all .cpp files in the src/ directory
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
# Create a list of .o (object) files in the obj/ directory
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

# Default Rule: Build the target
all: $(TARGET)

# Link the final executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "Build successful! Run with ./$(TARGET)"

# Compile .cpp files into .o files
# The | $(OBJDIR) ensures the directory exists before compiling
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create the object directory if it doesn't exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Clean up all build files
clean:
	rm -rf $(OBJDIR) $(TARGET)
	@echo "Cleanup complete."

# Declare phony targets (names that aren't actual files)
.PHONY: all clean