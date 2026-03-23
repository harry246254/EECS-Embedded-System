#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/init.h>
#include <linux/io.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Driver Demo");
#define DRIVER_NAME "SenTsai" 

static char buffer[256]; 
static dev_t demo_dev;
static struct cdev demo_cdev;
static dev_t demo_dev = MKDEV(90, 40);	

static ssize_t demo_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
 	int ret = 0; 	
    	ret = copy_to_user(buf, buffer, len); 	

    	if(ret == 0)
    	{
    		printk(KERN_INFO"Read success!\n");
    		return 0; 
    	}else 
    	{
    		printk(KERN_INFO"Read fail!\n");
    		return -EFAULT;
       	}
}

static ssize_t demo_write(struct file *f, const char __user *buf, size_t len, loff_t *off)
{
	int ret = 0;
	ret = copy_from_user(buffer, buf, len);
	if(ret == 0) 
    	{
    		printk(KERN_INFO"Write success!\n");
    		return 0; 
    	}else
    	{
    		printk(KERN_INFO"Write fail!\n");
    		return -EFAULT; 
       	}
}

static int demo_open(struct inode *nd, struct file *instance)
{
	printk(KERN_INFO"demo open!!\n");
	return 0;
}

static int demo_close(struct inode *df, struct file *instance)
{
	printk(KERN_INFO"demo closed!!\n");
	return 0;
}

static struct file_operations demo_fops = 
{
	.owner = THIS_MODULE,
	.read = demo_read,
	.write = demo_write,
	.open = demo_open,
	.release = demo_close,
};

static int __init demo_init(void)
{
    int ret = 0, error = 0; 

    error = register_chrdev_region(demo_dev, 1, DRIVER_NAME);
    printk("MAJOR = %d MINOR = %d\r\n", MAJOR(demo_dev), MINOR(demo_dev));
    if (error < 0) 
    {
        ret = -EBUSY;
        goto fail;  
    }
    cdev_init(&demo_cdev, &demo_fops);
    error = cdev_add(&demo_cdev, demo_dev, 1);
    if (error < 0)
    {
        ret = -EBUSY;
        goto fail;
    }   
    printk("Hello demo!!\n");
    return 0; 


fail: 
    return ret;
}
static void __exit demo_exit(void)
{
	cdev_del(&demo_cdev); 
	unregister_chrdev_region(demo_dev, 1); 
	printk("Goodbye demo!!\n");
}
module_init(demo_init);
module_exit(demo_exit);
