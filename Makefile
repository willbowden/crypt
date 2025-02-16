COMPILER = gcc
OPTIONS = -Wall -Werror -pedantic -ansi -fsanitize=address -lSDL2 -lSDL2_image -MMD -MP -I $(INCLUDE)
COMPILE = $(COMPILER) $(OPTIONS)

SRC = src
BUILD = build
BIN = bin
INCLUDE = include

SOURCES = $(wildcard $(SRC)/*.c)
OBJECTS = $(patsubst $(SRC)/%.c,$(BUILD)/%.o,$(SOURCES))
DEPS = $(OBJECTS:.o=.d)

TARGET = $(BIN)/crypt

$(shell mkdir -p $(BUILD) $(BIN))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(COMPILE) $(OBJECTS) -o $(TARGET)

$(BUILD)/%.o: $(SRC)/%.c
	$(COMPILE) -c $< -o $@

-include $(DEPS)

clean:
	rm -rf $(BUILD) $(BIN)

.PHONY: all clean