#include "headerfiles.h"

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
    if (maximum_reconnects == 15)
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