/* Main -  GlueLogics System Core
 *
 *
 *
 */
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_err.h"
#include "esp_log.h"

#include "app_gl_ctrl.h"
#include "app_gl_startup.h"

#include "led_blinker.h"

//#include "uart_async.h"

#include "fw_version.h"


void app_main() {

	ESP_LOGI("FW", "running fw version %s", FW_VERSION);

	xTaskCreatePinnedToCore(&led_blinker_task, "led_task",  1 * 1024, NULL, 6, NULL, 1);

	app_gl_startup();
	
	xTaskCreatePinnedToCore(&app_gl_ctrl_task, "gl_ctrl_task", 12 * 1024, NULL, 5, NULL, 0);
	
	//xTaskCreatePinnedToCore(&tx_uart_task, "tx_uart_task", 2 * 1024, NULL, 5, NULL, 1);
	//xTaskCreatePinnedToCore(&rx_uart_task, "rx_uart_task", 2 * 1024, NULL, 5, NULL, 1);
}