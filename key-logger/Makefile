ifneq ($(KERNELRELEASE),)
obj-m := klogger.o
else
KERNELDIR ?= /lib/modules/$(shell uname -r)/build	# 커널 디렉토리
PWD := $(shell pwd)

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules		# 커널 디렉토리에서 작업하여 결과는 현재 디렉토리에 저장
endif

clean:
	rm -rf *.o *~ *.ko *.mod.c Module.* *.order
