CC = gcc

TARGET = test
FLAGS = -Wall -Wextra
SOURCES = src/main.c src/parser.c src/utils.c

$(TARGET): $(SOURCES)
	$(CC) $(FLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET)
