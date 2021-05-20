#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/mm.h>

// #include <linux/sched.h>
// #include <linux/mm.h>
// #include <asm/io.h>

/* Print all vma of the current process */
static void mtest_list_vma(void) {
    struct vm_area_struct *vma = current->mm->mmap;

    while (vma) {
        printk(KERN_INFO "VMA 0x%lx - 0x%lx\n", vma->vm_start, vma->vm_end);

        printk(KERN_INFO "%c\n%c\n%c\n\n",
            vma->vm_flags & VM_READ ? 'r' : '-',
            vma->vm_flags & VM_WRITE ? 'w' : '-',
            vma->vm_flags & VM_EXEC ? 'x' : '-');
        
        vma = vma->vm_next;
    }

}

/* Find va->pa translation */
static void mtest_find_page(unsigned long addr) {
    // TODO: Convert virtual memory to physical memory address and output
}

/* Write val to the specified address */
static void mtest_write_val(unsigned long addr, unsigned long val) {
    // TODO: Write val to addr
}

static ssize_t mtest_proc_write(struct file *file, const char __user * buffer, size_t count, loff_t * data) {
    char *tmp = kzalloc((count+1), GFP_KERNEL);
    // char tmp[128] = "";
    if (copy_from_user(tmp, buffer, count)) {
        kfree(tmp);
        return -EFAULT;
    }

    if (!memcmp(tmp, "listvma", 7)) {
        printk(KERN_INFO "listvma inputed\n");
        mtest_list_vma();
    } else if (!memcmp(tmp, "findpage", 8)) {
        printk(KERN_INFO "findpage inputed\n");
    } else if (!memcmp(tmp, "writeval", 8)) {
        printk(KERN_INFO "writeval inputed\n");
    }
    return count;
}

static struct file_operations proc_mtest_operations = {
    .owner      = THIS_MODULE,
    .write      = mtest_proc_write,
};

static struct proc_dir_entry *mtest_proc_entry;

static int __init mtest_init(void) {
    proc_create("mtest", 0777, NULL, &proc_mtest_operations);

    return 0;
}

static void __exit mtest_exit(void) {
    remove_proc_entry("mtest", NULL);
}

module_init(mtest_init);
module_exit(mtest_exit);

MODULE_LICENSE("GPL");
