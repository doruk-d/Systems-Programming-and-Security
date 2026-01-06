# lang rules
CC= gcc
CFLAGS= -Wall -Wextra -Werror -Wpedantic -g \
	-Wno-unused-parameter \
	-D_POSIX_C_SOURCE=200809L \
	-O2 \
	-std=c11

VFLAGS= --leak-check=full --show-leak-kinds=all --track-origins=yes

# files
TARGET= my_malloc
BUILD_DIR= build


SRC= $(wildcard *.c)
OBJS= $(SRC:%.c=$(BUILD_DIR)/%.o)

#rules
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^


$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# mkdir step
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# utilities
test: $(TARGET)
	./$(TARGET)


valgrind: $(TARGET)
	valgrind $(VFLAGS) ./$(TARGET)


clean:
	rm -rf $(TARGET) $(BUILD_DIR)


.PHONY: all test valgrind clean
