/*
* sys_call_hijack.c - Puts hook on syscall
*/

#include <asm/cacheflush.h>
#include <linux/module.h>
#include <linux/kallsyms.h>
#include <linux/kernel.h>
#include <asm/unistd.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yaakov Kr");
MODULE_DESCRIPTION("Places hooks on the syscalls in the kernel");
MODULE_VERSION("0.0.4");


typedef long (*sys_call_func_ptr_t)(void);

static void ** g_sys_call_table = 0;
static sys_call_func_ptr_t g_original_func_ptr = 0;
static unsigned long g_original_cr0;

/* 
	The orginal write_cr0 is protected and well guarded
    This trick helps me overcome this probelm
*/
static inline void fast_write_cr0(unsigned long cr0)
{
	asm volatile("mov %0,%%cr0" : "+r"(cr0), "+m"(__force_order));
}

static void turn_off_write_protect(void)
{
	g_original_cr0 = read_cr0();
	/* Write protect flag is the 16th bit in CR0 register */
	fast_write_cr0(g_original_cr0 & (~0x10000));
}

static void turn_on_write_protect(void)
{
	/* Write protect flag is the 16th bit in CR0 register */
	fast_write_cr0(g_original_cr0);
}

asmlinkage long sc_getuid_hook(void)
{
	long retval = g_original_func_ptr();
	printk(KERN_INFO "getuid() -> %lu\n", retval);
	return retval;
}

static void insert_hook(void)
{
	/* 
		Finding the sys_call_table
		Note: this overcomes the KASLR protection
	*/
	g_sys_call_table = (void **) kallsyms_lookup_name("sys_call_table");
	g_original_func_ptr = (sys_call_func_ptr_t) g_sys_call_table[__NR_getuid];

	/* Writing the hook */
	turn_off_write_protect();
	g_sys_call_table[__NR_getuid] = (void *)sc_getuid_hook;
	turn_on_write_protect();
}

static void remove_hook(void)
{
	turn_off_write_protect();
	g_sys_call_table[__NR_getuid] = (void *)g_original_func_ptr;
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
