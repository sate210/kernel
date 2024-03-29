/*
 * Copyright (C) 2008 Google, Inc.
 * Author: Nick Pelly <npelly@google.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

/* Control bluetooth power for smdkv210 platform */

#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/rfkill.h>
#include <linux/delay.h>
#include <linux/types.h>
#include <linux/wakelock.h>
#include <linux/irq.h>
#include <linux/workqueue.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <mach/gpio.h>
#include <mach/hardware.h>
#include <plat/gpio-cfg.h>
#include <plat/irqs.h>
#include <linux/io.h>
#include <mach/dma.h>
#include <plat/regs-otg.h>
#include <mach/regs-gpio.h>
#include <plat/gpio-cfg.h>
#include <mach/gpio-smdkc110.h>

#ifdef CONFIG_CPU_IDLE
#include <mach/cpuidle.h>
#endif /* CONFIG_CPU_IDLE */

//#define BT_SLEEP_ENABLER

static struct wake_lock rfkill_wake_lock;
#ifdef BT_SLEEP_ENABLER
static struct wake_lock bt_wake_lock;
#endif 

void rfkill_switch_all(enum rfkill_type type, enum rfkill_user_states state);
extern void s3c_setup_uart_cfg_gpio(unsigned char port);

#ifdef CONFIG_CPU_IDLE
extern void s5p_set_lpaudio_lock(int flag);
#endif /* CONFIG_CPU_IDLE */

extern void wlan_setup_power(int flag, int on);

static struct rfkill *bt_rfk;

#if defined (CONFIG_MRVL8787)
static const char bt_name[] = "mrvell8787";
#endif

extern int wlan_carddetect_en(int onoff);
static unsigned char onetime = 0;


#ifdef CONFIG_MRVL8787
extern int get_wlan_power_state(void);
#endif
static bool lpaudio_lock = 0;
static bool bt_init_complete = 0;
static unsigned long suspend = 0;


static ssize_t get_state(struct device *dev, struct device_attribute *attr, char *buf)
{
//printk("bt get suspend = %ld\n", suspend);
	return sprintf(buf, "%ld\n", suspend);
}

static ssize_t set_state(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
  int rc;
  rc = strict_strtoul(buf, 0, &suspend);
//printk("bt set suspend = %ld\n", suspend);
	return strnlen(buf, PAGE_SIZE);
}

static DEVICE_ATTR(state, 0777, get_state, set_state);


void bt_config_gpio_table(int array_size, unsigned int (*gpio_table)[4])
{
	u32 i, gpio;

	for (i = 0; i < array_size; i++) {
		gpio = gpio_table[i][0];
		s3c_gpio_cfgpin(gpio, S3C_GPIO_SFN(gpio_table[i][1]));
		s3c_gpio_setpull(gpio, gpio_table[i][3]);
		if (gpio_table[i][2] != GPIO_LEVEL_NONE)
			gpio_set_value(gpio, gpio_table[i][2]);
	}
}



static int bluetooth_set_power(void *data, enum rfkill_user_states state)
{
	if(onetime == 0)
	{
		onetime = 1;
		return 0;
	}
	switch (state) {

		case RFKILL_USER_STATE_UNBLOCKED:
		{
			printk("[BT] Device Powering ON \n");

      wlan_setup_power(1,1);
//  		s3c_gpio_cfgpin(GPIO_BT_HOST_WAKE, S3C_GPIO_SFN(GPIO_BT_HOST_WAKE_AF));
//  		s3c_gpio_setpull(GPIO_BT_HOST_WAKE, S3C_GPIO_PULL_DOWN);
//  
//  		s3c_gpio_cfgpin(GPIO_BT_WAKE, S3C_GPIO_SFN(GPIO_BT_WAKE_AF));
//  		s3c_gpio_setpull(GPIO_BT_WAKE, S3C_GPIO_PULL_NONE);
//  		gpio_set_value(GPIO_BT_WAKE, GPIO_LEVEL_LOW);

#ifdef  CONFIG_MRVL8787
      if(get_wlan_power_state() != 0x03)
        wlan_carddetect_en(1);
#endif
			break;
    }
		case RFKILL_USER_STATE_SOFT_BLOCKED:
		{
			printk("[BT] Device Powering OFF \n");
      wlan_setup_power(1,0);
#ifdef	CONFIG_MRVL8787
      if(get_wlan_power_state() == 0)
        wlan_carddetect_en(0);
#endif

			break;
		}

		default:
			printk("[BT] Bad bluetooth rfkill state %d\n", state);
	}

	return 0;
}

