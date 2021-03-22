#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

MODULE_LICENSE("GPL");

static int paramInt;
static char *paramStr;
static int paramArr[3];
static int pr_arr;

module_param(paramInt, int, 0644);
module_param(paramStr, charp, 0);
module_param_array(paramArr, int, &pr_arr, 0444);

// Module 3
static int hello_proc_show(struct seq_file *m, void *v) {
  seq_printf(m, "Hello proc!\n");
  return 0;
}

// Module 3
static int hello_proc_open(struct inode *inode, struct  file *file) {
  return single_open(file, hello_proc_show, NULL);
}

// Module 3
static const struct proc_ops hello_proc_fops = {
    .proc_open = hello_proc_open,
    .proc_release = single_release,
    .proc_read = seq_read,
    .proc_lseek = seq_lseek
};

static int __init hello_init(void) {
    int i;

    // Module 1
    printk(KERN_INFO "Hello, world\n");

    // Module 2
    printk(KERN_INFO "Param-int:%d;\n", paramInt);
    printk(KERN_INFO "Param-str:%s;\n", paramStr);
    for (i = 0; i < (sizeof paramArr / sizeof (int)); i++) {
		printk(KERN_INFO "Param-arr[%d]: %d\n", i, paramArr[i]);
	}

    // Module 3
    proc_create("test", 0, NULL, &hello_proc_fops);
    printk(KERN_INFO "Test file created, see /proc/test\n");

    return 0;
}

static void __exit hello_exit(void) {
    // Module 1
    printk(KERN_INFO "Goodbye, world\n");

    // Module 3
    remove_proc_entry("test", NULL);
}

module_init(hello_init);
module_exit(hello_exit);