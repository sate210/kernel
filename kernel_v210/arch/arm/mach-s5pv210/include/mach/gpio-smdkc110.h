#ifndef __GPIO_SMDKC110_H_
#define __GPIO_SMDKC110_H_


#define GPIO_PS_VOUT			S5PV210_GPH0(2)
#define GPIO_PS_VOUT_AF			0xFF

#define GPIO_BUCK_1_EN_A		S5PV210_GPH0(3)
#define GPIO_BUCK_1_EN_B		S5PV210_GPH0(4)

#define GPIO_BUCK_2_EN			S5PV210_GPH0(5)
#define GPIO_DET_35			S5PV210_GPH0(6)
#define GPIO_DET_35_AF			0xFF

#define GPIO_nPOWER			S5PV210_GPH2(6)

#define GPIO_EAR_SEND_END		S5PV210_GPH3(6)
#define GPIO_EAR_SEND_END_AF		0xFF

#define GPIO_HWREV_MODE0		S5PV210_GPJ0(2)
#define GPIO_HWREV_MODE1		S5PV210_GPJ0(3)
#define GPIO_HWREV_MODE2		S5PV210_GPJ0(4)
#define GPIO_HWREV_MODE3		S5PV210_GPJ0(7)

#define GPIO_PS_ON			S5PV210_GPJ1(4)

#define GPIO_MICBIAS_EN			S5PV210_GPJ4(2)

#define GPIO_UART_SEL			S5PV210_MP05(7)
/****************************************************************/
#define GPIO_LEVEL_LOW          0
#define GPIO_LEVEL_HIGH         1
#define GPIO_LEVEL_NONE         2
#define GPIO_INPUT              0
#define GPIO_OUTPUT             1

//#define GPIO_WLAN_nRST          S5PV210_GPJ3(3)
//#define GPIO_WLAN_nRST_CPDN     S5PV210_GPJ3CONPDN
//#define GPIO_WLAN_nRST_CPDN_V   (0x3<<6)
//
//#define GPIO_BT_nRST            S5PV210_GPJ3(4)
//#define GPIO_BT_nRST_CPDN       S5PV210_GPJ3CONPDN
//#define GPIO_BT_nRST_CPDN_V     (0x3<<8)

#define GPIO_WLAN_SDIO_CLK      S5PV210_GPG0(0)
#define GPIO_WLAN_SDIO_CLK_AF   2

#define GPIO_WLAN_SDIO_CMD      S5PV210_GPG0(1)
#define GPIO_WLAN_SDIO_CMD_AF   2

#define GPIO_WLAN_SDIO_D0       S5PV210_GPG0(3)
#define GPIO_WLAN_SDIO_D0_AF    2

#define GPIO_WLAN_SDIO_D1       S5PV210_GPG0(4)
#define GPIO_WLAN_SDIO_D1_AF    2

#define GPIO_WLAN_SDIO_D2       S5PV210_GPG0(5)
#define GPIO_WLAN_SDIO_D2_AF    2

#define GPIO_WLAN_SDIO_D3       S5PV210_GPG0(6)
#define GPIO_WLAN_SDIO_D3_AF    2

#define GPIO_WLAN_nRST 					S5PV210_GPH1(3)
#define GPIO_WLAN_BT_EN					S5PV210_GPH1(2)

#define	GPIO_KEY_VOLUME_DOWN						S5PV210_GPH2(0)
#define	GPIO_KEY_MENU						S5PV210_GPH2(1)
#define	GPIO_KEY_POWER					S5PV210_GPH2(2)
#define	GPIO_KEY_HOME						S5PV210_GPH2(3)
#define	GPIO_KEY_VOLUME_UP							S5PV210_GPH2(4)
#define	GPIO_KEY_ESC						S5PV210_GPH2(5)

#endif
/* end of __GPIO_SMDKC110_H_ */

