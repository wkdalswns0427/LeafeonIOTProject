/* UART Echo Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_log.h"


#include "ssd1306.h"//
#include "font8x8_basic.h"//

/**
 * This is an example which echos any data it receives on configured UART back to the sender,
 * with hardware flow control turned off. It does not use UART driver event queue.
 *
 * - Port: configured UART
 * - Receive (Rx) buffer: on
 * - Transmit (Tx) buffer: off
 * - Flow control: off
 * - Event queue: off
 * - Pin assignment: see defines below (See Kconfig)
 */

#define ECHO_TEST_TXD (CONFIG_EXAMPLE_UART_TXD)
#define ECHO_TEST_RXD (CONFIG_EXAMPLE_UART_RXD)
#define ECHO_TEST_RTS (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS (UART_PIN_NO_CHANGE)

#define ECHO_UART_PORT_NUM      (CONFIG_EXAMPLE_UART_PORT_NUM)
#define ECHO_UART_BAUD_RATE     (CONFIG_EXAMPLE_UART_BAUD_RATE)
#define ECHO_TASK_STACK_SIZE    (CONFIG_EXAMPLE_TASK_STACK_SIZE)

static const char *TAG = "UART TEST";

#define tag "SSD1306"//

#define BUF_SIZE (1024)

static void builtin_oled(void *parameter);//
static void echo_task(void *arg);

void app_main(void)
{
    xTaskCreate(echo_task, "uart_echo_task", ECHO_TASK_STACK_SIZE, NULL, 10, NULL);
    xTaskCreate(builtin_oled, "builtin_oled_taask", ECHO_TASK_STACK_SIZE, NULL, 10, NULL);
}

