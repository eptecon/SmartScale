/* JSON Utility
 *
 *
 * Copyright (C) Copyright 2018 eptecon, Apache 2.0 License.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "json_util.h"

/* defines */
/* structs */
//struct {
//	char deviceID[255];
//	char typeClass[255];
//	char clientID[255];
//	char FWversion[255];
//	char serialNumber[255];
	//double latitude;
	//double longitude;
//}device_attributes;

/* constants */


/* variables */
cJSON *root;
char *out;


char* json_create(char *value1) {
	//char *out;	
	//cJSON *root;

	root  = cJSON_CreateObject();

	cJSON_AddItemToObject(root, "deviceID", cJSON_CreateString(value1));
	//cJSON_AddItemToObject(root, "batteryVoltage", cJSON_CreateString(value2));
	//cJSON_AddItemToObject(root, "clickType", cJSON_CreateString(value3));

	out = cJSON_Print(root);
	//printf("%s\n",out);
	
	return out;
}/*JSON_create*/

char* json_create_attributes(char *value1, char *value2, char *value3, char *value4, char *value5)
{
	//char *out;	
	//cJSON *root;
	//struct device_attributes device;
	
	//strcpy(device_attributes.deviceID, value1);
	//strcpy(device_attributes.typeClass, value1);
	//strcpy(device_attributes.clientID, value1);
	//strcpy(device_attributes.FWversion, value1);
	//strcpy(device_attributes.serialNumber, value1);
	

	root  = cJSON_CreateObject();
	
	cJSON_AddItemToObject(root, "deviceID", cJSON_CreateString(value1));
	cJSON_AddItemToObject(root, "typeClass", cJSON_CreateString(value2));
	cJSON_AddItemToObject(root, "clientID", cJSON_CreateString(value3));
	cJSON_AddItemToObject(root, "FWversion", cJSON_CreateString(value4));
	cJSON_AddItemToObject(root, "serialNumber", cJSON_CreateString(value5));

	out = cJSON_Print(root);
	//printf("%s\n",out);
	
	return out;
}/*JSON_create_attributes*/

char* json_update_telemetry(char *value1, char *value2, char *value3, char *value4, char *value5, char *value6) {
	//char *out;	
	//cJSON *obj;
	
	//root  = cJSON_Parse(obj);
	root  = cJSON_CreateObject();
	
	cJSON_AddItemToObject(root, "timeStamp", cJSON_CreateString(value1));
	cJSON_AddItemToObject(root, "weightValue", cJSON_CreateString(value2));
	cJSON_AddItemToObject(root, "weightUnits", cJSON_CreateString(value3));
	cJSON_AddItemToObject(root, "localTemperature", cJSON_CreateString(value4));
	cJSON_AddItemToObject(root, "globalTemperature", cJSON_CreateString(value5));
	cJSON_AddItemToObject(root, "batteryVoltage", cJSON_CreateString(value6));

	out = cJSON_Print(root);
	//printf("%s\n",out);
		
	return out;
}/*JSON_update_telemetry*/
