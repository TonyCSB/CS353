#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

// #include <linux/sched.h>
// #include <linux/mm.h>
// #include <asm/io.h>

/* Print all vma of the current process */
static void mtest_list_vma(void) {
    // TODO: Get all vma and output
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
    char *tmp = kzalloc((count + 1), GFP_KERNEL);
    if (copy_from_user(tmp, buffer, count)) {
        kfree(tmp);
        return -EFAULT;
    }
    if (strcmp(tmp, "listvma")) {
        printk("listvma");
    } else if (strcmp(tmp, "findpage")) {
        printk("findpage");
    } else if (strcmp(tmp, "writeval")) {
        printk("writeval");
    }
    return count;
}

static struct proc_ops proc_mtest_operations = {
    .proc_write     = mtest_proc_write
};

static struct proc_dir_entry *mtest_proc_entry;

static int __init mtest_init(void) {
    proc_create("mtest", 0777, NULL, &proc_mtest_operations);

    return 0;
}

static void __exit mtest_exit(void) {
    // TODO: Delete proc file
}

module_init(mtest_init);
module_exit(mtest_exit);

MODULE_LICENSE("GPL");
