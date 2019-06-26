
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/moduleparam.h>

MODULE_LICENSE("GPL");

#define A1  17// Pin 11
#define A2  27 // Pin 13
#define A3  22 // Pin 15
#define A4  23 // Pin 16

//static char *user_set;

static char status[4] = {'0' , '0' , '0' , '0'};
static int lamps[] = {A1 , A2 , A3 , A4};
static int read_p;
static int counter = 0;
static int LAMPNUM = 4;
static int enable = 1;
module_param(enable , int , 0644);

//PROC
static int proc_open(struct inode *sp_inode, struct file *file) {
	read_p = 1;
	printk("proc called open\n");
	return 0;
}

static int proc_release(struct inode *sp_inode,struct file *sp_file) {
	printk("proc called release\n");
	return 0;
}

static size_t lampctrl_write( struct file *file, char __user *buff, size_t count, loff_t *ppos ){

  printk("proc call write\n");

  if(count > LAMPNUM+1){	
      printk("Error en el valor de seteo");
      return -EFAULT;
  }

  counter = 0;
  while(counter < LAMPNUM){
    if(buff[counter]=='0'){
      printk("Apaga lampara: %d" , lamps[counter]);
      gpio_set_value(lamps[counter], 1);
    }else{
      printk("Enciende lampara: %d" , lamps[counter]);
      gpio_set_value(lamps[counter], 0);
    }
      status[counter] = buff[counter];
      counter++;
   }
  return count;

}

static ssize_t lampctrl_read( struct file *file, char __user *buffer, size_t count, loff_t *ppos ){

  printk("proc Called read\n");
  read_p = !read_p;
  if(read_p){
	return 0;
  }
  if(copy_to_user(buffer, status, LAMPNUM)){
    printk("lampctrl: fallo al enviar status\n");
    return -EFAULT;
  }else {
      printk("lampctrl: Se ha enviado estado actual al usuario : %s\n", status);
      *ppos = 0;
      return strlen(status);
   }
  

}

static struct file_operations fops = {
    .open = proc_open,
    .read = lampctrl_read,
    .write = lampctrl_write,
    .release =proc_release,
    .owner = THIS_MODULE,
};

static int proc_init(void){

  printk("LAMP: proc starting...");

  if(!proc_create("lampctrl", 0644 , NULL, &fops)){
      printk("lampctrl: Couldn't create proc entry\n");
      remove_proc_entry("lampctrl",NULL);
      return -ENOMEM;
  }
  printk("LAMP: proc succes...");
  return 0;
}

static void proc_exit( void ){
  remove_proc_entry("lampctrl" , NULL);
  printk("Removing proc");
}


//GPIO
static void lamp_gpio_init(void){
  printk( "LAMP: starting gpio...");

  gpio_request(A1, "A1");
  gpio_request(A2, "A2");
  gpio_request(A3, "A3");
  gpio_request(A4, "A4");

  gpio_direction_output(A1, 1);
  gpio_direction_output(A2, 1);
  gpio_direction_output(A3, 1);
  gpio_direction_output(A4, 1);
  printk("LAMP: starting gpio done.");
}

static void lamp_gpio_exit(void){
  
 
  gpio_free(A1);
  gpio_free(A2);
  gpio_free(A3);
  gpio_free(A4);

}


// MODULE
static int __init lamp_init(void){

  printk(KERN_INFO "LAMP: starting...");

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
