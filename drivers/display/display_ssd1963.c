
#define DT_DRV_COMPAT solomon_ssd1963_fsmc_8080

#include <zephyr/drivers/display.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include "display_ssd1963.h"
#include <zephyr/drivers/gpio.h>

LOG_MODULE_REGISTER(display_ssd1963, CONFIG_DISPLAY_LOG_LEVEL);

#define SSD1963_PROP DT_NODELABEL(ssd1963)



struct ssd1963_config{
    uint32_t cmd_addr;
    uint32_t data_addr;
    struct gpio_dt_spec backlight;
    struct gpio_dt_spec backlight_power;
};

struct ssd1963_data{
    uint16_t	width;
	uint16_t	height;
	uint16_t	hsync_len;
	uint16_t	vsync_len;
	uint16_t	hback_porch;
	uint16_t	vback_porch;
	uint16_t	hfront_porch;
	uint16_t	vfront_porch;
};

typedef struct
{
	uint16_t cmd;       
	uint16_t data[16];
	uint8_t databytes;
    uint8_t delayms;
} ssd1963_cmd_t;


void ssd1963_transmit_command(const struct device *dev,uint16_t cmd)
{
    const struct ssd1963_config *config = dev->config;
    volatile uint32_t addr = config->cmd_addr;
    *((uint16_t *)addr) = cmd;
}

void ssd1963_transmit_data(const struct device *dev,uint16_t data)
{
    const struct ssd1963_config *config = dev->config;
    volatile uint32_t addr = config->data_addr;
    *((uint16_t *)addr) = data;
}

void ssd1963_transmit_buff(const struct device *dev, uint16_t *buff, uint32_t len)
{
    for(int i = 0; i<len ;i++)
    {
        ssd1963_transmit_data(dev, buff[i]);
    }

}

uint16_t ssd1963_transmit_command_buff(const struct device *dev, uint16_t cmd, uint16_t *buff, uint32_t len)
{
    ssd1963_transmit_command(dev, cmd);
    ssd1963_transmit_buff(dev, buff, len);
    return 0;
}

uint16_t ssd1963_recover_data(const struct device *dev)
{
    const struct ssd1963_config *config = dev->config;
    volatile uint32_t addr = config->data_addr;
    return *((uint32_t *)addr);
}



uint16_t ssd1963_read_id(const struct device *dev)
{
    uint16_t ssd1963id = 0;
    ssd1963_transmit_command(dev, SSD1963_CHIP_ID);
    ssd1963id = ssd1963_recover_data(dev);
    ssd1963id = ssd1963_recover_data(dev);  /* 读回0x57 */
    ssd1963id <<= 8;
    ssd1963id |= ssd1963_recover_data(dev); /* 读回0x61 */
    return ssd1963id;
}

/**
 * @brief       设置窗口(对RGB屏无效), 并自动设置画点坐标到窗口左上角(sx,sy).
 * @param       sx,sy:窗口起始坐标(左上角)
 * @param       width,height:窗口宽度和高度,必须大于0!!
 * @note        窗体大小:width*height.
 *
 * @retval      无
 */
void ssd1963_set_window(const struct device *dev,uint16_t sx, uint16_t sy, uint16_t width, uint16_t height)
{
    uint16_t twidth, theight, data[4];
    twidth = sx + width - 1;
    theight = sy + height - 1;

    ssd1963_transmit_command(dev,SSD1963_SET_X_CMD);
    data[0] = sx >> 8;
    data[1] = sx & 0xFF;
    data[2] = twidth >> 8;
    data[3] = twidth & 0xFF;
    ssd1963_transmit_buff(dev,data,4);
    ssd1963_transmit_command(dev,SSD1963_SET_Y_CMD);
    data[0] = sy >> 8;
    data[1] = sy & 0xFF;
    data[2] = theight >> 8;
    data[3] = theight & 0xFF;
    ssd1963_transmit_buff(dev,data,4);
    ssd1963_transmit_command(dev,SSD1963_WRITE_GRAM);
}

/**
 * @brief       LCD加速绘制函数
 * @param       (sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex - sx + 1) * (ey - sy + 1)
 * @param       color:要填充的颜色
 * @retval      无
 */
uint8_t ssd1963_write_color(const struct device *dev,int16_t sx, int16_t sy,int16_t ex, int16_t ey, uint16_t *color)
{
    uint16_t w = ex - sx + 1;
    uint16_t h = ey - sy + 1;
    uint32_t draw_size = w * h;

    ssd1963_set_window(dev, sx, sy, w, h);
    ssd1963_transmit_buff(dev,color, draw_size);
    return 0;
}

