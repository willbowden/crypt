COMPILER = gcc
OPTIONS = -Wall -Werror -ansi -pedantic -MMD -MP -I $(INCLUDE)
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
	$(COMPILE) $(OBJECTS) -o $(TARGET) -lSDL2 -lSDL2_image

$(BUILD)/%.o: $(SRC)/%.c
	$(COMPILE) -c $< -o $@

-include $(DEPS)

clean:
	rm -rf $(BUILD) $(BIN)

.PHONY: all clean