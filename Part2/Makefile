KERNEL_DIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

obj-m += va_status.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean