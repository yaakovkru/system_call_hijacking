/*
* sys_call_hijack.c - Puts hook on some syscalls
*/

#include <asm/cacheflush.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <asm/unistd.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kooker");
MODULE_DESCRIPTION("Places Kernel hooks");
MODULE_VERSION("1.0.0");


typedef long (*sys_call_func_ptr_t)(void);

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
static sys_call_func_ptr_t g_original_func_ptr = 0;


static void turn_off_write_protect(void)
{
	/* Write protect flag is the 16th bit in CR0 register */
	write_cr0(read_cr0() & (~0x10000));
}

static void turn_on_write_protect(void)
{
	/* Write protect flag is the 16th bit in CR0 register */
	write_cr0(read_cr0() | (~0x10000));
}

static long sc_getuid_hook(void)
{
	long retval = g_original_func_ptr();
	printk(KERN_INFO "getuid() -> %lu\n", retval);
	return retval;
}

static void insert_hook(void)
{
	g_original_func_ptr = sc_table_address[__NR_getuid];
	turn_off_write_protect();
	sc_table_address[__NR_getuid] = (void *)sc_getuid_hook;
	turn_on_write_protect();
}

static void remove_hook(void)
{

	turn_off_write_protect();
	sc_table_address[__NR_getuid] = (void *)g_original_func_ptr;
	turn_on_write_protect();
}

int init_module(void)
{
	printk(KERN_INFO "Hello Kernel\n");
	printk(KERN_INFO "Inserting Hooks...\n");
	insert_hook();
	printk(KERN_INFO "Hook was inserted!\n");
	return 0;
}

void cleanup_module(void)
{
	printk(KERN_INFO "Removing hook\n");
	remove_hook();
	printk(KERN_INFO "Goodbye, Kernel... \n");
}
