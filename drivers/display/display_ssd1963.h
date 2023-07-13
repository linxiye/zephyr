#ifndef ZEPHYR_DRIVERS_DISPLAY_DISPLAY_SSD1963_H_
#define ZEPHYR_DRIVERS_DISPLAY_DISPLAY_SSD1963_H_


#define SSD1963_SOFT_RESET                  0x01    /* Software Reset */
#define SSD1963_CHIP_ID                     0xA1    /* Read the DDB from the provided location */
#define SSD1963_SET_DISPLAY_ON              0x29    /* Set the display on */
#define SSD1963_SET_X_CMD                   0x2A    /* Set the column address  */
#define SSD1963_SET_Y_CMD                   0x2B    /* Set the page address */
#define SSD1963_WRITE_GRAM                  0x2C    /* Transfer image information from the host processor interface 
                                                        to the SSD1963 starting at the location provided by
                                                        set_column_address and set_page_address */
#define SSD1963_SET_ADDRESS_MODE            0x36    /* Set the read order from frame buffer to the display panel */
#define SSD1963_SET_LCD_MODE                0xB0    /* Set the LCD panel mode and resolution */
#define SSD1963_SET_HORI_PERIOD             0xB4    /* Set front porch */
#define SSD1963_SET_VERT_PERIOD             0xB6    /* Set back porch */
#define SSD1963_SET_GPIO_CONF               0xB8    /* Set the GPIO configuration. If the GPIO is not used for LCD,
                                                        set the direction. Otherwise, they are toggled with LCD signals */                                       
#define SSD1963_SET_GPIO_VALUE              0xBA    /* Set GPIO value for GPIO configured as output */
#define SSD1963_SET_PWM_CONF                0xBE    /* Set the image post processor */
#define SSD1963_SET_PLL                     0xE0    /* Start the PLL. Before the start, 
                                                        the system was operated with the crystal oscillator or clock input  */
#define SSD1963_SET_PLL_MN                  0xE2    /* Set the PLL */
#define SSD1963_SET_LSHIFT_FREQ             0xE6    /* Set the LSHIFT (pixel clock) frequency */
#define SSD1963_SET_PIXEL_DATA_INTERFACE    0xF0    /* Set the pixel data format of the parallel host processor interface  */





/* LCD分辨率设置 */ 
#define SSD_HOR_RESOLUTION                  800     /* LCD水平分辨率 */ 
#define SSD_VER_RESOLUTION                  480     /* LCD垂直分辨率 */ 

/* LCD驱动参数设置 */ 
#define SSD_HOR_PULSE_WIDTH                 1       /* 水平脉宽 */ 
#define SSD_HOR_BACK_PORCH                  46      /* 水平前廊 */ 
#define SSD_HOR_FRONT_PORCH                 210     /* 水平后廊 */ 

#define SSD_VER_PULSE_WIDTH                 1       /* 垂直脉宽 */ 
#define SSD_VER_BACK_PORCH                  23      /* 垂直前廊 */ 
#define SSD_VER_FRONT_PORCH                 22      /* 垂直前廊 */ 

/* 如下几个参数，自动计算 */ 
#define SSD_HT                              (SSD_HOR_RESOLUTION + SSD_HOR_BACK_PORCH + SSD_HOR_FRONT_PORCH)
#define SSD_HPS                             (SSD_HOR_BACK_PORCH)
#define SSD_VT                              (SSD_VER_RESOLUTION + SSD_VER_BACK_PORCH + SSD_VER_FRONT_PORCH)
#define SSD_VPS                             (SSD_VER_BACK_PORCH)


#endif

