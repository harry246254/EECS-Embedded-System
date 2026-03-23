#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/init.h>
#include <linux/io.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Driver Demo");
#define DRIVER_NAME "SenTsai" // 定義驅動程序的名稱

static char buffer[256]; // 定義一個大小為256字節的字符緩衝區
static dev_t demo_dev; // 定義設備號
static struct cdev demo_cdev; // 定義字符設備結構體
static dev_t demo_dev = MKDEV(90, 40); //定義一個結構demo_dev，主設備號90，次設備號40		

static ssize_t demo_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
 	int ret = 0; //宣告變數ret來存放還需要複製的字元數  	
    	ret = copy_to_user(buf, buffer, len); // 將數據從內核緩衝區複製到用戶緩衝區 	

    	if(ret == 0) //ret = 0代表全數字元複製成功
    	{
    		printk(KERN_INFO"Read success!\n");
    		return 0; // 返回0表示成功
    	}else //ret != 0代表字元未全數複製
    	{
    		printk(KERN_INFO"Read fail!\n");
    		return -EFAULT; // 返回EFAULT表示失敗
       	}
}

static ssize_t demo_write(struct file *f, const char __user *buf, size_t len, loff_t *off)
{
	int ret = 0; //宣告變數ret來存放還需要複製的字元數
	ret = copy_from_user(buffer, buf, len); // 將數據從用戶緩衝區複製到內核緩衝區
	if(ret == 0) //ret = 0代表全數字元複製成功
    	{
    		printk(KERN_INFO"Write success!\n");
    		return 0; 
    	}else //ret != 0代表字元未全數複製
    	{
    		printk(KERN_INFO"Read fail!\n");
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

static struct file_operations demo_fops = // 定義字符設備操作集
{
	.owner = THIS_MODULE,
	.read = demo_read,
	.write = demo_write,
	.open = demo_open,
	.release = demo_close,
};

static int __init demo_init(void)
{
    int ret = 0, error = 0; //宣告兩個變數，ret是函式的回傳值，error為回傳註冊字符設備的代碼

    error = register_chrdev_region(demo_dev, 1, DRIVER_NAME);// 註冊字符設備的主、次設備號範圍
    printk("MAJOR = %d MINOR = %d\r\n", MAJOR(demo_dev), MINOR(demo_dev));// 印出設備符號信息
    if (error < 0) // error小於零代表註冊失敗，處理註冊失敗情況
    {
        ret = -EBUSY;
        goto fail;  
    }
    cdev_init(&demo_cdev, &demo_fops);// 初始化字符設備結構  
    error = cdev_add(&demo_cdev, demo_dev, 1);// 將字符設備加入到kernel
    if (error < 0)// 如果添加失败，則處理失敗的情況
    {
        ret = -EBUSY;
        goto fail;
    }   
    printk("Hello demo!!\n");
    return 0; //成功返回0

// 如果註冊字符或添加字符設備备失敗，才會執行flag的動作
fail: 
    return ret;// 返回錯誤碼
}
static void __exit demo_exit(void)
{
	cdev_del(&demo_cdev); // 刪除字符設備
	unregister_chrdev_region(demo_dev, 1); // 注銷字符設備號
	printk("Goodbye demo!!\n");
}
module_init(demo_init);
module_exit(demo_exit);