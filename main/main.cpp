#include <stdint.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2s_std.h"
#include "driver/gpio.h"
#include "esp_check.h"
#include "sdkconfig.h"

#include "esp_wifi.h"
#include "nvs_flash.h"
#include "protocol_examples_common.h"
#include "esp_http_server.h"
#include "esp_netif.h"

#include "WebServer.hpp"
#include "I2S_Mic.hpp"
#include "Audio_file_recorder.hpp"

#include "mock_mic.hpp" // For test purposes only

#define BCLK_IO1 GPIO_NUM_4 // I2S bit clock io number
#define WS_IO1 GPIO_NUM_5   // I2S word select io number
#define DIN_IO1 GPIO_NUM_19 // I2S data in io number

#define EXAMPLE_BUFF_SIZE 2048

static i2s_chan_handle_t rx_chan; // I2S rx channel handler

static void wifi_init(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    ESP_ERROR_CHECK(example_connect());
}

extern "C" void app_main(void)
{
    wifi_init();

    // Create and start the web server
    WebServer server;
    server.start();

    i2s_std_gpio_config_t mic_gpio_config{
        .bclk = BCLK_IO1,
        .ws = WS_IO1,
        .din = DIN_IO1,
    };

    I2S_Mic i2s_mic(mic_gpio_config);

    Mock_Mic mock_mic;

    Audio_file_recorder recorder(&mock_mic);

    // The application will run indefinitely
    while (true)
    {
        // vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
