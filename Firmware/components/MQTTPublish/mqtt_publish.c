/* MQTT Publisher
 *
 *
 */
#include <string.h>

#include "MQTTClient.h"

#include "esp_log.h"

//#include "json_util.h"

/* Constants that aren't configurable in menuconfig */
#define MQTT_SERVER "192.168.178.67" //Raspberry Pi
//#define MQTT_PORT 443
//#define MQTT_PORT 8883
#define MQTT_PORT 1883
#define MQTT_BUF_SIZE 1024
//#define MQTT_WEBSOCKET 0  // 0=no 1=yes

static unsigned char mqtt_sendBuf[MQTT_BUF_SIZE];
static unsigned char mqtt_readBuf[MQTT_BUF_SIZE];

static const char *TAG = "MQTT";

/*
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
static void mqtt_message_handler(MessageData *md) {
	ESP_LOGI(TAG, "Topic received!: %.*s %.*s", md->topicName->lenstring.len, md->topicName->lenstring.data, md->message->payloadlen, (char*)md->message->payload);
}
#pragma GCC diagnostic pop
*/

void mqtt_pub(char *topic, char *msgbuf) {
	
	int ret;
	Network network;

   //while(1) {
					

		ESP_LOGD(TAG, "Start MQTT ...");
		
		MQTTClient client;
		NetworkInit(&network);
		//network.websocket = MQTT_WEBSOCKET;

		ESP_LOGD(TAG,"NetworkConnect %s:%d ...",MQTT_SERVER,MQTT_PORT);
		ret = NetworkConnect(&network, MQTT_SERVER, MQTT_PORT);
		if (ret != 0) {
			ESP_LOGI(TAG, "NetworkConnect not SUCCESS: %d", ret);
			goto exit;
		}
		
		ESP_LOGI(TAG,"MQTTClientInit  ...");
		MQTTClientInit(&client, &network,
			2000,            // command_timeout_ms
			mqtt_sendBuf,         //sendbuf,
			MQTT_BUF_SIZE, //sendbuf_size,
			mqtt_readBuf,         //readbuf,
			MQTT_BUF_SIZE  //readbuf_size
		);

		MQTTString clientId = MQTTString_initializer;
		clientId.cstring = "40b799e0-286e-11eb-8145-7350bf54db8a"; 
		
		MQTTString user = MQTTString_initializer;
		user.cstring= "8dTIp5cjmyv5knqeNaVi";
	
		//MQTTString passw = MQTTString_initializer;
		//passw.cstring = "4588c730edac7125855d56da2da1ab74bb0e";
		
		MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
		data.clientID          = clientId;
		data.username		   = user;
		//data.password		   = passw;
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
		
		MQTTMessage message;
			
		ESP_LOGI(TAG, "MQTTPublish  ... %s",msgbuf);
		message.qos = QOS0;
		message.retained = false;
		message.dup = false;
		message.payload = (void*)msgbuf;
		message.payloadlen = strlen(msgbuf);
			
		int check = strlen(msgbuf);
		printf("%d\n",check);

		ret = MQTTPublish(&client, topic, &message);
		if (ret != SUCCESS) {
			ESP_LOGI(TAG, "MQTTPublish not SUCCESS: %d", ret);
			goto exit;
		}
			
		for(int countdown = 3; countdown >= 0; countdown--) {	
			if(countdown%10==0) {
				ESP_LOGI(TAG, "%d...", countdown);
			}
			vTaskDelay(100 / portTICK_RATE_MS);
		}
	
/*		ESP_LOGI(TAG, "MQTTSubscribe  ...");
		ret = MQTTSubscribe(&client, topic, QOS0, mqtt_message_handler);
		if (ret != SUCCESS) {
			ESP_LOGI(TAG, "MQTTSubscribe: %d", ret);
			goto exit;
		}
		
		ESP_LOGI(TAG, "MQTTYield  ...");
		while(1) {
			ret = MQTTYield(&client, (data.keepAliveInterval+1)*100);
			if (ret != SUCCESS) {
				ESP_LOGI(TAG, "MQTTYield: %d", ret);
				goto exit;
			}
		}*/
	
		exit:
			MQTTDisconnect(&client);
			NetworkDisconnect(&network);
			ESP_LOGI(TAG, "MQTTDisconnect ...");
			for(int countdown = 5; countdown >= 0; countdown--) {
				if(countdown%10==0) {
					ESP_LOGI(TAG, "%d...", countdown);
				}
				vTaskDelay(100 / portTICK_RATE_MS);
			}
			printf("Disconnected!\n");
	//}
}			
