/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

#include "wifi.h"
#include <nvs_flash.h>
#include "mqtt_client.h"
#include <esp_log.h>

static const char *TAGM = "MQTT_EXAMPLE";

#define WIFI_SSID       ""
#define WIFI_PASSWORD   ""

/** @brief IoT HUB name     */ 
#define IOTHUB_NAME     "iot-hub-name"
/** @brief your device id   */ 
#define IOTHUB_DEVID    "iot-device"
/** @brief entire SaS Token including SharedAccessSignature sr= ... */ 
#define IOTHUB_KEY      "sas-token"

esp_mqtt_client_handle_t client;


static void log_error_if_nonzero(const char * message, int error_code)
{
    if (error_code != 0) {
        ESP_LOGE(TAGM, "Last error %s: 0x%x", message, error_code);
    }
}

static esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event)
{
    esp_log_level_set("*", ESP_LOG_INFO);
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    // your_context_t *context = event->context;
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAGM, "MQTT_EVENT_CONNECTED");
            msg_id = esp_mqtt_client_publish(client, "devices/" IOTHUB_DEVID "/messages/events/", "hello_world", 0, 1, 1);
            ESP_LOGI(TAGM, "sent publish successful, msg_id=%d", msg_id);

            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAGM, "MQTT_EVENT_DISCONNECTED");
            break;

        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAGM, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
            ESP_LOGI(TAGM, "sent publish successful, msg_id=%d", msg_id);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAGM, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAGM, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAGM, "MQTT_EVENT_DATA");
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAGM, "MQTT_EVENT_ERROR");
            if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
                log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
                log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
                log_error_if_nonzero("captured as transport's socket errno",  event->error_handle->esp_transport_sock_errno);
                ESP_LOGI(TAGM, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));

            }
            break;
        default:
            ESP_LOGI(TAGM, "Other event id:%d", event->event_id);
            break;
    }
    return ESP_OK;
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    ESP_LOGD(TAGM, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    mqtt_event_handler_cb((esp_mqtt_event_handle_t) event_data);
}

static void mqtt_app_start(void)
{
    ESP_LOGD(TAGM, "inicializando cliente mqtt");
    const char* username = IOTHUB_NAME".azure-devices.net/"IOTHUB_DEVID"/?api-version=2021-04-12";
    ESP_LOGD(TAGM, "mqtt username: %s", username);
    esp_mqtt_client_config_t mqtt_cfg = {
        .uri = "mqtts://"IOTHUB_NAME".azure-devices.net",
        .port=8883,
        .username = username,
        .client_id = IOTHUB_DEVID,
        .password = IOTHUB_KEY,
        .keepalive = 120,
    };

    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
}


void app_main(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
        ESP_LOGE("main", "-> NVS ERASED");
    }
    ESP_ERROR_CHECK(ret);
    
    wifi_connection_t conn = {WIFI_SSID, WIFI_PASSWORD };
    wifi_register_connection(conn);    

    /// start WiFi station
    wifi_init_sta();
    /// Start mqtt
    mqtt_app_start(); 
}
