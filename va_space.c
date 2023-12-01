//#include <stdio.h>
#include <unistd.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/mm_types.h>
#include <linux/unistd.h>
#include <linux/time.h>

void va_space(__pid_t);

int main(int argc, char* argv[])
{
    // __pid_t pid;
    // printk("Please enter a process ID");
    // scanf("%d", &pid);
    // va_space(pid);

    if(argc > 1){
        va_space(argc);
    }
    else
    {
        pr_err("ERROR: please provide a PID\n");
        panic("Terminating...\n");
    }

    
    return 0;
}

void va_space(__pid_t pid)
{
    struct mm_struct *adr_space;
    struct task_struct *task;
    struct vm_area_struct *vm_area;
    unsigned long size = 0;
    
    // Get the task struct for the PID entered
    task = pid_task(find_get_pid(pid), PIDTYPE_PID);
    if(task == NULL){
        printk("No process with PID %d found\n", pid);
    }

    // Get memory descriptor of the given process
    adr_space = task->mm;
    if(adr_space == NULL){
        printk("No memory descriptor found.");
    }

    // Go through the vma's to calculate size of virtual addressss space
    down_read(&adr_space->mmap_sem);
    for(vm_area = adr_space->mmap; vm_area; vm_area = vm_area->vm_next) {
        size += vm_area->vm_end - vm_area->vm_start;
    }
    up_read(&adr_space->mmap_sem);

    printk("Size of virtual address space for"
        " process(PID: %d): %lu bytes(?)", pid, size);

}

