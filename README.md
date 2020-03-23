# System Call Hijacking Repositroy

This repository is a solution that I've made for the [system call hijacking](https://docs.google.com/document/d/1m6Q2RonmRrsPqhIq1AwfBJalbMzI4USGAQlgehz98rI/edit) tutorial.
The tutorial was divided into stages and therefore the answer as well:

| Stage | Filename | Estimated Time |
| ----- | -------- | -------------- |
| Hello Kernel | hello_kernel.c | 20min |
| SC Calling | get_uid.c | 40min |
| SC Hijacking | sys_call_hijack.c | 60min |

Total of 2 hours training.

## Hello Kernel Stage
At this stage simple kernel module is been written.

## SC Calling 
At this stage you have to call a syscall function from the kernel by finding the sys_call_table.

Notes: 
*  This is a bad practice - Don't do it in a real kernel module.
*  In order to find sys_call_table symbol you have to overcome KASLR.

## SC Hijacking
At this stage you place hook on a chosen syscall function.
I placed it on getuid function.

Notes:
*  Todo so you need to overcome Write Protect in the kernel.
*  To overcome the WR in kernel you cannot use the write_cr0 function.

## References
* [Linux Kernel Module Rootkit - Syscall Table Hijacking](https://medium.com/bugbountywriteup/linux-kernel-module-rootkit-syscall-table-hijacking-8f1bc0bd099c)
* [system call hijacking](https://docs.google.com/document/d/1m6Q2RonmRrsPqhIq1AwfBJalbMzI4USGAQlgehz98rI/edit)
* [Calling write_cr0 don't work stackoverflow](https://stackoverflow.com/questions/58512430/how-to-write-to-protected-pages-in-the-linux-kernel)