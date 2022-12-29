#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "ssd1306.h"
#define tskNO_AFFINITY  ( 0x7FFFFFFF )
#define tag "SSD1306"


void task_1(void *param);
void task_2(void *param);
static void oled_macro(void *param);

void app_main(void)
{   
    // SSD1306_t dev;
    // i2c_master_init(&dev, CONFIG_SDA_GPIO, CONFIG_SCL_GPIO, CONFIG_RESET_GPIO);
    // ssd1306_init(&dev, 128, 32);

    // ssd1306_clear_screen(&dev, false);
    // ssd1306_contrast(&dev, 0xff);
    // ssd1306_display_text(&dev, 0, "INIT", 4, false);

    xTaskCreatePinnedToCore(oled_macro, "oled_macro", 4096, NULL, 3 ,NULL,1);
    xTaskCreatePinnedToCore(task_2, "task2", 2048, NULL, 4 ,NULL,1);
}

void task_1(void *param)
{   
    SSD1306_t dev;
    i2c_master_init(&dev, CONFIG_SDA_GPIO, CONFIG_SCL_GPIO, CONFIG_RESET_GPIO);
    ssd1306_init(&dev, 128, 32);

    while(1)
    {
        // printf("this is taks1");
        ssd1306_clear_screen(&dev, false);
        ssd1306_contrast(&dev, 0xff);
        ssd1306_display_text(&dev, 0, "this is taks1", 13, false);
    }
    vTaskDelete(NULL);
}

void task_2(void *param)
{
    SSD1306_t dev;
    i2c_master_init(&dev, CONFIG_SDA_GPIO, CONFIG_SCL_GPIO, CONFIG_RESET_GPIO);
    ssd1306_init(&dev, 128, 32);

    while(1)
    {
        // printf("this is taks2");
        ssd1306_clear_screen(&dev, false);
        ssd1306_contrast(&dev, 0xff);
        ssd1306_display_text(&dev, 1, "this is taks2", 13, false);
    }
    vTaskDelete(NULL);
}

static void oled_macro(void *param)
{
    SSD1306_t dev;

    #if CONFIG_I2C_INTERFACE
        ESP_LOGI(tag, "INTERFACE is i2c");
        ESP_LOGI(tag, "CONFIG_SDA_GPIO=%d",CONFIG_SDA_GPIO);
        ESP_LOGI(tag, "CONFIG_SCL_GPIO=%d",CONFIG_SCL_GPIO);
        ESP_LOGI(tag, "CONFIG_RESET_GPIO=%d",CONFIG_RESET_GPIO);
        i2c_master_init(&dev, CONFIG_SDA_GPIO, CONFIG_SCL_GPIO, CONFIG_RESET_GPIO);
    #endif // CONFIG_I2C_INTERFACE

    #if CONFIG_SPI_INTERFACE
        ESP_LOGI(tag, "INTERFACE is SPI");
        ESP_LOGI(tag, "CONFIG_MOSI_GPIO=%d",CONFIG_MOSI_GPIO);
        ESP_LOGI(tag, "CONFIG_SCLK_GPIO=%d",CONFIG_SCLK_GPIO);
        ESP_LOGI(tag, "CONFIG_CS_GPIO=%d",CONFIG_CS_GPIO);
        ESP_LOGI(tag, "CONFIG_DC_GPIO=%d",CONFIG_DC_GPIO);
        ESP_LOGI(tag, "CONFIG_RESET_GPIO=%d",CONFIG_RESET_GPIO);
        spi_master_init(&dev, CONFIG_MOSI_GPIO, CONFIG_SCLK_GPIO, CONFIG_CS_GPIO, CONFIG_DC_GPIO, CONFIG_RESET_GPIO);
    #endif // CONFIG_SPI_INTERFACE

    #if CONFIG_SSD1306_128x32
        ESP_LOGI(tag, "Panel is 128x32");
        ssd1306_init(&dev, 128, 32);
    #endif // CONFIG_SSD1306_128x32

        ssd1306_clear_screen(&dev, false);
        ssd1306_contrast(&dev, 0xff);
    ssd1306_display_text_x3(&dev, 0, "Hello?", 6, false);
    vTaskDelay(3000 / portTICK_PERIOD_MS);

    vTaskDelay(3000 / portTICK_PERIOD_MS);


        ssd1306_clear_screen(&dev, false);
        ssd1306_contrast(&dev, 0xff);
        ssd1306_display_text(&dev, 0, "custom oled", 11, true);
        ssd1306_display_text(&dev, 1, "hope this works", 15, true);
        ssd1306_display_text(&dev, 2, "init!", 5, true);
    
        
    #if 0

    #endif

}