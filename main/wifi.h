/**
 * @file wifi_app.h
 * @brief Ponto de acesso WiFi (WiFi AP)
 * @version 0.1
 * @date 2022-03-23
 * 
 * @copyright Copyright (c) 2022
 */

#ifndef WIFI_APP_H_
#define WIFI_APP_H_

#include <inttypes.h>

// Configuração do ponto de acesso WIFI



/**
 * @brief Inicia a aplicação Wi-FI
 * 
 * A aplicação Wi-Fi é inicializada com ponto de acesso com comprimento de banda de 20MHz
 * permitindo no máximo 5 dispositivos conectador simultaneamente
 */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct wifi_connection
{
    uint8_t ssid[32];
    uint8_t password[64];
} wifi_connection_t;


void wifi_register_connection(wifi_connection_t);
void wifi_init_sta(void);

#ifdef __cplusplus
}
#endif

#endif //!WIFI_APP_H