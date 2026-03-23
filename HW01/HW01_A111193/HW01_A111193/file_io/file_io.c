#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("reverse file to output");

#define BUF_SIZE 1024

void *in_buf;
void *out_buf;

static int __init test_start(void){
        struct file *i_fp, *o_fp;
        loff_t pos = 0;
        ssize_t bytes_read, bytes_written;
        char *temp_buf;
        
        printk(KERN_INFO "start converting...\n");

        // Open input and output files
        i_fp = filp_open("/home/user/file_io/input.txt",O_RDWR | O_CREAT, 0644);
        o_fp = filp_open("/home/user/file_io/output.txt",O_RDWR | O_CREAT, 0644);
        if (IS_ERR(i_fp)){
                printk(KERN_INFO "intput file open error\n");
                return -1;
        }
        if (IS_ERR(o_fp)){
                printk(KERN_INFO "output file open error\n");
                return -1;
        }

        // Allocate memory for buffers
        in_buf = kmalloc(BUF_SIZE, GFP_KERNEL);
        out_buf = kmalloc(BUF_SIZE, GFP_KERNEL);
        if (!in_buf || !out_buf) {
            printk(KERN_INFO "Memory allocation error\n");
            return -1;
        }

        // Read from input file
        bytes_read = kernel_read(i_fp, in_buf, BUF_SIZE, &pos);
        bytes_read -= 1;
	in_buf = krealloc(in_buf, bytes_read, GFP_KERNEL);
        if (bytes_read < 0) {
            printk(KERN_INFO "Error reading from input file\n");
            return -1;
        }

        // Reverse the content
        temp_buf = in_buf;
        for (int i = 0; i < bytes_read; i++) {
            ((char *)out_buf)[i] = temp_buf[bytes_read - i - 1];
        }

        // Write to output file
        pos = 0;
        bytes_written = kernel_write(o_fp, out_buf, bytes_read, &pos);
        if (bytes_written < 0) {
            printk(KERN_INFO "Error writing to output file\n");
            return -1;
        }

        // Clean up
        kfree(in_buf);
        kfree(out_buf);
        filp_close(i_fp, NULL);
        filp_close(o_fp, NULL);
        
        printk(KERN_INFO "Conversion completed\n");
        
        return 0;
}

static void __exit test_end(void){
        printk(KERN_INFO "Ending~\n");
}

module_init(test_start);
module_exit(test_end);
