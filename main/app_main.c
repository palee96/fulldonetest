#include "main.h"

#define BROKER_URL ""
#define IO_TOPIC ""
static const char *TAG = "Main";


// cJSON veriables //
char *string = NULL;
cJSON *name = NULL;
cJSON *led_entry = NULL;
cJSON *spiff_write = NULL;
cJSON *spiff_read = NULL;
cJSON *number = NULL;


char* json_send(){     //Create the JSON object and prep it for sending
cJSON *monitor = cJSON_CreateObject();
    name = cJSON_CreateString("Dav");
    led_entry = cJSON_CreateBool(true);
    spiff_write = cJSON_CreateBool(false);
    spiff_read = cJSON_CreateBool(true);
    number = cJSON_CreateNumber(2);

    cJSON_AddItemToObject(monitor,"name",name);
    cJSON_AddItemToObject(monitor,"access",led_entry);
    cJSON_AddItemToObject(monitor,"number",number);
    cJSON_AddItemToObject(monitor, "spiff_write", spiff_write);
    cJSON_AddItemToObject(monitor, "spiff_read", spiff_read);

    string = cJSON_Print(monitor);

    return string;
}

void cb_connection_ok(void *pvParameter){
	ip_event_got_ip_t* param = (ip_event_got_ip_t*)pvParameter;

	/* transform IP to human readable string */
	char str_ip[16];
	esp_ip4addr_ntoa(&param->ip_info.ip, str_ip, IP4ADDR_STRLEN_MAX);

	ESP_LOGI(TAG, "I have a connection and my IP is %s!", str_ip);
}


esp_mqtt_client_handle_t mqtt_client;
int mqtt_connected = 0;

static esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event)
{
    esp_mqtt_client_handle_t client = event->client;
    mqtt_client = event->client;
    int msg_id;

    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            msg_id = esp_mqtt_client_subscribe(client, IO_TOPIC, 0);
            ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
            mqtt_connected = 1;
            ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            mqtt_connected = 0;
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            break;
        default:
            ESP_LOGI(TAG, "Other event id:%d", event->event_id);
            break;
    }
           
    return ESP_OK;
}

static void mqtt_app_start(void)
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .uri = BROKER_URL,
        .event_handle = mqtt_event_handler,
    };

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_start(client);


    int counter = 0;
    char* json_tosend = json_send();
    
    while (counter<2)
    {
        esp_mqtt_client_publish(client, IO_TOPIC, json_tosend, 0, 1, 0);
        vTaskDelay(3000);   
        counter++;
    }
    
}


void app_main()
{
    nvs_flash_init();
    wifi_manager_start();
    wifi_manager_set_callback(WM_EVENT_STA_GOT_IP, &cb_connection_ok);
    vTaskDelay(500);
    mqtt_app_start();   
}
