/* Controller -  GlueLogics System Business Logic
 *
 *
 *
 */
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "esp_system.h"

#include "driver/uart.h"
#include "soc/uart_struct.h"
#include "string.h"

#include "mqtt_publish.h"
#include "mqtt_subscribe.h"

#include "led_blinker.h"

//#include "uart_async.h"

#include "json_util.h"


#define LONG_PRESS_PERIOD_MS 100
#define BTN_SENSE_BIT BIT0
#define KEY_GPIO_NUM 27
#define KEY_ACTIVE 1
#define GLITCH_TIMEOUT_MS 500

#define SERIAL_NUM "1001-00001"
#define FW_VERSION "1.01.00a"
#define CLIENT_NAME "test_client"
#define DEVICE_TYPE "SmartScale"
#define DEVICE_ID "Scale001"


#define TOPIC1 "v1/devices/me/attributes"
#define TOPIC2 "v1/devices/me/telemetry"

static const int RX_BUF_SIZE = 1024;

#define TXD_PIN (GPIO_NUM_4)
#define RXD_PIN (GPIO_NUM_5)

static EventGroupHandle_t button_event_group;

static const char *TAG = "gl_ctrl";

static void IRAM_ATTR gpio_isr_handler_up(void* arg) {
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	xEventGroupSetBitsFromISR(button_event_group, BTN_SENSE_BIT, &xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR();
}

void init_gpio(void) {
	/*
	 * Setup button GPIO
	 */
	gpio_config_t io_conf = {
		.pin_bit_mask = 1 << KEY_GPIO_NUM,
		.mode = GPIO_MODE_INPUT,
		.pull_down_en = GPIO_PULLDOWN_ENABLE,
		.pull_up_en = GPIO_PULLUP_DISABLE,
		.intr_type = GPIO_INTR_POSEDGE,
	};
	gpio_config(&io_conf);
	printf("Button configured\n");
	
	gpio_intr_enable(KEY_GPIO_NUM);
	gpio_install_isr_service(0);
	gpio_isr_handler_add(KEY_GPIO_NUM, gpio_isr_handler_up, (void*) KEY_GPIO_NUM);
}


void init_uart() {
    const uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    // We won't use a buffer for sending data.
     uart_driver_install(UART_NUM_1, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
}

// This function seperates the single input string in to numFields substrings
void ParseFields(char* inputBuffer, char** pFields, uint32_t numFields, char* delimiterChars)
{
    char* pString = inputBuffer;
    char* pField;
    
    for(uint32_t i=0; i<numFields; i++)
    {
        pField = strtok(pString, delimiterChars);
 
        if(pField != NULL)
        {
            pFields[i] = pField;
        }
        else
        {
            pFields[i] = "";
        }
 
        pString = NULL; //to make strtok continue parsing the next field rather than start again on the original string (see strtok documentation for more details)
    }
}

void app_gl_ctrl_task(void *arg) {
	
	/*Task variables*/
	//EventBits_t bits;
	//button_event_group = xEventGroupCreate();
	
	/*Init functions*/
	init_gpio();
	init_uart();
		
	/*Uart variables*/	
	static const char *RX_TASK_TAG = "RX_TASK";
    esp_log_level_set(RX_TASK_TAG, ESP_LOG_INFO);
    uint8_t* data = (uint8_t*) malloc(RX_BUF_SIZE+1);
	
	/*Message variables*/
	char *msg1, *msg2;
	char dTimeStamp[32], dWeight[32], dUnit[32], dLocalTemp[32], dGlobalTemp[32], dBattVolt[32];
	//uint8_t count=0;
	//char batt_value[8];
	char measurement[32];
	char *p[5];

	//sprintf(batt_value, "%d",(uint8_t)(esp_random()&0xFF));
		
	/*Create JSON Object*/
	msg1 = json_create_attributes(DEVICE_ID, DEVICE_TYPE, CLIENT_NAME, FW_VERSION, SERIAL_NUM);
	printf("%s\n",msg1);
	
	/*Send attributes*/
	mqtt_pub(TOPIC1, msg1);
	

    while(1) {
		
		const int rxBytes = uart_read_bytes(UART_NUM_1, data, RX_BUF_SIZE, 1000 / portTICK_RATE_MS);
       
		led_blinker_set_mode(BLINK_MODE_CONNECTED);
		vTaskDelay(1000 / portTICK_RATE_MS);
		ESP_LOGI(TAG, "Waiting for measurement...");
		
		if (rxBytes > 0) {
            data[rxBytes] = 0;
            ESP_LOGI(RX_TASK_TAG, "Read %d bytes: '%s'", rxBytes, data);
            ESP_LOG_BUFFER_HEXDUMP(RX_TASK_TAG, data, rxBytes, ESP_LOG_INFO);
			
			/*Parse data*/
			
		//	ESP_LOGI(TAG, "give data: '%s'", data);
			
			sprintf(measurement, "%s",data);
			
		//	ESP_LOGI(TAG, "give data string: '%s'", measurement);
			
			ParseFields(measurement, p, 5, ",");
			
		/*	ESP_LOGI(TAG, "pars1: '%s'", p[0]);
			ESP_LOGI(TAG, "pars1: '%s'", p[1]);
			ESP_LOGI(TAG, "pars1: '%s'", p[2]);
			ESP_LOGI(TAG, "pars1: '%s'", p[3]);
			ESP_LOGI(TAG, "pars1: '%s'", p[4]);*/
			
			sprintf(dTimeStamp, "%s", p[0]);
			sprintf(dWeight,"%s", p[1]);
			sprintf(dUnit,"%s", p[2]);
			sprintf(dLocalTemp,"%s", p[3]);
			sprintf(dGlobalTemp,"%s", p[4]);
			sprintf(dBattVolt, "%d",(uint8_t)(esp_random()&0xFF));
			
			/*Create JSON Object*/
			msg2 = json_update_telemetry(dTimeStamp, dWeight, dUnit, dLocalTemp, dGlobalTemp, dBattVolt);
			printf("%s\n",msg2);
			
			mqtt_pub(TOPIC2, msg2);
			vTaskDelay(100 / portTICK_RATE_MS);		
			
        }

    }
	free(data);
}
 
 