// task 1
static void echo_task(void *arg)
{
    /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    uart_config_t uart_config = {
        .baud_rate = ECHO_UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    int intr_alloc_flags = 0;

#if CONFIG_UART_ISR_IN_IRAM
    intr_alloc_flags = ESP_INTR_FLAG_IRAM;
#endif

    ESP_ERROR_CHECK(uart_driver_install(ECHO_UART_PORT_NUM, BUF_SIZE * 2, 0, 0, NULL, intr_alloc_flags));
    ESP_ERROR_CHECK(uart_param_config(ECHO_UART_PORT_NUM, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(ECHO_UART_PORT_NUM, ECHO_TEST_TXD, ECHO_TEST_RXD, ECHO_TEST_RTS, ECHO_TEST_CTS));

    // Configure a temporary buffer for the incoming data
    uint8_t *data = (uint8_t *) malloc(BUF_SIZE);

    while (1) {
        // Read data from the UART
        int len = uart_read_bytes(ECHO_UART_PORT_NUM, data, (BUF_SIZE - 1), 20 / portTICK_RATE_MS);
        // Write data back to the UART
        uart_write_bytes(ECHO_UART_PORT_NUM, (const char *) data, len);
        if (len) {
            data[len] = '\0';
            ESP_LOGI(TAG, "Recv str: %s", (char *) data);
        }
    }
}


// task 2
static void builtin_oled(void *parameter){
    SSD1306_t dev;
	int center, top, bottom;
	char lineChar[20];
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

    #if CONFIG_FLIP
        dev._flip = true;
        ESP_LOGW(tag, "Flip upside down");
    #endif

    #if CONFIG_SSD1306_128x64
        ESP_LOGI(tag, "Panel is 128x64");
        ssd1306_init(&dev, 128, 64);
    #endif // CONFIG_SSD1306_128x64
    #if CONFIG_SSD1306_128x32
        ESP_LOGI(tag, "Panel is 128x32");
        ssd1306_init(&dev, 128, 32);
    #endif // CONFIG_SSD1306_128x32

        ssd1306_clear_screen(&dev, false);
        ssd1306_contrast(&dev, 0xff);
    ssd1306_display_text_x3(&dev, 0, "Hello", 5, false);
    vTaskDelay(3000 / portTICK_PERIOD_MS);

    #if CONFIG_SSD1306_128x64
        top = 2;
        center = 3;
        bottom = 8;
        ssd1306_display_text(&dev, 0, "SSD1306 128x64", 14, false);
        ssd1306_display_text(&dev, 1, "ABCDEFGHIJKLMNOP", 16, false);
        ssd1306_display_text(&dev, 2, "abcdefghijklmnop",16, false);
        ssd1306_display_text(&dev, 3, "Hello World!!", 13, false);
        //ssd1306_clear_line(&dev, 4, true);
        //ssd1306_clear_line(&dev, 5, true);
        //ssd1306_clear_line(&dev, 6, true);
        //ssd1306_clear_line(&dev, 7, true);
        ssd1306_display_text(&dev, 4, "SSD1306 128x64", 14, true);
        ssd1306_display_text(&dev, 5, "ABCDEFGHIJKLMNOP", 16, true);
        ssd1306_display_text(&dev, 6, "abcdefghijklmnop",16, true);
        ssd1306_display_text(&dev, 7, "Hello World!!", 13, true);
    #endif // CONFIG_SSD1306_128x64

    #if CONFIG_SSD1306_128x32
        top = 1;
        center = 1;
        bottom = 4;
        ssd1306_display_text(&dev, 0, "SSD1306 128x32", 14, false);
        ssd1306_display_text(&dev, 1, "Hello World!!", 13, false);
        //ssd1306_clear_line(&dev, 2, true);
        //ssd1306_clear_line(&dev, 3, true);
        ssd1306_display_text(&dev, 2, "SSD1306 128x32", 14, true);
        ssd1306_display_text(&dev, 3, "Hello World!!", 13, true);
    #endif // CONFIG_SSD1306_128x32
        vTaskDelay(3000 / portTICK_PERIOD_MS);

    while(true){
        
        // Display Count Down
        uint8_t image[24];
        memset(image, 0, sizeof(image));
        ssd1306_display_image(&dev, top, (6*8-1), image, sizeof(image));
        ssd1306_display_image(&dev, top+1, (6*8-1), image, sizeof(image));
        ssd1306_display_image(&dev, top+2, (6*8-1), image, sizeof(image));
        for(int font=0x39;font>0x30;font--) {
            memset(image, 0, sizeof(image));
            ssd1306_display_image(&dev, top+1, (7*8-1), image, 8);
            memcpy(image, font8x8_basic_tr[font], 8);
            if (dev._flip) ssd1306_flip(image, 8);
            ssd1306_display_image(&dev, top+1, (7*8-1), image, 8);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
        
        // Scroll Up
        ssd1306_clear_screen(&dev, false);
        ssd1306_contrast(&dev, 0xff);
        ssd1306_display_text(&dev, 0, "---Scroll  UP---", 16, true);
        //ssd1306_software_scroll(&dev, 7, 1);
        ssd1306_software_scroll(&dev, (dev._pages - 1), 1);
        for (int line=0;line<bottom+10;line++) {
            lineChar[0] = 0x01;
            sprintf(&lineChar[1], " Line %02d", line);
            ssd1306_scroll_text(&dev, lineChar, strlen(lineChar), false);
            vTaskDelay(500 / portTICK_PERIOD_MS);
        }
        vTaskDelay(3000 / portTICK_PERIOD_MS);
        
        // Scroll Down
        ssd1306_clear_screen(&dev, false);
        ssd1306_contrast(&dev, 0xff);
        ssd1306_display_text(&dev, 0, "--Scroll  DOWN--", 16, true);
        //ssd1306_software_scroll(&dev, 1, 7);
        ssd1306_software_scroll(&dev, 1, (dev._pages - 1) );
        for (int line=0;line<bottom+10;line++) {
            lineChar[0] = 0x02;
            sprintf(&lineChar[1], " Line %02d", line);
            ssd1306_scroll_text(&dev, lineChar, strlen(lineChar), false);
            vTaskDelay(500 / portTICK_PERIOD_MS);
        }
        vTaskDelay(3000 / portTICK_PERIOD_MS);

        // Page Down
        ssd1306_clear_screen(&dev, false);
        ssd1306_contrast(&dev, 0xff);
        ssd1306_display_text(&dev, 0, "---Page	DOWN---", 16, true);
        ssd1306_software_scroll(&dev, 1, (dev._pages-1) );
        for (int line=0;line<bottom+10;line++) {
            //if ( (line % 7) == 0) ssd1306_scroll_clear(&dev);
            if ( (line % (dev._pages-1)) == 0) ssd1306_scroll_clear(&dev);
            lineChar[0] = 0x02;
            sprintf(&lineChar[1], " Line %02d", line);
            ssd1306_scroll_text(&dev, lineChar, strlen(lineChar), false);
            vTaskDelay(500 / portTICK_PERIOD_MS);
        }
        vTaskDelay(3000 / portTICK_PERIOD_MS);

        // Horizontal Scroll
        ssd1306_clear_screen(&dev, false);
        ssd1306_contrast(&dev, 0xff);
        ssd1306_display_text(&dev, center, "Horizontal", 10, false);
        ssd1306_hardware_scroll(&dev, SCROLL_RIGHT);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
        ssd1306_hardware_scroll(&dev, SCROLL_LEFT);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
        ssd1306_hardware_scroll(&dev, SCROLL_STOP);
        
        // Vertical Scroll
        ssd1306_clear_screen(&dev, false);
        ssd1306_contrast(&dev, 0xff);
        ssd1306_display_text(&dev, center, "Vertical", 8, false);
        ssd1306_hardware_scroll(&dev, SCROLL_DOWN);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
        ssd1306_hardware_scroll(&dev, SCROLL_UP);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
        ssd1306_hardware_scroll(&dev, SCROLL_STOP);
        
        // Invert
        ssd1306_clear_screen(&dev, true);
        ssd1306_contrast(&dev, 0xff);
        ssd1306_display_text(&dev, center, "  Good Bye!!", 12, true);
        vTaskDelay(5000 / portTICK_PERIOD_MS);


        // Fade Out
        ssd1306_fadeout(&dev);
        
    #if 0
        // Fade Out
        for(int contrast=0xff;contrast>0;contrast=contrast-0x20) {
            ssd1306_contrast(&dev, contrast);
            vTaskDelay(40);
        }
    #endif

    }



	// esp_restart();
}