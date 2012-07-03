
#ifndef LINUX_BMA150_MODULE_H
#define LINUX_BMA150_MODULE_H


#define GRAVITY_EARTH					9806550
#define ABSMIN_2G						(-GRAVITY_EARTH * 2)
#define ABSMAX_2G						(GRAVITY_EARTH * 2)
#define BMA150_MAX_DELAY				200
#define BMA150_CHIP_ID					2
#define BMA150_RANGE_SET				0
#define BMA150_BW_SET					4



#define BMA150_CHIP_ID_REG			0x00
#define BMA150_X_AXIS_LSB_REG			0x02
#define BMA150_X_AXIS_MSB_REG		0x03
#define BMA150_Y_AXIS_LSB_REG			0x04
#define BMA150_Y_AXIS_MSB_REG		0x05
#define BMA150_Z_AXIS_LSB_REG			0x06
#define BMA150_Z_AXIS_MSB_REG			0x07
#define BMA150_STATUS_REG				0x09
#define BMA150_CTRL_REG				0x0a
#define BMA150_CONF1_REG				0x0b

#define BMA150_CUSTOMER1_REG			0x12
#define BMA150_CUSTOMER2_REG			0x13
#define BMA150_RANGE_BWIDTH_REG		0x14
#define BMA150_CONF2_REG				0x15

#define BMA150_OFFS_GAIN_X_REG		0x16
#define BMA150_OFFS_GAIN_Y_REG		0x17
#define BMA150_OFFS_GAIN_Z_REG		0x18
#define BMA150_OFFS_GAIN_T_REG		0x19
#define BMA150_OFFSET_X_REG			0x1a
#define BMA150_OFFSET_Y_REG			0x1b
#define BMA150_OFFSET_Z_REG			0x1c
#define BMA150_OFFSET_T_REG			0x1d

#define BMA150_CHIP_ID__POS			0
#define BMA150_CHIP_ID__MSK			0x07
#define BMA150_CHIP_ID__LEN			3
#define BMA150_CHIP_ID__REG			BMA150_CHIP_ID_REG

/* DATA REGISTERS */

#define BMA150_NEW_DATA_X__POS		0
#define BMA150_NEW_DATA_X__LEN		1
#define BMA150_NEW_DATA_X__MSK		0x01
#define BMA150_NEW_DATA_X__REG		BMA150_X_AXIS_LSB_REG

#define BMA150_ACC_X_LSB__POS			6
#define BMA150_ACC_X_LSB__LEN			2
#define BMA150_ACC_X_LSB__MSK		0xC0
#define BMA150_ACC_X_LSB__REG			BMA150_X_AXIS_LSB_REG

#define BMA150_ACC_X_MSB__POS		0
#define BMA150_ACC_X_MSB__LEN		8
#define BMA150_ACC_X_MSB__MSK		0xFF
#define BMA150_ACC_X_MSB__REG		BMA150_X_AXIS_MSB_REG

#define BMA150_ACC_Y_LSB__POS			6
#define BMA150_ACC_Y_LSB__LEN			2
#define BMA150_ACC_Y_LSB__MSK		0xC0
#define BMA150_ACC_Y_LSB__REG			BMA150_Y_AXIS_LSB_REG

#define BMA150_ACC_Y_MSB__POS		0
#define BMA150_ACC_Y_MSB__LEN		8
#define BMA150_ACC_Y_MSB__MSK		0xFF
#define BMA150_ACC_Y_MSB__REG		BMA150_Y_AXIS_MSB_REG

#define BMA150_ACC_Z_LSB__POS			6
#define BMA150_ACC_Z_LSB__LEN			2
#define BMA150_ACC_Z_LSB__MSK		0xC0
#define BMA150_ACC_Z_LSB__REG			BMA150_Z_AXIS_LSB_REG

#define BMA150_ACC_Z_MSB__POS		0
#define BMA150_ACC_Z_MSB__LEN		8
#define BMA150_ACC_Z_MSB__MSK		0xFF
#define BMA150_ACC_Z_MSB__REG		BMA150_Z_AXIS_MSB_REG

