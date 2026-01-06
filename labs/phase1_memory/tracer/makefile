CC = gcc

CFLAGS = -Wall -Wextra -Wpedantic -Werror \
		 -std=c11 -O2 -g \
		 -D_POSIX_C_SOURCE=200809L

LDFLAGS=

LDLIBS=

TARGET = test_driver
OBJS = test_driver.o tracked_malloc.o

all: $(TARGET)


$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -f $(TARGET) $(OBJS)


.PHONY: all clean
	

