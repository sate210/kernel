/*
 * drivers/video/samsung/ch7026.c
 *
 * $Id: ch7026.c,v 1.12 2009/09/13 02:13:24 
 *
 * Copyright (C) 2009 sata210 <sata210@gmail.com>
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file COPYING in the main directory of this archive for
 * more details.
 *
 *	S3C Frame Buffer Driver
 *	based on skeletonfb.c, sa1100fb.h, s3c2410fb.c
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/i2c-id.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/io.h>

#include "s3cfb.h"
#include "ch7026.h"

#define CH7026_I2C_ADDR		0x76		//AS下拉:0x76 上拉:0x75
#define I2C_BUS_NUM			0
//struct
//{
//	int addr;
//	struct i2c_client *client;

//}ch7026_data;// = 
//{
//	.addr =CH7026_I2C_ADDR,
//};


static void ch7026_start(struct i2c_client *client)
{
	int i;

	for (i = 0; i <CH7026_INIT_REGS; i++) 
	{
		i2c_master_send(client, ch7026_init_reg[i],sizeof(ch7026_init_reg[i]));
	}
	printk("ch7026 init ok\n");
}

#define ID_ADDR		0x00
static int ch7026_id_detect(struct i2c_client *client)
{
	unsigned char buffer[1];
	int rc;
	buffer[0] = ID_ADDR;
	if (1 != (rc = i2c_master_send(client, buffer, 1)))
		printk("i2c i/o error: rc == %d (should be 1)\n", rc);

	msleep(10);

	if (1 != (rc = i2c_master_recv(client, buffer, 1)))
		printk("i2c i/o error: rc == %d (should be 1)\n", rc);

	if (buffer[0] != 0x54)
	{
		printk("*** unknown chip detected.\n");
	   return -ENODEV;
	}
	else
	{
		printk("ch7026 detected.\n");
		return 0;
	}
}

/*
static int ch7026_attach_adapter(struct i2c_adapter *adapter)
{
	struct i2c_board_info info;

	printk("[VGA] ch7026_attach_adapter.\n");


	memset(&info, 0, sizeof(struct i2c_board_info));

	strlcpy(info.type, "ch7026", I2C_NAME_SIZE);
	info.addr = ch7026_data.addr;
	ch7026_data.client = i2c_new_device(adapter, &info);

	if (!ch7026_data.client)
	{
		printk("failed to add i2c driver\n");
		return -ENODEV;
	}

	if (!ch7026_data.client->driver) 
	{
		i2c_unregister_device(ch7026_data.client);
		ch7026_data.client = NULL;
		printk("failed to attach ch7026 driver\n");
		return -ENODEV;
	}

//	list_add_tail(&ch7026_data.client->detected,
//		      &ch7026_data.client->driver->clients);

	if (ch7026_id_detect(ch7026_data.client))
	{
		printk("ch7026 detect error!\n");
		return -ENODEV;
	}

	printk("ch7026 attached successfully\n");

	ch7026_start(ch7026_data.client);

	return 0;
}
*/
static int __devinit ch7026_probe(struct i2c_client *client,
			 const struct i2c_device_id *id)
{
	dev_info(&client->adapter->dev, "ch7026 probe\n");

	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) 
	{
		printk("i2c_check_functionality error\n");
//		i2c_unregister_device(client);
		return -ENODEV;
	}

	if (ch7026_id_detect(client))
	{
		printk("ch7026 detect error!\n");
//		i2c_unregister_device(client);
		return -ENODEV;
	}

	printk("ch7026 probe successfully\n");

	ch7026_start(client);
	
//	i2c_set_clientdata(client, &ch7026_data);

	return 0;
}

static int ch7026_remove(struct i2c_client *client)
{
//	i2c_unregister_device(client);
	i2c_set_clientdata(client, NULL);
	
//	if (client == ch7026_data.client)
//		ch7026_data.client = NULL;

	return 0;
}

static const struct i2c_device_id ch7026_i2c_id[] = {
	{ "ch7026", 0 }
};

static struct i2c_driver ch7026_i2c_driver = {
	.driver = {
		.owner = THIS_MODULE,
		.name = "ch7026",
	},

//	.attach_adapter = ch7026_attach_adapter,
	.probe = ch7026_probe,
	.remove = __devexit_p(ch7026_remove),
	.id_table = ch7026_i2c_id,
};

static int ch7026_attach_adapter(void)
{
	struct i2c_board_info info;
	struct i2c_adapter *adapter;
	struct i2c_client *client;
	printk("[VGA] ch7026_attach_adapter.\n");
	
	adapter = i2c_get_adapter(I2C_BUS_NUM);

	memset(&info, 0, sizeof(struct i2c_board_info));

	strlcpy(info.type, "ch7026", I2C_NAME_SIZE);
	info.addr = CH7026_I2C_ADDR;
	client = i2c_new_device(adapter, &info);
	if (!client)
	{
		printk("failed to add i2c driver\n");
		return -ENODEV;
	}
	return 0;
}
#if 1
static int __init ch7026_init(void)
{
	int ret;
	ch7026_attach_adapter();
	ret = i2c_add_driver(&ch7026_i2c_driver);
	if (ret != 0)
		pr_err("Failed to register ch7026 I2C driver: %d\n", ret);

	return ret;
}

static void __exit ch7026_exit(void)
{
	i2c_del_driver(&ch7026_i2c_driver);
}

module_init(ch7026_init);
module_exit(ch7026_exit);

MODULE_AUTHOR("sata210 <sata210@gmail.com>");
MODULE_DESCRIPTION("Texas Instruments CH7026 driver");
MODULE_LICENSE("GPL");

#else
int ch7026_init(void)
{
	return i2c_add_driver(&ch7026_i2c_driver);
}
#endif

#if 0
static struct s3cfb_lcd ch7026_vga = {
	.width	= 800,
	.height	= 600,
	.bpp	= 24,
	.freq	= 60,

	.timing = {
		.h_fp	= 10,
		.h_bp	= 20,
		.h_sw	= 10,
		.v_fp	= 10,
		.v_fpe	= 1,
		.v_bp	= 20,
		.v_bpe	= 1,
		.v_sw	= 10,
	},

	.polarity = {
		.rise_vclk	= 0,
		.inv_hsync	= 1,
		.inv_vsync	= 1,
		.inv_vden	= 0,
	},
};



/* name should be fixed as 's3cfb_set_lcd_info' */
void s3cfb_set_lcd_info(struct s3cfb_global *ctrl)
{
	ch7026_vga.init_ldi = NULL;
	ctrl->lcd = &ch7026_vga;

//	ch7026_init();
//	printk("*************************************\n");
}
#endif
