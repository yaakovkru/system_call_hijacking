# System Call Hijacking Repositroy

This repository is kind of a solution for an excercise that helped me practice writing kernel modules.
Eventually that kernel module's purpose is to place hook on syscall functions inside the kernel.
Unfortunately, the tutorial that I've done is written in non-English language and therefore I won't attach it here.

The tutorial is divided into stages:

| Stage | Filename | Estimated Time |
| ----- | -------- | -------------- |
| Hello Kernel | hello_kernel.c | 20min |
| SC Calling | get_uid.c | 40min |
| SC Hijacking | sys_call_hijack.c | 60min |

Total of 2 hours training.

## Stages Explained
### Hello Kernel Stage
In this stage we practice on writing basic kernel module.
The module writes to the kernel log (dmesg) "Hello Kernel" when it is inserted.
Then at the removal it writes "Goodbye Kernel"

### SC Calling 
At this stage you gotta have a little more understanding in the kernel source code. Your mission is to call (within the kernel space) to a syscall function. (Yeah, I know that it is a bad practice which shouldn't be done at all).
To do so, You need to find the address of sys_call_table and then call to the desired syscall.

In my solution, I called to the getuid syscall.

**Challenges**
*  sys_call_table symobl is not exported therefore you got to find it.
*  The new kernel versions got KASLR which make it more challenging.

### SC Hijacking
At this stage you finally place hook on a chosen syscall function.
I've placed it on the "getuid" syscall function.

**Challenges:**
*  You cannot just modify the kernel code on the run, you gotta turn off the Write Protect flag.
*  To overcome the WR in kernel you cannot use the write_cr0 function.

## References
* [Linux Kernel Module Rootkit - Syscall Table Hijacking](https://medium.com/bugbountywriteup/linux-kernel-module-rootkit-syscall-table-hijacking-8f1bc0bd099c)
* [Calling write_cr0 don't work stackoverflow](https://stackoverflow.com/questions/58512430/how-to-write-to-protected-pages-in-the-linux-kernel)
