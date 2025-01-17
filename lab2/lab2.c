/*+
 * Module:  lab2.c
-*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>

#define NR_THREADS_LOC 0xc038b3a8

static struct task_struct *firstTask, *lastTask;

int my_read_proc(char *page, char **start, off_t fpos, int blen, int *eof, void *data) {
    int numChars = 0;

    if (fpos == 0) {
        // Get the number of running threads
        int *num_threads = (int*) NR_THREADS_LOC;

        // Write headers
        numChars += sprintf(page, "Number of running processes: %d\n", nr_running);
        numChars += sprintf(page + numChars, "Number of running threads: %d\n", *num_threads);
        numChars += sprintf(page + numChars, "PID\tUID\tNICE\n");

        // Find first task
        firstTask = &init_task;
        lastTask = firstTask;

        // Write first task
        if (firstTask->pid != 0) {
            numChars += sprintf(page + numChars, "%d\t%d\t%d\n", firstTask->pid, firstTask->uid, firstTask->nice);
        }

        // Advance to next task
        lastTask = lastTask->next_task;
    } else {
        // Check if the current task has already been processed
        if (lastTask == firstTask) {
            *eof = 0;
            *start = page;
            return 0;
        }

        if (lastTask->pid != 0) {
            // write task info for one task
            numChars += sprintf(page, "%d\t%d\t%d\n", lastTask->pid, lastTask->uid, lastTask->nice);
        }
        
        // Advance to next task
        lastTask = lastTask->next_task;
    }

    *eof = 1;
    *start = page;
    return numChars;
}

int init_module() {
    struct proc_dir_entry *proc_entry;
    proc_entry = create_proc_entry("lab2", 0444, NULL); // Create the proc entry

    // Check if the entry could not be created
    if (proc_entry == NULL) {
        remove_proc_entry("lab2", &proc_root);
        return -ENOMEM;
    }

    // Initialize the entry
    proc_entry->read_proc = my_read_proc;
    return 0;
}

void cleanup_module() {
    // Remove the module
    remove_proc_entry("lab2", &proc_root);
}
