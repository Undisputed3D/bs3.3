/**
 * jiffie_seconds.c
 *
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/version.h>

#define BUFFER_SIZE 128

#define PROC_NAME "jiffies"
#define PROC2_NAME "seconds"


/*
 * Global variables
 */
extern unsigned long volatile jiffies;
static unsigned long jiffies_ref;

/**
 * Function prototypes
 */
static char * get_mod_jiffies(void);
static char * get_mod_seconds(void);
static ssize_t proc_read(struct file *file, char *buf, size_t count, loff_t *pos);


#if LINUX_VERSION_CODE <= KERNEL_VERSION(5,6,0)

static struct file_operations proc_jiffie_ops = {
        .owner = THIS_MODULE,
        .read = proc_read,
};

static struct file_operations proc_seconds_ops = {
        .owner = THIS_MODULE,
        .read = proc_read,
};
#else

static struct proc_ops proc_jiffie_ops = {
        .proc_read = proc_read
};

static struct proc_ops proc_seconds_ops = {
        .proc_read = proc_read
};
#endif


/* This function is called when the module is loaded. */
static int proc_init(void)
{
        /* initializes the jiffies at module load time */
        /*
         * your code here
         *
         */
        
    
        /* creates the /proc/jiffie and /proc/seconds entry */
        /*
         * your code here
         *
         */
	return 0;
}

/* This function is called when the module is removed. */
static void proc_exit(void) {
        // removes the /proc/jiffies and /proc seconds entry
        /*
         * your code here
         *
         */
}
/* call this function when /proc/jiffies is read */
static char * get_mod_jiffies() {
        /*
         * your code here
         *
         */
}

/* call this function when /proc/seconds is read */
static char * get_mod_seconds() {
        /*
         * your code here
         *
         */
}

/**
 * This function is called each time the /proc/jiffies
 * or /proc/seconds is read.
 * 
 * params:
 *
 * file:
 * buf: buffer in user space
 * count:
 * pos:
 */
static ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
        int rv = 0;
        char buffer[128];
        char * proc_file_name;
        long unsigned int res=0;
        static int completed = 0;
        printk(KERN_INFO "state completed=[%i]\n", completed);
        if (completed) {
                completed = 0;
                return 0;
        }

        completed = 1;
        
        /*
         * filname of read file is in: file->f_path.dentry->d_iname
         */
        proc_file_name=file->f_path.dentry->d_iname;

        /* check which file name has been read [jiffies/seconds] and dispatch! */
        /*
         * your code here
         *
         */
        
        
        // copies the contents of buffer to userspace usr_buf
        res=copy_to_user(usr_buf, buffer, rv);
        if (res>0)
                printk(KERN_ALERT "Could not copy all bytes to userspace");
        
        return rv;
}


/* Macros for registering module entry and exit points. */
module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Proc Jiffie Seconds Module");
MODULE_AUTHOR("YOU");

