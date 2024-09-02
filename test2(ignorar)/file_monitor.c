#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#define DEVICE_NAME "file_monitor"

static int major;
static char message[256] = {0};

static ssize_t dev_read(struct file *file, char __user *buf, size_t len, loff_t *offset) {
    return simple_read_from_buffer(buf, len, offset, message, strlen(message));
}

static struct file_operations fops = {
    .read = dev_read,
};

static int __init lkm_example_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) {
        printk(KERN_ALERT "Failed to register character device\n");
        return major;
    }
    printk(KERN_INFO "Module loaded. Device major number: %d\n", major);
    return 0;
}

static void __exit lkm_example_exit(void) {
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "Module unloaded\n");
}

module_init(lkm_example_init);
module_exit(lkm_example_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple character device for file monitoring.");
MODULE_VERSION("0.01");
