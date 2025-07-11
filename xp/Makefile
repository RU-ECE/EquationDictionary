CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra

TARGET = eqdlatex
SOURCE = eqdlatex.cpp

$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCE)

clean:
	rm -f $(TARGET) *.aux *.log *.out *.pdf

.PHONY: clean 