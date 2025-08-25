#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio/consumer.h>
#include <linux/interrupt.h>
#include <linux/timer.h>
#include <linux/spinlock.h>

#define IO_LED 21
#define IO_BUTTON 20

#define IO_OFFSET 512

#define DEBOUNCE_DELAY_MS 50

static struct gpio_desc *led, *button;
static struct timer_list debounce_timer;
static bool irq_disabled = false;
static spinlock_t irq_disabled_lock = __SPIN_LOCK_UNLOCKED(irq_disabled_lock);

static int irq;
static int ret;
static int count = 0;

// Interrupt handler for GPIO20
static irqreturn_t gpio20_irq_handler(int irq, void *dev_id)
{
	unsigned long flags;
	spin_lock_irqsave(&irq_disabled_lock, flags);

	if (!irq_disabled)
	{
		pr_info("GPIO20 interrupt triggered, button value = %d, count = %d \n", gpiod_get_value(button), count++);
		disable_irq_nosync(irq);
		irq_disabled = true;

		// Start debounce time
		mod_timer(&debounce_timer, jiffies + msecs_to_jiffies(DEBOUNCE_DELAY_MS));
	}
    
	spin_unlock_irqrestore(&irq_disabled_lock, flags);

    return IRQ_HANDLED;
}

static void debounce_timer_callback(struct timer_list *t)
{
	unsigned long flags;
	spin_lock_irqsave(&irq_disabled_lock, flags);
    irq_disabled = false;
    enable_irq(irq);
	spin_unlock_irqrestore(&irq_disabled_lock, flags);
}

static int __init my_init(void)
{
    // Get GPIO descriptors (replace IO_OFFSET if specific to your platform)
    led = gpio_to_desc(IO_LED + IO_OFFSET);
    if (!led) {
        printk(KERN_ERR "gpioctrl - Error getting led pin %d\n", IO_LED + IO_OFFSET);
        return -ENODEV;
    }

    button = gpio_to_desc(IO_BUTTON + IO_OFFSET);
    if (!button) {
        printk(KERN_ERR "gpioctrl - Error getting button pin %d\n", IO_BUTTON + IO_OFFSET);
        return -ENODEV;
    }

    // Set directions before requesting IRQ
    ret = gpiod_direction_output(led, 0);
    if (ret) {
        printk(KERN_ERR "gpioctrl - Error setting led pin to output\n");
        return ret;
    }

    ret = gpiod_direction_input(button);
    if (ret) {
        printk(KERN_ERR "gpioctrl - Error setting button pin to input\n");
        return ret;
    }

    // Map GPIO descriptor to IRQ number
    irq = gpiod_to_irq(button);
    if (irq < 0) {
        printk(KERN_ERR "gpioctrl - Failed to get IRQ for button\n");
        return irq;
    }

    // Request threaded IRQ (no dev context, no devm_)
    ret = request_threaded_irq(irq, NULL, gpio20_irq_handler,
                               IRQF_TRIGGER_RISING | IRQF_ONESHOT,
                               "gpio20_irq", NULL);
    if (ret) {
        printk(KERN_ERR "gpioctrl - Failed to request IRQ\n");
        return ret;
    }

	timer_setup(&debounce_timer, debounce_timer_callback, 0);

    gpiod_set_value(led, 1);

    printk(KERN_INFO "gpioctrl - Button is %spressed\n",
           gpiod_get_value(button) ? "" : "not ");

    return 0;
}

static void __exit my_exit(void)
{
    gpiod_set_value(led, 0);
	del_timer_sync(&debounce_timer);
    free_irq(irq, NULL);
	printk(KERN_INFO "Module unloaded!\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Johannes 4Linux");
MODULE_DESCRIPTION("An example for using GPIOs without the device tree and without platform device");
