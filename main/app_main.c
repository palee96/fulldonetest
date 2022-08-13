#include "main.h"
#include "used_defines.h"
#include "used_functions.h"


char __SSID[64];
char __PWD[32];
 
bool wifi_lost = false;
int maximum_reconnects =0;

void event_handler(void* arg, esp_event_base_t event_base,
                                    int32_t event_id, void* event_data)
{
    if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "Connected with IP Address:" IPSTR, IP2STR(&event->ip_info.ip));
        
        mqtt_app_start();
        
    }
}

 void event_handler_err(void* arg, esp_event_base_t event_base,
                                    int32_t event_id, void* event_data)
{
    wifi_lost = true;
    if (maximum_reconnects == 20)
    {
        printf("Maximum number of reconnects reached...starting soft-AP\n");
        printf("Please check wifi SSID and Password!\n");
        wifi_init_softap();
    }
    else{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        printf("retrying connection.....\n");
        maximum_reconnects++;
        vTaskDelay(500);
        esp_wifi_connect();
    }
    }
       
}




static esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event)
{
    
    
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;

    char* json_tosend = json_send();

    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            msg_id = esp_mqtt_client_subscribe(client, IO_TOPIC, 0);
            esp_mqtt_client_publish(client, IO_TOPIC, json_tosend, 0, 1, 0);
            ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            vTaskDelay(2000);
            esp_mqtt_client_reconnect(client); 
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

void mqtt_app_start()
{

    esp_mqtt_client_config_t mqtt_cfg = {
        .uri = BROKER_URL,
        .event_handle = mqtt_event_handler,
    };

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    
    if (wifi_lost == true)
    {
       ESP_LOGE(TAG,"Lost wifi connection...resending last data....\n");;
       esp_mqtt_client_reconnect(client);
       wifi_lost = false;
    }
    else{
        esp_mqtt_client_start(client);
    }
}

void app_main(void)
{

    static httpd_handle_t server = NULL;

//Initialize NVS to store data
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }

    ESP_ERROR_CHECK(ret);
    
    read_data_from_nvs();

    wifi_scan();

    esp_event_handler_register(IP_EVENT,IP_EVENT_AP_STAIPASSIGNED,&start_webserver,&server); //Event handler to start webserver
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL); // Event handler to start mqtt
    esp_event_handler_register(WIFI_EVENT,WIFI_EVENT_STA_DISCONNECTED, &event_handler_err,NULL); // Event handler for sta disconnection
    
}

