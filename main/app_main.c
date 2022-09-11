#include "main.h"
#include "used_defines.h"
#include "used_functions.h"
#include "read_data_from_nvs.h"
#include "wifi_scan.h"
#include "start_wifi_with_nvs.h"
#include "soft_ap.h"
#include "mqtt_esp.h"
#include "json_handler.h"
#include "event_handlers.h"
#include "esp_webserver.h"
//Global variables
char __SSID[64];
char __PWD[32];
int maximum_reconnects;

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

    esp_event_handler_register(IP_EVENT,IP_EVENT_AP_STAIPASSIGNED, &start_webserver,&server); //Event handler to start webserver
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL); // Event handler to start mqtt
    esp_event_handler_register(WIFI_EVENT,WIFI_EVENT_STA_DISCONNECTED, &event_handler_err,NULL); // Event handler for sta disconnection
    
}

