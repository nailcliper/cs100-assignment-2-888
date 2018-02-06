CC = g++
CCFLAGS = -Wall -Werror -ansi -pedantic
TARGET = rshell.out

SRC = src
HEADER = header

SOURCE=$(wildcard $(SRC)/*.cpp)
OBJECT=$(patsubst %,$(HEADER)/%, $(notdir $(SOURCE:.cpp=.o)))

$(TARGET) : $(OBJECT)
	@echo "Linking..."
	$(CC) -o $@ $^

$(HEADER)/%.o : $(SRC)/%.cpp
	@echo "Compiling..."
	$(CC) $(CCFLAGS) -c $< -o $@

clean:
	@echo "Cleaning..."
	rm -f $(TARGET)
	rm -f $(OBJECT)