uint8_t ssc1963_backlight_gpio_init(const struct device *dev)
{
    int ret;
    const struct ssd1963_config *config = dev->config;

    if(config->backlight_power.port != NULL){
		if (!device_is_ready(config->backlight_power.port)) {
			LOG_ERR("BackLight Power GPIO device not ready");
			return -ENODEV;
		}

		ret = gpio_pin_configure_dt(&config->backlight_power, GPIO_OUTPUT_HIGH);
		if (ret < 0) {
			LOG_ERR("Could not configure reset GPIO (%d)", ret);
			return ret;
		}
        gpio_pin_set_dt(&config->backlight_power, 0);
    }

    if (config->backlight.port != NULL) {
		if (!device_is_ready(config->backlight.port)) {
			LOG_ERR("BackLight GPIO device not ready");
			return -ENODEV;
		}

		ret = gpio_pin_configure_dt(&config->backlight, GPIO_OUTPUT_HIGH);
		if (ret < 0) {
			LOG_ERR("Could not configure reset GPIO (%d)", ret);
			return ret;
		}
        gpio_pin_set_dt(&config->backlight, 0);
	}
    return 0;
}
const struct device *ssd_dev = DEVICE_DT_GET(DT_NODELABEL(fsmc));
static int ssd1963_init(const struct device *dev)
{
    int ret;
    const struct ssd1963_config *config = dev->config;
    struct ssd1963_data *data = dev->data;



    ret = ssc1963_backlight_gpio_init(dev);
    if(ret < 0)
    {
        printk("SSD1963 BackLight Gpio Disable!!");
    }

    printk("ssd1963 Chip ID:%#x\r\n",ssd1963_read_id(dev));

    

    /* reset */
    ssd1963_transmit_command(dev, SSD1963_SOFT_RESET);
    k_msleep(100);

    /* set pll */
    uint16_t pll[DT_PROP_LEN(SSD1963_PROP, pll)] = DT_PROP(SSD1963_PROP, pll);
    ssd1963_transmit_command_buff(dev, SSD1963_SET_PLL_MN, pll, DT_PROP_LEN(SSD1963_PROP, pll));

    /* enable pll */
    ssd1963_transmit_command(dev, SSD1963_SET_PLL);
    ssd1963_transmit_data(dev, 0x01);
    k_msleep(2);

    /* lock pll */
    ssd1963_transmit_command(dev, SSD1963_SET_PLL);
    ssd1963_transmit_data(dev, 0x03);
    k_msleep(100);

    /* set lcd freq */
    int lcd_freq = DT_PROP(SSD1963_PROP, lcd_freq);
    ssd1963_transmit_command(dev, SSD1963_SET_LSHIFT_FREQ);
    ssd1963_transmit_data(dev, (lcd_freq >> 16) & 0xFF);
    ssd1963_transmit_data(dev, (lcd_freq >> 8) & 0xFF);
    ssd1963_transmit_data(dev, lcd_freq & 0xFF);
    k_msleep(100);

    /* set lcd mode */
    ssd1963_transmit_command(dev, SSD1963_SET_LCD_MODE);
    ssd1963_transmit_data(dev, 0x20);
    ssd1963_transmit_data(dev, 0x00);
    ssd1963_transmit_data(dev, ((data->width - 1) >> 8));
    ssd1963_transmit_data(dev, (data->width - 1));
    ssd1963_transmit_data(dev, ((data->height - 1) >> 8));
    ssd1963_transmit_data(dev, (data->height - 1));
    ssd1963_transmit_data(dev, 0x00);

    /* set lcd HORI */
    int HT = data->width + data->hback_porch + data->hfront_porch;
    ssd1963_transmit_command(dev, SSD1963_SET_HORI_PERIOD);
    ssd1963_transmit_data(dev, ((HT - 1) >> 8));
    ssd1963_transmit_data(dev, (HT - 1));
    ssd1963_transmit_data(dev, data->hback_porch >> 8);
    ssd1963_transmit_data(dev, data->hback_porch);
    ssd1963_transmit_data(dev, data->hsync_len - 1);
    ssd1963_transmit_data(dev, 0x00);
    ssd1963_transmit_data(dev, 0x00);
    ssd1963_transmit_data(dev, 0x00);

    /* set lcd VERT */
    int VT = data->height + data->vback_porch + data->vfront_porch;
    ssd1963_transmit_command(dev, SSD1963_SET_VERT_PERIOD);
    ssd1963_transmit_data(dev, ((VT - 1) >> 8));
    ssd1963_transmit_data(dev, (VT - 1));
    ssd1963_transmit_data(dev, data->vback_porch >> 8);
    ssd1963_transmit_data(dev, data->vback_porch);
    ssd1963_transmit_data(dev, data->vsync_len - 1);
    ssd1963_transmit_data(dev, 0x00);
    ssd1963_transmit_data(dev, 0x00);

    /* set lcd pixel format */
    int pixel_format = DT_PROP(SSD1963_PROP, pixel_format);
    ssd1963_transmit_command(dev, SSD1963_SET_PIXEL_DATA_INTERFACE);

    if(pixel_format == 0)
        ssd1963_transmit_data(dev, 0x03);
    else 
        ssd1963_transmit_data(dev, 0x04);
    
    /* set lcd bclk pwm */
    ssd1963_transmit_command(dev, SSD1963_SET_PWM_CONF);
    ssd1963_transmit_data(dev, 0x05);
    ssd1963_transmit_data(dev, 0x00);
    ssd1963_transmit_data(dev, 0x01);
    ssd1963_transmit_data(dev, 0x00);
    ssd1963_transmit_data(dev, 0x00);
    ssd1963_transmit_data(dev, 0x00);

    /* set lcd gpio conf */
    ssd1963_transmit_command(dev, SSD1963_SET_GPIO_CONF);
    ssd1963_transmit_data(dev, 0x03);
    ssd1963_transmit_data(dev, 0x01);

    /* set lcd gpio value */
    ssd1963_transmit_command(dev, SSD1963_SET_GPIO_VALUE);
    ssd1963_transmit_data(dev, 0x01);

    /* set lcd display */
    ssd1963_transmit_command(dev, SSD1963_SET_DISPLAY_ON);

    if(config->backlight_power.port != NULL)
        gpio_pin_set_dt(&config->backlight_power, 1);

    if(config->backlight.port != NULL)
        gpio_pin_set_dt(&config->backlight, 1);

    return 0;
}

