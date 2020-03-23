KMODULE := sys_call_hijack.ko
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

test: all install uninstall 
	dmesg | tail -10

clean: 
	$(MAKE) -C ${KDIR} M=$(PWD) clean


