#include "headerfiles.h"

void read_data_from_nvs(){


 nvs_handle_t my_handle;
 size_t required_size_pwd;
 size_t required_size_ssid; 
       esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
        if (err != ESP_OK) {
            printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        } else {
            
        nvs_get_str(my_handle, "__SSID", NULL, &required_size_ssid);
        
        nvs_get_str(my_handle, "__SSID", __SSID, &required_size_ssid);


        nvs_get_str(my_handle, "__PWD", NULL, &required_size_pwd);
        
        nvs_get_str(my_handle, "__PWD", __PWD, &required_size_pwd);

        }
        // Close
        nvs_close(my_handle);

        printf("EZ VAN A SSID_NAMEBEN: %s\n",__SSID);
        printf("EZ VAN A PWD_NAMEBEN: %s\n",__PWD);
        
}
