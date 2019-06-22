#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/vmalloc.h>
#include <asm/uaccess.h>
#include <linux/uaccess.j>
#include <linux/gpio.h>

MODULE_LICENSE("GPL");

#define A1  17// 0
#define A2  27 // 1
#define A3  22 // 2

void lamp_gpio_init(void);
void lamp_gpio_exit(void);
int proc_init(void);
void proc_exit(void);
ssize_t lampctrl_write(struct file *file, char __user *ubuf, size_t count, loff_t *ppos);
ssize_t lampctrl_read(struct file *file, char __user *ubuf,size_t count, loff_t *ppos);


static char *user_set;
static char *status;
static int lamps[] = {A1 , A2 , A3};

static struct proc_dir_entry *proc_entry;

//GPIO
void lamp_gpio_init(void){
  printk(KERN_INFO "LAMP: starting gpio...");

  gpio_request(A1, "A1");
  gpio_request(A2, "A2");
  gpio_request(A3, "A3");

  gpio_direction_output(A1, 1);
  gpio_direction_output(A2, 1);
  gpio_direction_output(A3, 1);

  printk(KERN_INFO "PIX: starting gpio done.");
}

void lamp_gpio_exit(void){
  gpio_free(A1);
  gpio_free(A2);
  gpio_free(A3);   

}

//PROC
ssize_t lampctrl_write( struct file *filp, char __user *buff, unsigned long count, void *ppos ){
  
  if(*ppos > 0 || count > strlen(status)){
    return -EFAULT;
  }
  if (copy_from_user( &user_set, buff, count )) {
    return -EFAULT;
  }
  int i = 0;
  while(i < 3){
    if(user_set[i]=='0'){
      gpio_set_value(lamps[i], 0);
    }else{
      gpio_set_value(lamps[i], 1);
    }
    status[i] = user_set[i];
    i++;
  }
  
  return strlen(status);
 
}

ssize_t lampctrl_read( struct file *filep, char __user *buffer, size_t count, loff_t *ppos ){
 
  if(copy_to_user(buffer, status, strlen(status))){
    printk(KERN_INFO "lampctrl: fallo al enviar status\n");
    return -EFAULT;
  }else {  
      printk(KERN_INFO "lampctrl: Se ha enviado estado actual al usuario : %s\n", status);
      return (strlen(status));
   }
 
}

struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = lampctrl_read,    //read()
    .write = lampctrl_write, //write()
};

int init_proc(void){

  int ret = 0;
  user_set = (char*)vmalloc(PAGE_SIZE);

  if(!user_set){
    ret = -ENOMEM;
  }else{
    memset(user_set , 0 , PAGE_SIZE);
    proc_entry = proc_create("lampctrl", 0644 , NULL, &ops);
    if(proc_entry == NULL){
      vfree(user_set);
      printk(KERN_INFO "lampctrl: Couldn't create proc entry\n");
    }
  }
  return ret;
}

void proc_exit( void ){
 
  proc_remove(proc_entry);
  vfree(user_set);
 
}


// MODULE
static int __init lamp_init(void){

  printk(KERN_INFO "LAMP: staring...");

  status[0] = '0' ; status[1] = '0' ; status[2] = '0';
  
  lamp_gpio_init();
  proc_init();

  printk(KERN_INFO "LAMP: staring done.");

  return 0;
}
static void __exit lamp_exit(void){
  printk(KERN_INFO "LAMP: stopping...");

  proc_exit();
  lamp_gpio_exit();	

  printk(KERN_INFO "LAMP: stopping done.");
}


//MODULE INSTALL
module_init(lamp_init);
module_exit(lamp_exit);
