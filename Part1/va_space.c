#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/mm_types.h>
#include <linux/unistd.h>
#include <linux/time.h>
#include <linux/module.h>

// Initializes pid;
static int pid = 0;
module_param(pid, int, 0);


static int __init va_space(void)
{
    struct mm_struct *adr_space;
    struct task_struct *task;
    struct vm_area_struct *vm_area;
    unsigned long size = 0;
    unsigned long size_kb = 0;


    printk(KERN_INFO "Loading module...\n");


    // Get the task struct for the PID entered
    task = pid_task(find_vpid(pid), PIDTYPE_PID);
    if(task == NULL){
        printk(KERN_INFO "No process with PID %d found\n", pid);
        return -ESRCH;
    }


    // Get memory descriptor of the given process
    adr_space = task->mm;
    if(adr_space == NULL){
        printk(KERN_INFO"No memory descriptor found.\n");
        return -EFAULT;
    }


    // Go through the vma's to calculate size of virtual addressss space
    // Converts bytes to KB
    down_read(&adr_space->mmap_sem);
    for(vm_area = adr_space->mmap; vm_area; vm_area = vm_area->vm_next) {
        size += vm_area->vm_end - vm_area->vm_start;
    }
    size_kb = size/1024;
    up_read(&adr_space->mmap_sem);


    printk(KERN_INFO "Size of virtual address space for "
        "process(PID: %d): %lu bytes(%luK)\n", pid, size, size_kb);
      
    
    return 0;
}

static void __exit clean_exit(void)
{
    printk(KERN_INFO "Exiting. Goodbye!\n"); 
}

module_init(va_space);
module_exit(clean_exit);

MODULE_LICENSE("GPL");