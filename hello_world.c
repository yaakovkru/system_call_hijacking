/*
* hello_world.c - Simple hello world module
*/

#include <linux/module.h>
#include <linux/kernel.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yaakov K");
MODULE_DESCRIPTION("Basic Kernel coding for a beginner");
MODULE_VERSION("0.01");


int init_module(void)
{
	printk(KERN_INFO "Hello, Kernel!\n");
	return 0;
}


void cleanup_module(void)
{
	printk(KERN_INFO "Goodbye, Kernel... \n");
}