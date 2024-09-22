obj-m += mycar.o

KBUILD_CFLAGS   += -Werror=incompatible-pointer-types

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	#make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	ls | grep -vE "Makefile|mycar.c|mycar.ko" | xargs rm -rf

rmmod:
	rmmod mycar.ko

insmod:
	dmesg -c
	dmesg -c
	insmod mycar.ko

