#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kallsyms.h>

MODULE_LICENSE("GPL");

static unsigned long *syscall_table;
typedef long(sys_clone) (unsigned long, unsigned long, int __user *, int __user *, unsigned long);
static sys_clone *old_clone;
static long orig_cr0 = 0;

// src: https://stackoverflow.com/questions/58512430/how-to-write-to-protected-pages-in-the-linux-kernel/60564037#60564037
inline void mywrite_cr0(unsigned long cr0) {
  asm volatile("mov %0,%%cr0" : "+r"(cr0));
}

// src: https://infosecwriteups.com/linux-kernel-module-rootkit-syscall-table-hijacking-8f1bc0bd099c
#define unprotect_memory() \
({ \
	orig_cr0 =  read_cr0();\
	mywrite_cr0(orig_cr0 & (~ 0x10000)); /* Set WP flag to 0 */ \
});
#define protect_memory() \
({ \
	mywrite_cr0(orig_cr0); /* Set WP flag to 1 */ \
});

asmlinkage long sys_clone_hook(unsigned long x1, unsigned long x2, int __user *x3, int __user *x4, unsigned long x5) {
	long ret_val = old_clone(x1, x2, x3, x4, x5);
	printk(KERN_INFO "hello, I have hacked this syscall");
	return ret_val;
}

static int __init syscall_hack_init(void) {
    // Src: https://stackoverflow.com/questions/2103315/linux-kernel-system-call-hooking-example
    syscall_table = (void *) kallsyms_lookup_name("sys_call_table");
    if (!syscall_table) {
        printk(KERN_ERR "Syscall_table not found");
        return -1;
    }

    old_clone = (sys_clone *)syscall_table[__NR_clone];

    unprotect_memory();
    syscall_table[__NR_clone] = (unsigned long)sys_clone_hook;
    protect_memory();
    
    return 0;
}

static void __exit syscall_hack_exit(void) {
    unprotect_memory();
    syscall_table[__NR_clone] = (unsigned long)old_clone;
    protect_memory();
}

module_init(syscall_hack_init);
module_exit(syscall_hack_exit);