/* CONTROL BITS */

#define BMA150_SLEEP__POS				0
#define BMA150_SLEEP__LEN				1
#define BMA150_SLEEP__MSK				0x01
#define BMA150_SLEEP__REG				BMA150_CTRL_REG

#define BMA150_SOFT_RESET__POS		1
#define BMA150_SOFT_RESET__LEN		1
#define BMA150_SOFT_RESET__MSK		0x02
#define BMA150_SOFT_RESET__REG		BMA150_CTRL_REG

#define BMA150_EE_W__POS				4
#define BMA150_EE_W__LEN				1
#define BMA150_EE_W__MSK				0x10
#define BMA150_EE_W__REG				BMA150_CTRL_REG

#define BMA150_UPDATE_IMAGE__POS		5
#define BMA150_UPDATE_IMAGE__LEN		1
#define BMA150_UPDATE_IMAGE__MSK		0x20
#define BMA150_UPDATE_IMAGE__REG		BMA150_CTRL_REG

#define BMA150_RESET_INT__POS			6
#define BMA150_RESET_INT__LEN			1
#define BMA150_RESET_INT__MSK			0x40
#define BMA150_RESET_INT__REG			BMA150_CTRL_REG

/* BANDWIDTH dependend definitions */

#define BMA150_BANDWIDTH__POS		0
#define BMA150_BANDWIDTH__LEN		3
#define BMA150_BANDWIDTH__MSK		0x07
#define BMA150_BANDWIDTH__REG		BMA150_RANGE_BWIDTH_REG

/* RANGE */

#define BMA150_RANGE__POS				3
#define BMA150_RANGE__LEN				2
#define BMA150_RANGE__MSK				0x18
#define BMA150_RANGE__REG				BMA150_RANGE_BWIDTH_REG

/* WAKE UP */

#define BMA150_WAKE_UP__POS			0
#define BMA150_WAKE_UP__LEN			1
#define BMA150_WAKE_UP__MSK			0x01
#define BMA150_WAKE_UP__REG			BMA150_CONF2_REG

#define BMA150_WAKE_UP_PAUSE__POS	1
#define BMA150_WAKE_UP_PAUSE__LEN	2
#define BMA150_WAKE_UP_PAUSE__MSK	0x06
#define BMA150_WAKE_UP_PAUSE__REG	BMA150_CONF2_REG

#define BMA150_GET_BITSLICE(regvar, bitname)\
	((regvar & bitname##__MSK) >> bitname##__POS)


#define BMA150_SET_BITSLICE(regvar, bitname, val)\
	((regvar & ~bitname##__MSK) | ((val<<bitname##__POS)&bitname##__MSK))

/* range and bandwidth */

#define BMA150_RANGE_2G				0
#define BMA150_RANGE_4G				1
#define BMA150_RANGE_8G				2

#define BMA150_BW_25HZ				0
#define BMA150_BW_50HZ				1
#define BMA150_BW_100HZ				2
#define BMA150_BW_190HZ				3
#define BMA150_BW_375HZ				4
#define BMA150_BW_750HZ				5
#define BMA150_BW_1500HZ				6

/* mode settings */

#define BMA150_MODE_NORMAL      		0
#define BMA150_MODE_SLEEP       			2
#define BMA150_MODE_WAKE_UP     		3


struct bma150acc{
	s16	x,
		y,
		z;
} ;

struct bma150_data {
	struct i2c_client *bma150_client;
	struct bma150_platform_data *platform_data;
	int IRQ;
	atomic_t delay;
	unsigned char mode;
	struct input_dev *input;
	struct bma150acc value;
	struct mutex value_mutex;
	struct mutex mode_mutex;
	struct delayed_work work;
	struct work_struct irq_work;
};

struct bma150_platform_data {
	int (*setup)(struct device *);
	void (*teardown)(struct device *);
	int (*power_on)(void);
	void (*power_off)(void);
};



#endif /* LINUX_BMA150_MODULE_H */
