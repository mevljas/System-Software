obj-m := timer_tsc.o

all:
	make -C /usr/src/WSL2-Linux-Kernel-$(shell uname -r) M=$(shell pwd) modules
clean:
	make -C /usr/src/WSL2-Linux-Kernel-$(shell uname -r) M=$(shell pwd) clean
