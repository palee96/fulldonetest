#include "headerfiles.h"

// cJSON veriables //

char *string = NULL;
cJSON *name = NULL;
cJSON *led_entry = NULL;
cJSON *spiff_write = NULL;
cJSON *spiff_read = NULL;
cJSON *number = NULL;
cJSON *skill_name = NULL;


char* json_send(){     //Create the JSON object and prep it for sending
cJSON *monitor = cJSON_CreateObject();
    name = cJSON_CreateString("OMGITWORKS");
    led_entry = cJSON_CreateBool(true);
    spiff_write = cJSON_CreateBool(true);
    spiff_read = cJSON_CreateBool(true);
    number = cJSON_CreateNumber(2);

    cJSON_AddItemToObject(monitor,"name",name);
    cJSON_AddItemToObject(monitor,"access",led_entry);
    cJSON_AddItemToObject(monitor,"number",number);
    cJSON_AddItemToObject(monitor, "spiff_write", spiff_write);
    cJSON_AddItemToObject(monitor, "spiff_read", spiff_read);
   
    string = cJSON_Print(monitor);

    cJSON_Delete(monitor);
    return string;
}