/* MQTT Subscriber
 *
 *
 */
#include <string.h>

#include "MQTTClient.h"
#include "MQTTLinux.h"

#include "esp_log.h"
//#include "esp_system.h"

#include "json_util.h"

#define GL_CTRL_TASK_DELAY_MS 2000

/* Constants that aren't configurable in menuconfig */
#define MQTT_SERVER "192.168.178.67" //Raspberry Pi
#define MQTT_PORT 1883
#define MQTT_BUF_SIZE 1024
//#define MQTT_WEBSOCKET 0  // 0=no 1=yes

static unsigned char mqtt_sendBuf[MQTT_BUF_SIZE];
static unsigned char mqtt_readBuf[MQTT_BUF_SIZE];

static const char *TAG = "MQTT";

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
static void mqtt_message_handler(MessageData *md) {
	ESP_LOGI(TAG, "Topic received!: %.*s %.*s", md->topicName->lenstring.len, md->topicName->lenstring.data, md->message->payloadlen, (char*)md->message->payload);
}
#pragma GCC diagnostic pop

void mqtt_sub(void) {
	
	int ret;
	Network network;

    //while(1) {

		//vTaskDelay(GL_CTRL_TASK_DELAY_MS / portTICK_RATE_MS);

		ESP_LOGD(TAG, "Start MQTT Task ...");

		MQTTClient client;
		NetworkInit(&network);
		//network.websocket = MQTT_WEBSOCKET;

		ESP_LOGD(TAG,"NetworkConnect %s:%d ...",MQTT_SERVER,MQTT_PORT);
		ret = NetworkConnect(&network, MQTT_SERVER, MQTT_PORT);
		if (ret != 0) {
			ESP_LOGI(TAG, "NetworkConnect not SUCCESS: %d", ret);
			goto exit;
		}
		
		ESP_LOGD(TAG,"MQTTClientInit  ...");
		MQTTClientInit(&client, &network,
			2000,            // command_timeout_ms
			mqtt_sendBuf,         //sendbuf,
			MQTT_BUF_SIZE, //sendbuf_size,
			mqtt_readBuf,         //readbuf,
			MQTT_BUF_SIZE  //readbuf_size
		);

		//char buf[30];
		MQTTString clientId = MQTTString_initializer;
//#if defined(MBEDTLS_MQTT_DEBUG)
//        sprintf(buf, "ESP32MQTT");
//#else
//        sprintf(buf, "ESP32MQTT%08X",esp_random());
//#endif
//		ESP_LOGI(TAG,"MQTTClientInit  %s",buf);
        clientId.cstring = "eptecon-client*7227e9c9-1c01-5307-be94-c0ccc64a587f";

		MQTTString username = MQTTString_initializer;
		username.cstring = "client";
		
		MQTTString password = MQTTString_initializer;
		password.cstring = "client";


		MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
		data.clientID          = clientId;
		data.username		   = username;
		data.password		   = password;
		data.willFlag          = 0;
		data.MQTTVersion       = 4; // 3 = 3.1 4 = 3.1.1
		data.keepAliveInterval = 60;
		data.cleansession      = 0;

		ESP_LOGI(TAG,"MQTTConnect  ...");
		ret = MQTTConnect(&client, &data);
		if (ret != SUCCESS) {
			ESP_LOGI(TAG, "MQTTConnect not SUCCESS: %d", ret);
			goto exit;
		}

		ESP_LOGI(TAG, "MQTTSubscribe  ...");
		ret = MQTTSubscribe(&client, "event", QOS0, mqtt_message_handler);
		if (ret != SUCCESS) {
			ESP_LOGI(TAG, "MQTTSubscribe: %d", ret);
			goto exit;
		}
		
		ESP_LOGI(TAG, "MQTTYield  ...");
		while(1) {
			ret = MQTTYield(&client, (data.keepAliveInterval+1)*10000);
			if (ret != SUCCESS) {
				ESP_LOGI(TAG, "MQTTYield: %d", ret);
				goto exit;
			}
		}
		exit:
			MQTTDisconnect(&client);
			NetworkDisconnect(&network);
			ESP_LOGI(TAG, "Starting again!");
    //}
    //esp_task_wdt_delete();
    //vTaskDelete(NULL);
 }
