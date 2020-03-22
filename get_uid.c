/*
* get_uid.c - Calls the getuid system call from the kernel.
*/

#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <asm/unistd.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yaakov K");
MODULE_DESCRIPTION("Returns the UID of the kernel which is 0");
MODULE_VERSION("0.03");


/* 
 To get this address I've executed:
  	"sudo cat /proc/kallsyms | grep sys_call_table"
 Or
	"sudo cat /boot/System.map-`uname -r`  | grep sys_call_table"
 The information about these files can be found in the tutorial.
 Note: 
	   * The "/proc/kallsyms" is the new kernel version of "/proc/ksyms".
	   * System.map is the same as kallysms only when KASLR (Kernel ASLR) is disabled.
*/
static void ** sc_table_address = (void **)0xffffffff820002a0;


/* Declaration of the time syscall as it is written in the man 2 getuid page */
typedef uid_t (*sc_getuid_func_ptr)(void);


static unsigned int sc_getuid(void)
{
	sc_getuid_func_ptr getuid_func = sc_table_address[__NR_getuid];
	return (unsigned int)getuid_func();
}

int init_module(void)
{
	printk(KERN_INFO "UID=%u\n", sc_getuid());
	return 0;
}


void cleanup_module(void)
{
	printk(KERN_INFO "Goodbye, Kernel... \n");
}
