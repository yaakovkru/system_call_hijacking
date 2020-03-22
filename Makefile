KMODULE := hello_world.ko
KDIR ?= /lib/modules/`uname -r`/build

.PHONY: all clean install uninstall test vim-setup
all:
	$(MAKE) -C ${KDIR} M=$(PWD)

install:
	sudo insmod ${KMODULE}

uninstall:
	sudo rmmod ${KMODULE}

vim-setup:
	sudo $(MAKE) O=. ARCH=x86 -C ${KDIR} M=$(PWD) cscope tags

test: install uninstall 
	dmesg | tail -5

clean: 
	$(MAKE) -C ${KDIR} M=$(PWD) clean


