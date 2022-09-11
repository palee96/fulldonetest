#include "headerfiles.h"

//Start wifi with saved SSID and PWD 

void start_wifi_with_nvs(){

esp_wifi_set_mode(WIFI_MODE_STA);


    wifi_config_t wifi_internet_config = {};
        strcpy((char*)wifi_internet_config.sta.ssid, __SSID);
        strcpy((char*)wifi_internet_config.sta.password, __PWD);
     
esp_wifi_set_config(WIFI_IF_STA,&wifi_internet_config);
esp_wifi_start();
esp_wifi_connect();
}
