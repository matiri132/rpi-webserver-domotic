#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/vmalloc.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");

#define A1  17// 0
#define A2  27 // 1
#define A3  22 // 2

void lamp_gpio_init(void);
void lamp_gpio_exit(void)
int proc_init(void);
int proc_exit(void);
ssize_t lampctrl_write( struct file *filp, const char __user *buff, unsigned long len, void *data );
int lampctrl_read(char *page, char **start, off_t off, int count, int *eof, void *data);
 

static struct proc_dir_entry *proc_entry;
static char *user_set;
static char *status;
static int[3] lamps = [A1 , A2 , A3];

// MODULE
static int __init lamp_init(void){

  printk(KERN_INFO "LAMP: staring...");

  for(int i = 0 ; i < 3 ; i++){
    status[i] = '0';  
  }
  
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

int init_proc(void){

  int ret = 0;
  user_set = (char*)vmalloc(PAGE_SIZE);

  if(!user_set){
    ret = -ENOMEM;
  }else{
    memset(user_set , 0 , PAGE_SIZE);
    proc_entry = create_proc_entry("lampctrl", 0644 , NULL);
    if(proc_entry == NULL){
      vfree(user_set);
      printk(KERN_INFO "lampctrl: Couldn't create proc entry\n");
    }else{
      proc_entry->read_proc = lampctrl_read;
      proc_entry->write_proc = lampctrl_write;
      proc_entry->owner = THIS_MODULE;
 
    }
  }
  return ret;
}

void proc_exit( void ){
 
  remove_proc_entry("lampctrl", &proc_root);
  vfree(user_set);
 
}

ssize_t lampctrl_write( struct file *filp, const char __user *buff, unsigned long len, void *data ){
 
  if (copy_from_user( &user_set, buff, len )) {
    return -EFAULT;
  }
  
  for(int i = 0 ; i < 3 ; i++){
    if(user_set[i]=='0'){
      gpio_set_value(lamps[i], 0);
    }else{
      gpio_set_value(lamps[i], 1);
    }
  }

  status = user_set;
  return len;
 
}

int lampctrl_read( char *page, char **start, off_t off, int count, int *eof, void *data ){
 
  int len;
  if (off > 0) {
    *eof = 1;
    return 0;
  }
   
  len = sprintf(page, "%s\n", &user_set);
  return len;
 
}


//MODULE INSTALL
module_init(lamp_init);
module_exit(lamp_exit);
