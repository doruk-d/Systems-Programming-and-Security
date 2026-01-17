# lang rules
CC:= gcc
CFLAGS:= -Wall -Wextra -Werror -Wpedantic -g \
	-Wno-unused-parameter \
	-D_POSIX_C_SOURCE=200809L \
	-O2 \
	-std=c11

DEPFLAGS:= -MMD -MP
VFLAGS:= --leak-check=full --show-leak-kinds=all --track-origins=yes

# files
TARGET:= my_malloc
BUILD_DIR:= build


SRC:= my_malloc.c test_driver.c
OBJS:= $(SRC:%.c=$(BUILD_DIR)/%.o)
DEPS:= $(OBJS:%.o=%.d)

#rules
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $^ -o $@ 

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(DEPFLAGS) $< -c -o $@ 

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


-include $(DEPS)


.PHONY: all test valgrind clean
