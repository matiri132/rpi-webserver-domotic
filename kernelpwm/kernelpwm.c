#include <linux/init.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/moduleparam.h>

MODULE_LICENSE("GPL");

/* GPIO Pin Assignment*/

#define pwm1		24 // Kernel PWM channel 0

/* Global PWM parameters */
int duty      = 50;			// Dutycycle in %
int frequency = 1000;		// Frequency in Hz...
int enable    = 1;			// 0 = disable, 1 = enable 
int tusec_On  = 0;			// duty-cycle HIGH time suring one period in [usec]
int tusec_Off = 0;			// duty-cycle LOW time during one period in [usec]

int status = 1;

DECLARE_WAIT_QUEUE_HEAD(wq);

int condition;

module_param(duty, int, 0644);
module_param(frequency, int, 0644);
module_param(tusec_On, int, 0);
module_param(tusec_Off, int, 0);

/*--------------------------------------------------*/
//        GPIO

int __init pwm_gpio_init(void)
{
	printk("PWM: configuracion GPIO . \n" );
	gpio_request(pwm1, "pwm1");
	gpio_direction_output(pwm1, 0);
	
	return 0;
}

void __exit pwm_gpio_exit(void)
{
	printk("PWM: libera GPIO \n");
	gpio_free(pwm1);
}
/*--------------------------------------------------*/
// THREADS
struct task_struct *task;
struct task_struct *task2;	

int pwm_thread(void *data){
	printk("PWM: Hilo control PWM incia...");
	while(!kthread_should_stop()){

		tusec_On  = (1000000*(duty))/(frequency*100);			// Duration of on-cycle
		tusec_Off = (1000000*(100-duty))/(frequency*100);	// Duration of off-cycle
                gpio_set_value(pwm1, 1);
                usleep_range(tusec_On, tusec_On);
                gpio_set_value(pwm1, 0);
                usleep_range(tusec_Off, tusec_Off);
	}
	return 0;
}


void pwm_thread_init(void)
{
	
	task = kthread_create(pwm_thread, NULL, "pwm1");
	printk("PWM: creo Hilo de kernel...");	
	wake_up_process(task);
}

void pwm_thread_exit(void)
{
	printk("PWM: muere hilo de kernel...");
	kthread_stop(task);
}

/*--------------------------------------------------*/
//MODULE

int __init pwm_init(void)
{
	pwm_gpio_init();
	pwm_thread_init();
	
	return 0;
}

void __exit pwm_exit(void)  
{
	pwm_thread_exit();
	pwm_gpio_exit();
	
}

module_init(pwm_init);
module_exit(pwm_exit);
