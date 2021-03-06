#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/mm.h>

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

static struct page *find_page(unsigned long addr) {
    struct mm_struct *mm = current->mm;

    pgd_t *pgd = pgd_offset(mm, addr);
    pud_t *pud = NULL;
    pmd_t *pmd = NULL;
    pte_t *pte = NULL;
    struct page *page = NULL;

    if (pgd_none(*pgd) || pgd_bad(*pgd)) {
        return NULL;
    }
    pud = pud_offset(pgd, addr);

    if (pud_none(*pud) || pud_bad(*pud)) {
        return NULL;
    }
    pmd = pmd_offset(pud, addr);

    if (pmd_none(*pmd) || pmd_bad(*pmd)) {
        return NULL;
    }
    pte = pte_offset_map(pmd, addr);

    if (pte_none(*pte) || !pte_present(*pte)) {
        return NULL;
    }

    page = pte_page(*pte);
    if (!page) {
        return NULL;
    }
    return page;
}

/* Find va->pa translation */
static void mtest_find_page(unsigned long addr) {
    struct page *page = find_page(addr);
    unsigned long *pma;

    if (page == NULL) {
        printk(KERN_ERR "Failed to find a translation for 0x%lx", addr);
    } else {
        pma = (unsigned long *)page_address(page) + (addr & (~PAGE_MASK));
        printk(KERN_INFO "vma 0x%lx -> pma 0x%lx\n", addr, (unsigned long)pma);
    }
}

/* Write val to the specified address */
static void mtest_write_val(unsigned long addr, unsigned long val) {
    struct vm_area_struct *vma = find_vma(current->mm, addr);
    struct page *page = find_page(addr);
    unsigned long *pma;

    if (!vma) {
        printk(KERN_ERR "Invalid vma");
        return;
    }

    if (!(vma->vm_flags & VM_READ)) {
        printk(KERN_INFO "VMA not writable");
        return;
    }

    if (!page) {
        printk(KERN_INFO "Page not exist");
        return;
    }

    pma = (unsigned long *)page_address(page) + (addr & (~PAGE_MASK));
    *pma = val;
    printk("written 0x%lx to address 0x%lx\n", val, (unsigned long)pma);
}

static ssize_t mtest_proc_write(struct file *file, const char __user * buffer, size_t count, loff_t * data) {
    char *tmp = kzalloc((count+1), GFP_KERNEL);
    unsigned long addr;
    unsigned long val;

    if (copy_from_user(tmp, buffer, count)) {
        kfree(tmp);
        return -EFAULT;
    }

    if (!memcmp(tmp, "listvma", 7)) {
        printk(KERN_INFO "listvma inputted\n");
        mtest_list_vma();
    } else if (!memcmp(tmp, "findpage", 8)) {
        printk(KERN_INFO "findpage inputted\n");
        if (sscanf((tmp + 8), "%lx", &addr) == 1) {
            mtest_find_page(addr);
        }
    } else if (!memcmp(tmp, "writeval", 8)) {
        printk(KERN_INFO "writeval inputted\n");
        if (sscanf((tmp + 8), "%lx %lx", &addr, &val) == 2) {
            mtest_write_val(addr, val);
        }
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
