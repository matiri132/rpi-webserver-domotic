#include <linux/init.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/gpio.h>

/* Definicion de cantidad de pines a controlar */
#define NPINS 1

MODULE_LICENSE("GPL");

#define A1  17// 0
#define A2  27 // 1
#define A3  22 // 2

void lamp_gpio_init();
void lamp_gpio_exit()


// MODULE
static int __init lamp_init(void){

  printk(KERN_INFO "LAMP: staring...");

  lamp_gpio_init();
  gpio_set_value(A1 , 0);	

  printk(KERN_INFO "LAMP: staring done.");

  return 0;
}
static void __exit lamp_exit(void){
  printk(KERN_INFO "LAMP: stopping...");
  gpio_set_value(A1 , 1);
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



//MODULE INSTALL
module_init(lamp_init);
module_exit(lamp_exit);
