#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static int __init hello_init(void) {
    return 0;
}

static void __exit hello_exit(void) {
    return 0;
}

module_init(hello_init);
module_exit(hello_exit);