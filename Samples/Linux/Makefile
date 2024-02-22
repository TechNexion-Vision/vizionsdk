# Makefile

# Compiler and flags setup
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

# Target name setup
TARGET = Sample

# Source files and object files setup
SRCS = main.cpp
OBJS = $(SRCS:.cpp=.o)

# Setup link libraries and pthread
LDLIBS = -ldl -pthread 
# Target: Generate executable file
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS) 

# Compile all .cpp files into .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Install target to specified directory
install: $(TARGET)
	install -m 755 $< $(DESTDIR)$(PREFIX)/bin

# Clean intermediate files and executable file
clean:
	rm -f $(OBJS) $(TARGET)

# Ensure that generated intermediate files and executable file are not treated as real files
.PHONY: all clean install
