# Define the object file and module
obj-m += lkm_example.o

# Targets
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

test:
	-sudo rmmod lkm_example
	sudo dmesg -C
	sudo insmod lkm_example.ko
	-sudo dmesg || true
