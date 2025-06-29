/**
 * Copyright (c) 2023 Milk-V
 *
 * SPDX-License-Identifier: BSD-3-Clause
 **/

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

#include <wiringx.h>

 /* 关于SSD1306 OLED显示模块的示例代码 
    Example code to SSD1306 OLED Display Module

    注意：确保设备在3.3伏而不是5伏的电压下使用。Duo GPIO（以及I2C）不能在5伏电平下使用。
    NOTE: Ensure the device is capable of being driven at 3.3v NOT 5v. The Duo
    GPIO (and therefore I2C) cannot be used at 5v.

    如果你想在5伏的电压下使用该模块，需要额外加装电平转换器转换I2C电平。
    You will need to use a level shifter on the I2C lines if you want to run the
    board at 5v.

    请按下面的注释接线，运行程序前请确保引脚复用在正确的功能上。
    Please wire according to the notes below and make sure 
    the pins are set for the correct function before running the program.

    I2C1_SDA -> SDA on SSD1306 Moudle
    I2C1_SCL -> SCL on SSD1306 Moudle
    3.3v -> VCC on SSD1306 Moudle
    GND -> GND on SSD1306 Moudle
 */

// 使用到的I2C地址 The I2C used
#define I2C_DEV "/dev/i2c-1"
int fd_i2c;

// 器件地址 device address
#define MPU6050_SLAVE_ADDR 0x68
#define WHOAMI 0x75
#define POWER_MGMT_ADDR 0x38

#define OLED_CMD     0
#define OLED_DATA    1
#define OLED_DATA    1


#define PAGE_SIZE    8
#define XLevelL		 0x00
#define XLevelH		 0x10
#define YLevel       0xB0
#define	Brightness	 0xFF 
#define WIDTH 	     128
#define HEIGHT 	     32	


void mpu6050_write_byte(unsigned cmd, unsigned dat)
{
	if(cmd)
	{
		wiringXI2CWriteReg8(fd_i2c, POWER_MGMT_ADDR, dat);
	}
	else
	{
		wiringXI2CWriteReg8(fd_i2c, 0x00, dat);
	}
}

int mpu6050_read_byte(unsigned reg)
{
    int value = wiringXI2CReadReg8(fd_i2c, reg);  // Register 0x01
    if (value == -1) {
        fprintf(stderr, "I2C Read failed\n");
    } else {
        printf("Read value: %d\n", value);
    }

    return value;
}

/**
  * @brief          SSD1306初始化 SSD1306 initialization
  * @retval         void
  */
void mpu6050_init(void)
{
    // Wake: Write 1 to power management 
    // mpu6050_write_byte(POWER_MGMT_ADDR, 1);
    wiringXI2CWriteReg8(fd_i2c, POWER_MGMT_ADDR, 0x1);
    delayMicroseconds(100000);
}  

/**
  * @brief          SSD1306初始化 SSD1306 initialization
  * @retval         void
  */
int mpu6050_verify(void)
{
    // Read address from WHOAMI register 0x75 (117)
    int a = mpu6050_read_byte(WHOAMI);
    printf("Value read from register is %d\n", a);
    if(a != MPU6050_SLAVE_ADDR) {
        return 0;
    }
    return 1;
}  

int main() {
    int data = 0;

    // Duo:     milkv_duo
    // Duo256M: milkv_duo256m
    // DuoS:    milkv_duos
    if(wiringXSetup("milkv_duo256m", NULL) == -1) {
        wiringXGC();
        return -1;
    }

    if ((fd_i2c = wiringXI2CSetup(I2C_DEV, MPU6050_SLAVE_ADDR)) <0) {
        printf("I2C Setup failed: %i\n", fd_i2c);
        return -1;
    }

    printf("Setup is successful\n");

    mpu6050_init();
    printf("Done initialization\n");
    mpu6050_verify();
    printf("Done verifying\n");

    // ssd1306_init();
    // ssd1306_clear_full_screen(0);

    // ssd1306_push_string(0,0,"Hello MilkV Duo!",8);
    // ssd1306_push_string(0,1,"Hello MilkV Duo!",16);
}
