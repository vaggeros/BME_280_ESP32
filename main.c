#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "bme280.h"
#include "driver/gpio.h"


#define LED 33
#define LED_BLINKING_DELAY 400

bme280_measurement_t reading;

 bme280_config_t bme0 = {
            .sda_pin = GPIO_NUM_21,
            .scl_pin = GPIO_NUM_22,
            .i2c_instance = I2C_NUM_1
    };


//---------------------------------------------------------------------------
void esp32_delay_sec(int time)
{
    vTaskDelay(time * 1000 / portTICK_PERIOD_MS);
}

//--------------------------------------------------------------------------
void bme280_init()
{
    bme280_setup(&bme0);
}

//--------------------------------------------------------------------------
void esp32_gpio_init()
{
  gpio_pad_select_gpio(LED);
  gpio_set_direction(LED, GPIO_MODE_OUTPUT);
}

//--------------------------------------------------------------------------
void esp32_led_blink()
{
  int i;
  for (i=0;i<3;i++) 
  {
    gpio_set_level(LED,1);
    vTaskDelay(LED_BLINKING_DELAY / portTICK_PERIOD_MS);
    gpio_set_level(LED,0);
    vTaskDelay(LED_BLINKING_DELAY / portTICK_PERIOD_MS);
  }
}

//--------------------------------------------------------------------------
void app_main()
{
    esp32_gpio_init();
    esp32_led_blink();
    bme280_init();

    for (;;)
    {
        bme280_make_measurement(&bme0, &reading);
        bme280_print_measurement(&reading);
        esp32_delay_sec(10);
    }
}
