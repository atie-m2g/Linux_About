obj-m += mycar.o

KVER := $(shell uname -r)
KDIR := /lib/modules/$(KVER)/build

KBUILD_CFLAGS   += -Werror=incompatible-pointer-types

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	#make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	ls | grep -vE "Makefile|mycar.c|mycar.ko" | xargs rm -rf

rmmod:
	rmmod mycar.ko

insmod:
	dmesg -c
	dmesg -c
	sudo insmod mycar.ko

