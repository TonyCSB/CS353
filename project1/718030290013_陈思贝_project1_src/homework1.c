#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");

static int paramInt;
static char *paramStr;
static int paramArr[3];
static int pr_arr;

module_param(paramInt, int, 0644);
module_param(paramStr, charp, 0);
module_param_array(paramArr, int, &pr_arr, 0444);

static int __init hello_init(void) {
    int i;

    printk(KERN_INFO "Hello, world\n");
    printk(KERN_INFO "Param-int:%d;\n", paramInt);
    printk(KERN_INFO "Param-str:%s;\n", paramStr);
    for (i = 0; i < (sizeof paramArr / sizeof (int)); i++) {
		printk(KERN_INFO "Param-arr[%d]: %d\n", i, paramArr[i]);
	}
    return 0;
}

static void __exit hello_exit(void) {
    printk(KERN_INFO "Goodbye, world\n");
}

module_init(hello_init);
module_exit(hello_exit);