//#include <stdio.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/mm_types.h>
#include <linux/unistd.h>
#include <linux/time.h>
#include <linux/module.h>

//void va_space(pid_t);

static int pid = 1866;
module_param(pid, int, 0);

//MODULE_PARAM_DESC(pid, "Process ID for the module.");

// static int __init module_init(void)
// {
//     printk(KERN_INFO "initializing...\n");
//     return 0;
// }

// int va_space(void)
// {
//     // __pid_t pid;
//     // printk(KERN_INFO(KERN_INFO("Please enter a process ID");
//     // scanf("%d", &pid);
//     // va_space(pid);

//     if(argc > 1){
//         va_space(argc);
//     }
//     else
//     {
//         pr_err("ERROR: please provide a PID\n");
//         panic("Terminating...\n");
//     }

    
//     return 0;
// }

void va_space(void)
{
    struct mm_struct *adr_space;
    struct task_struct *task;
    struct vm_area_struct *vm_area;
    unsigned long size = 0;
    
    // Get the task struct for the PID entered
    task = pid_task(find_vpid(pid), PIDTYPE_PID);
    if(task == NULL){
        printk(KERN_INFO "No process with PID %d found\n", pid);
    }

    // Get memory descriptor of the given process
    adr_space = task->mm;
    if(adr_space == NULL){
        printk(KERN_INFO"No memory descriptor found.");
    }

    // Go through the vma's to calculate size of virtual addressss space
    down_read(&adr_space->mmap_sem);
    for(vm_area = adr_space->mmap; vm_area; vm_area = vm_area->vm_next) {
        size += vm_area->vm_end - vm_area->vm_start;
    }
    up_read(&adr_space->mmap_sem);

    printk(KERN_INFO "Size of virtual address space for"
        " process(PID: %d): %lu bytes(?)", pid, size);

}

static void __exit clean_exit(void)
{
    printk(KERN_INFO "exiting. Goodbye!\n");
    return 0;
}

module_init(va_space);
module_exit(clean_exit)