static void bt_host_wake_work_func(struct work_struct *ignored)
{
	printk("[BT] wake_lock timeout = 5 sec\n");
	wake_lock_timeout(&rfkill_wake_lock, 5*HZ);
	
#ifdef CONFIG_CPU_IDLE
	if(bt_init_complete && !lpaudio_lock)
	{
		s5p_set_lpaudio_lock(1);
		lpaudio_lock = 1;
	}
#endif /* CONFIG_CPU_IDLE */
	
	enable_irq(GPIO_BT_HOST_IRQ);
}
static DECLARE_WORK(bt_host_wake_work, bt_host_wake_work_func);


irqreturn_t bt_host_wake_irq_handler(int irq, void *dev_id)
{
printk("[BT] bt_host_wake_irq_handler start\n");

	disable_irq_nosync(GPIO_BT_HOST_IRQ);

	schedule_work(&bt_host_wake_work);

	return IRQ_HANDLED;
}

static int bt_rfkill_set_block(void *data, bool blocked)
{
	int ret =0;

printk("%s %d\n", __FUNCTION__, blocked);
	ret = bluetooth_set_power(data, blocked? RFKILL_USER_STATE_SOFT_BLOCKED : RFKILL_USER_STATE_UNBLOCKED);
		
	return ret;
}

static const struct rfkill_ops bt_rfkill_ops = {
	.set_block = bt_rfkill_set_block,
};

static int __init smdkv210_rfkill_probe(struct platform_device *pdev)
{
	int rc = 0;
	int irq,ret;

	//Initialize wake locks
	wake_lock_init(&rfkill_wake_lock, WAKE_LOCK_SUSPEND, "board-rfkill");
#ifdef BT_SLEEP_ENABLER
	wake_lock_init(&bt_wake_lock, WAKE_LOCK_SUSPEND, "bt-rfkill");
#endif

	//BT Host Wake IRQ
	irq = GPIO_BT_HOST_IRQ;
	

	bt_rfk = rfkill_alloc(bt_name, &pdev->dev, RFKILL_TYPE_BLUETOOTH, &bt_rfkill_ops, NULL);
	if (!bt_rfk)
		return -ENOMEM;

	rfkill_init_sw_state(bt_rfk, 0);
	printk("[BT] rfkill_register(bt_rfk) \n");
	rc = rfkill_register(bt_rfk);
	if (rc)
	{
		printk ("***********ERROR IN REGISTERING THE RFKILL***********\n");
		rfkill_destroy(bt_rfk);
	}
	printk("[BT] rfkill_register(bt_rfk) OK\n");


  //bluetooth off
	rfkill_set_sw_state(bt_rfk, 1);
	//bluetooth_set_power(NULL, RFKILL_USER_STATE_SOFT_BLOCKED);
  ret = device_create_file(&(pdev->dev), &dev_attr_state);
	return rc;
}

static int smdkv210_rfkill_suspend(struct platform_device *pdev, pm_message_t state)
{
//	printk("%s\n", __func__);

#ifdef CONFIG_MRVL8787
  if(get_wlan_power_state() & 0x02)
  {
    wlan_setup_power(1,0);
    onetime = 2;
  }
  else
    onetime = 1;
#endif /* CONFIG_MRVL8787 */
	return 0;
}
static int smdkv210_rfkill_resume(struct platform_device *pdev)
{
#ifdef CONFIG_MRVL8787
  if((onetime == 2) && (get_wlan_power_state() == 0))
  {
    wlan_setup_power(1,1);
    wlan_carddetect_en(1);
  }
#endif /* CONFIG_MRVL8787 */
	return 0;
}

static struct platform_driver smdkv210_device_rfkill = {
	.probe = smdkv210_rfkill_probe,
	.suspend = smdkv210_rfkill_suspend,
	.resume = smdkv210_rfkill_resume,
	.driver = {
		.name = "bt_rfkill",
		.owner = THIS_MODULE,
	},
};

#ifdef BT_SLEEP_ENABLER
static struct rfkill *bt_sleep;

