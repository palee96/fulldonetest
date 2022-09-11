#include "headerfiles.h"

/* Initialize Wi-Fi as sta and set scan method */
 void wifi_scan(void)
{
    
    ESP_ERROR_CHECK(esp_netif_init()); //Starts underlying TCP/IP
    ESP_ERROR_CHECK(esp_event_loop_create_default()); //Start default event loop, that is a special type of loop used for system events (Wi-Fi events, for example).
    esp_netif_create_default_wifi_sta(); //Create default API
    
    
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT(); //Initialize wifi with default config
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    
    
    uint16_t number = DEFAULT_SCAN_LIST_SIZE;
    wifi_ap_record_t ap_info[DEFAULT_SCAN_LIST_SIZE];
    uint16_t ap_count = 0;
    memset(ap_info, 0, sizeof(ap_info));
    
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
    
    esp_wifi_scan_start(NULL, true);
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&number, ap_info));
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_count));
    ESP_LOGI(TAG, "Total APs scanned = %u", ap_count);

    
    bool stop = false;
    bool stop_ap = false;
    char tarhely[32];
    strcpy(tarhely,__SSID);
    printf("Ez van a tarhelyben: %s\n",tarhely);
    if (strcmp(tarhely,"")==0)
    {
        printf("No SSID has been saved...starting soft-AP!\n");
        stop = true;
        wifi_init_softap();
    }
    else{
    
    for (size_t j = 0; (j < maximum_retries ) && !stop ; j++)
    {
        for (int i = 0; (i < DEFAULT_SCAN_LIST_SIZE) && (i < ap_count) && !stop; i++) {
        
        ESP_LOGI(TAG, "SSID \t\t%s", ap_info[i].ssid);
        vTaskDelay(10);
        
        if (strcmp((char*)ap_info[i].ssid,tarhely)==0)
        {
            printf("Found saved SSID!\n");
            stop = true;
            stop_ap = true;
            start_wifi_with_nvs();
            break;
        }
        else if (strcmp((char*)ap_info[i].ssid,tarhely)!=0)  
        {
            printf("Continueing search....\n");
            continue;
        }
        
    }
    }
    if (stop_ap==true)
    {
        printf("Stopping search...\n");
    }
    else{
    printf("Didn't find saved SSID, starting soft-AP...\n");
    wifi_init_softap();
    }
      
}
}
