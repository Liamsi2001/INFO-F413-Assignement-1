CXX = g++
CXXFLAGS = -std=c++11 -I"C:/Program Files/Python312/include"
LDFLAGS = -L"C:/Program Files/Python312/libs" -lpython312
TARGET = select  # Name of the main executable
TEST_TARGET = test_select  # Target for the test executable
PYTHON = python
CSV_FILES = quick_selection_data.csv lazy_selection_data.csv
PNG_FILES = comparison_plots.png time_comparison_plots.png

# Rule to compile the main program including both quickselect and lazyselect
$(TARGET): main.cpp quickselection.cpp quickselection.hpp lazyselection.cpp lazyselection.hpp
	$(CXX) $(CXXFLAGS) main.cpp quickselection.cpp lazyselection.cpp -o $(TARGET)

# Rule to compile the test program
$(TEST_TARGET): test.cpp quickselection.cpp quickselection.hpp lazyselection.cpp lazyselection.hpp
	$(CXX) $(CXXFLAGS) test.cpp quickselection.cpp lazyselection.cpp -o $(TEST_TARGET)

# Rule to run the main program and generate CSV
data: $(TARGET)
	./$(TARGET)

# Rule to run the test program
test: $(TEST_TARGET)
	./$(TEST_TARGET)

# Rule to run the Python script for plotting
plot: $(CSV_FILES)
	$(PYTHON) plot_comparisons.py

# Rule to clean up (optional)
clean:
	rm -f $(TARGET) $(TEST_TARGET) $(CSV_FILES) $(PNG_FILES)

# Default rule (runs everything)
all: $(TARGET) plot