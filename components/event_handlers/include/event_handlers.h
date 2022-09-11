#include "extern_values.h"
#include "used_defines.h"

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_netif.h"
#include "nvs_flash.h"
#include "esp_event_loop.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"

#include "cJSON.h"

#include "stdio.h"
#include "esp_mac.h"
#include "esp_event.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include <esp_system.h>
#include <sys/param.h>
#include "esp_eth.h"
#include "esp_http_server.h"
#include "tcpip_adapter.h"
#include <unistd.h>
#include "esp_err.h"


void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
void event_handler_err(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
void mqtt_app_start();
void wifi_init_softap();