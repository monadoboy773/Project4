#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/mm_types.h>
#include <linux/unistd.h>
#include <linux/module.h>
#include <linux/highmem.h>

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

    pud = pud_offset(task->mm, addr);
    if(pud_none(*pud) || pud_bad(*pud)) {
        return 0;
    }

    pmd = pmd_offset(task->mm, addr);
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
    int pid = 0; //PLACEHOLDER, ADD PROPER INPUT LATER
    char *address_str = "0x000000"; //PLACEHOLDER

    // Convert virtual address to long from a string
    kstroul(address_str, 0, &address);

    task = pid_task(find_vpid(pid), PIDTYPE_PID);
    if(task == NULL) {
        pr_info("No process with PID %d found\n", pid);
        return -EINVAL;
    }

    // Check if the virtual address is in memory
    if(check_address_in_memory(task, address)) {
        pr_info("Virtual address %lx is in memory\n", address);
    } else {
        pr_info("Virtual address %lx is NOT in memory\n", address);
    }

    return 0;
}

// Module exit function
static void __exit va_status_exit(void) {
    pr_info("VA status module unloaded\n");
}

// Initialize the modules
module_init(va_status_init);
module_exit(va_status_exit);