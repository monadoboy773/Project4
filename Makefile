# CC=gcc
# CFLAGS=-I. -Wall
# DEPS=list.h
# OBJ= va_space.o list.o

# %.o: %.c $(DEPS)
# 	$(CC) -c -o $@ $< $(CFLAGS)

# list_test: $(OBJ)
# 	$(CC) -o $@ $^ $(CFLAGS)

# clean: 
# 	rm -f list_test
obj-m += va_space.o

PWD := $(CURDIR)

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean