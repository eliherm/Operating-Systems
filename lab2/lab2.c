/*+
 * Module:  lab2.c
 *
 * Purpose: Skeleton solution to ELEC 377 Lab2.
 *
-*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>

static struct task_struct * firstTask, *lastTask;

// int cnt;

int my_read_proc(char * page, char **start, off_t fpos, int blen, int * eof, void * data) {
  int numChars;
  if (fpos > 0) {
    numChars = 0
  } else {
    numChars = sprintf(page, "Hello World\n");
  }

  return numChars;

/*
    int numChars;
    if (fpos == 0) {
      // write headers
      sprintf(page, "Hello World\n");

	    // find first task
        // write first task
        // advance to next task
    } else {
        if (at back at begining of list) {
            *eof = 0;
            *start = page;
            return 0;
        }
	    // write task info for one task
	    // advance to next task
    }
    *eof = 1;
    *start = page;
    return numChars;
    */
}

int init_module() {
   struct proc_dir_entry * proc_entry;
   proc_entry = create_proc_entry("lab2", 0444, NULL); // Create the proc entry

   // Check if the entry could not be created
   if (proc_entry == NULL) {
     remove_proc_entry("lab2", &proc_root);
     return -ENOMEM;
   }

   // Initialize the entry
   proc_entry->read_proc = my_read_proc;

   /*
   proc_entry->owner = THIS_MODULE;
   proc_entry->mode = S_IFREG | S_IRUGO;
   proc_entry->uid = 0
   proc_entry->gid = 0
   proc_entry->size = 0*/

   return 0;
}

void cleanup_module() {
  remove_proc_entry("lab2", &proc_root);
}