static int ssd1963_display_blanking_off(const struct device *dev)
{
	return 0;
}

static int ssd1963_display_blanking_on(const struct device *dev)
{
	return 0;
}

static void *ssd1963_get_framebuffer(const struct device *dev)
{
	LOG_ERR("Direct framebuffer access not supported");
	return NULL;
}

static int ssd1963_display_set_brightness(const struct device *dev,
					const uint8_t brightness)
{
	return 0;
}

static int ssd1963_display_set_contrast(const struct device *dev,
				      const uint8_t contrast)
{
	return 0;
}

static void ssd1963_get_capabilities(const struct device *dev,
				     struct display_capabilities *capabilities)
{

}

static int ssd1963_set_pixel_format(const struct device *dev,
			 const enum display_pixel_format pixel_format)
{
    return 0;
}

static int ssd1963_write(const struct device *dev, const uint16_t x,
			 const uint16_t y,
			 const struct display_buffer_descriptor *desc,
			 const void *buf)
{
    uint16_t *data = (uint16_t *)buf;
    ssd1963_write_color(dev, x, y, x + desc->width - 1, y + desc->height - 1, data);
    return 0;
}

static int ssd1963_read(const struct device *dev, const uint16_t x,
			const uint16_t y,
			const struct display_buffer_descriptor *desc, void *buf)
{
	LOG_ERR("Reading not supported");
	return -ENOTSUP;
}

static const struct display_driver_api ssd1963_api = {
	.blanking_on = ssd1963_display_blanking_off,
	.blanking_off = ssd1963_display_blanking_on,
	.write = ssd1963_write,
	.read = ssd1963_read,
	.get_framebuffer = ssd1963_get_framebuffer,
	.set_brightness = ssd1963_display_set_brightness,
	.set_contrast = ssd1963_display_set_contrast,
	.get_capabilities = ssd1963_get_capabilities,
	.set_pixel_format = ssd1963_set_pixel_format,
};

#define SSD1963_DEVICE(inst)									                \
	static const struct ssd1963_config ssd1963_config = {	                    \
		.cmd_addr = DT_INST_PROP(inst, cmd_addr),			                    \
        .data_addr = DT_INST_PROP(inst, data_addr),                             \
        .backlight = GPIO_DT_SPEC_INST_GET(inst, backlight_gpios),              \
        .backlight_power = GPIO_DT_SPEC_INST_GET(inst, backlight_powers_gpios), \
	};                                                                          \
                                                                                \
	static struct ssd1963_data ssd1963_data = {                                 \
        .width = DT_INST_PROP(inst, width),                                     \
        .height = DT_INST_PROP(inst, height),                                   \
        .hsync_len = DT_INST_PROP(inst, hsync_len),                             \
        .vsync_len = DT_INST_PROP(inst, vsync_len),                             \
        .hback_porch = DT_INST_PROP(inst, hback_porch),                         \
        .vback_porch = DT_INST_PROP(inst, vback_porch),                         \
        .hfront_porch = DT_INST_PROP(inst, hfront_porch),                       \
        .vfront_porch = DT_INST_PROP(inst, vfront_porch),                       \
    };			                                                                \
                                                                                \
DEVICE_DT_INST_DEFINE(0, &ssd1963_init, NULL,		                            \
                &ssd1963_data,				                                    \
                &ssd1963_config,				                                \
                POST_KERNEL,				                                    \
                CONFIG_DISPLAY_INIT_PRIORITY,		                            \
                &ssd1963_api);	                                                \

DT_INST_FOREACH_STATUS_OKAY(SSD1963_DEVICE)

		        
