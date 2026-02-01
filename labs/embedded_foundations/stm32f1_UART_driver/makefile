CC = arm-none-eabi-gcc


CFLAGS = -mcpu=cortex-m3 -mthumb -Wall -O0 -g -MMD -MP
LDFLAGS = -T linker.ld -nostdlib


BUILD_DIR = build


TARGET = $(BUILD_DIR)/output.elf
SRCS = startup.c uart.c main.c 
OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)
DEPS = $(OBJS:.o=.d)


all: $(TARGET)


$(TARGET): $(OBJS) | $(BUILD_DIR)
	$(CC) $(LDFLAGS) $^ -o $@


$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@


-include $(DEPS)


$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)


clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean

