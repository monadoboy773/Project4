#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/pid.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/mm_types.h>
#include <linux/unistd.h>
#include <linux/module.h>
#include <linux/highmem.h>

MODULE_LICENSE("GPL");

// Initial PID and address
static int pid = 0;
static char *address_str = "0x000000";

// Module params for passing command line arguments
module_param(pid, int, 0);
module_param(address_str, charp, 0000);

// Method to check if the address is in memory
int check_address_in_memory(struct task_struct *task, unsigned long addr) {
    pgd_t *pgd;
    pud_t *pud;
    pmd_t *pmd;
    pte_t *pte;
    spinlock_t *lock;

    pgd = pgd_offset(task->mm, addr);
    if(pgd_none(*pgd) || pgd_bad(*pgd)) {
        return 0;
    }

    pud = pud_offset(pgd, addr);
    if(pud_none(*pud) || pud_bad(*pud)) {
        return 0;
    }

    pmd = pmd_offset(pud, addr);
    if(pmd_none(*pmd) || pmd_bad(*pmd)) {
        return 0;
    }

    pte = pte_offset_map_lock(task->mm, pmd, addr, &lock);
    if(!pte_present(*pte)) {
        pte_unmap_unlock(pte, lock);
        return 0;
    }

    pte_unmap_unlock(pte, lock);
    return 1;
}

// Module initialization function
static int __init va_status_init(void) {
    struct task_struct *task;
    unsigned long address;
    int convert_err = kstrtoul(address_str, 0, &address);


    task = pid_task(find_vpid(pid), PIDTYPE_PID);
    if(task == NULL) {
        printk(KERN_INFO "No process with PID %d found\n", pid);
        return -ESRCH;
    }

    // Convert virtual address to long from a string
    if(convert_err) {
        printk(KERN_INFO "Error converting address to unsigned long (ERROR: %d)\n", convert_err);
        return convert_err;
    }

    // Check if the virtual address is in memory
    if(check_address_in_memory(task, address)) {
        printk(KERN_INFO "Virtual address %lx is in memory\n", address);
    } else {
        printk(KERN_INFO "Virtual address %lx is on disk\n", address);
    }

    return 0;
}

// Module exit function
static void __exit va_status_exit(void) {
    printk(KERN_INFO "VA status module unloaded\n");
}

// Initialize the modules
module_init(va_status_init);
module_exit(va_status_exit);

