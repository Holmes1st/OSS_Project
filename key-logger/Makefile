FLAGS=-std=gnu99
ccflags-y:=-std=gnu99 -Wno-declaration-after-statement
ifneq ($(KERNELRELEASE),)
obj-m := klogger.o

else
KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
ECHO := $(shell pwd)/test

all: echo_server
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

echo_server: ./test/var.c ./test/echo_server.c
	gcc -std=c99 $^ -o $@
	ln -sf $(PWD)/$@ /tmp

endif

clean:
	rm -rf *.o *~ *.ko *.mod.c Module.* *.order echo_server .tmp* .*.cmd
	rm -f /tmp/echo_server