static int bluetooth_set_sleep(void *data, enum rfkill_user_states state)
{	
	int ret =0;
	
	switch (state) {

		case RFKILL_USER_STATE_UNBLOCKED:
			printk ("[BT] In the unblocked state of the sleep\n");
//			if (gpio_is_valid(GPIO_BT_WAKE))
//			{
//				ret = gpio_request(GPIO_BT_WAKE, "GPG3");
//				if(ret < 0) {
//					printk("[BT] Failed to request GPIO_BT_WAKE!\n");
//					return ret;
//				}
//				gpio_direction_output(GPIO_BT_WAKE, GPIO_LEVEL_LOW);
//			}
//
//			s3c_gpio_setpull(GPIO_BT_WAKE, S3C_GPIO_PULL_NONE);
//			gpio_set_value(GPIO_BT_WAKE, GPIO_LEVEL_LOW);
			
//			printk("[BT] GPIO_BT_WAKE = %d\n", gpio_get_value(GPIO_BT_WAKE) );
//			gpio_free(GPIO_BT_WAKE);
			
			printk("[BT] wake_unlock(bt_wake_lock)\n");
			wake_unlock(&bt_wake_lock);
			
#ifdef CONFIG_CPU_IDLE
			if(bt_init_complete && lpaudio_lock)
			{
				s5p_set_lpaudio_lock(0);
				lpaudio_lock = 0;
			}
#endif /* CONFIG_CPU_IDLE */
			
			break;

		case RFKILL_USER_STATE_SOFT_BLOCKED:
			printk ("[BT] In the soft blocked state of the sleep\n");
//			if (gpio_is_valid(GPIO_BT_WAKE))
//			{
//				ret = gpio_request(GPIO_BT_WAKE, "GPG3");
//				if(ret < 0) {
//					printk("[BT] Failed to request GPIO_BT_WAKE!\n");
//					return ret;
//				}
//				gpio_direction_output(GPIO_BT_WAKE, GPIO_LEVEL_HIGH);
//			}
//
//			s3c_gpio_setpull(GPIO_BT_WAKE, S3C_GPIO_PULL_NONE);
//			gpio_set_value(GPIO_BT_WAKE, GPIO_LEVEL_HIGH);
//
//			printk("[BT] GPIO_BT_WAKE = %d\n", gpio_get_value(GPIO_BT_WAKE) );
//			gpio_free(GPIO_BT_WAKE);
			printk("[BT] wake_lock(bt_wake_lock)\n");
			wake_lock(&bt_wake_lock);
			break;

		default:
			printk("[BT] bad bluetooth rfkill state %d\n", state);
	}
	return 0;
}

static int btsleep_rfkill_set_block(void *data, bool blocked)
{
	int ret =0;
	
	ret = bluetooth_set_sleep(data, blocked? RFKILL_USER_STATE_SOFT_BLOCKED : RFKILL_USER_STATE_UNBLOCKED);
		
	return ret;
}

static const struct rfkill_ops btsleep_rfkill_ops = {
	.set_block = btsleep_rfkill_set_block,
};

static int __init smdkv210_btsleep_probe(struct platform_device *pdev)
{
	int rc = 0;
	
	bt_sleep = rfkill_alloc(bt_name, &pdev->dev, RFKILL_TYPE_BLUETOOTH, &btsleep_rfkill_ops, NULL);
	if (!bt_sleep)
		return -ENOMEM;

	rfkill_set_sw_state(bt_sleep, 1);

	rc = rfkill_register(bt_sleep);
	if (rc)
		rfkill_destroy(bt_sleep);

	bluetooth_set_sleep(NULL, RFKILL_USER_STATE_UNBLOCKED);

	return rc;
}

static struct platform_driver smdkv210_device_btsleep = {
	.probe = smdkv210_btsleep_probe,
	.driver = {
		.name = "bt_sleep",
		.owner = THIS_MODULE,
	},
};
#endif

static int __init smdkv210_rfkill_init(void)
{
	int rc = 0;

	rc = platform_driver_register(&smdkv210_device_rfkill);

#ifdef BT_SLEEP_ENABLER
	platform_driver_register(&smdkv210_device_btsleep);
#endif

	bt_init_complete = 1;
	return rc;
}

module_init(smdkv210_rfkill_init);
MODULE_DESCRIPTION("smdkv210 rfkill");
MODULE_LICENSE("GPL");
