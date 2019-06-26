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
int frequency = 10000;		// Frequency in Hz...
int enable    = 1;			// 0 = disable, 1 = enable 
int tusec_On  = 0;			// duty-cycle HIGH time suring one period in [usec]
int tusec_Off = 0;			// duty-cycle LOW time during one period in [usec]

int status = 1;

module_param(duty, int, 0644);
module_param(frequency, int, 0644);
module_param(enable, int, 0644);
module_param(tusec_On, int, 0);
module_param(tusec_Off, int, 0);

/*--------------------------------------------------*/
/*          Configure the GPIO port(s)              */

int __init pwm_gpio_init(void)
{
	printk("PWM: starting . \n", );
	gpio_request(pwm1, "pwm1");
	gpio_direction_output(pwm1, 0);
	
	return 0;
}

void __exit pwm_gpio_exit(void)
{
	printk("PWM: stopping \n");
	gpio_free(pwm1);
}

/*--------------------------------------------------*/
/*         Thread running calculations              */

struct task_struct *task;
struct task_struct *task2;	

int pwm_thread(void *data){

	while(1){
	/* Calculate from frequency and dutycycle the delay-times */
		tusec_On  = (1000000*duty)/(frequency*100);			// Duration of on-cycle
		tusec_Off = (1000000*(100-duty))/(frequency*100);	// Duration of off-cycle
		if (kthread_should_stop()) break;
	}
	return 0;
}

int pwm_thread2(void *data){

	while(enable){	
		gpio_set_value(pwm1, 1);
		usleep_range(tusec_On, tusec_On);
		gpio_set_value(pwm1, 0);
		usleep_range(tusec_Off, tusec_Off);
	}
	return 0;
}

void pwm_thread_init(void)
{
	printk(KERN_INFO "PWM: started thread...");
	task = kthread_run(pwm_thread, NULL, "pwm1");
	task2 = kthread_run(pwm_thread2, NULL , "pwm2")
}

void pwm_thread_exit(void)
{
	printk(KERN_INFO "PWM: stopping thread...");
	kthread_stop(task);
	kthread_stop(task2);
}

/*--------------------------------------------------*/
/*         The Module starting function             */

int __init pwm_init(void)
{
	printk(KERN_INFO " Starte the %s function.\n", __FUNCTION__);
	/* Configure initial state of the PWM pins */
	pwm_gpio_init();
	pwm_thread_init();
	
	return 0;
}

void __exit pwm_exit(void)  
{
	printk(KERN_INFO " Ending the function %s. \n", __FUNCTION__);
	pwm_thread_exit();
	pwm_gpio_exit();
	
}

module_init(pwm_init);
module_exit(pwm_exit);
