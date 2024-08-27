/*************************************************************************
* Description: Programa de para recibir un mensaje desde un modulo
* RYLR998 LORA y transmitirlo a traves de una conexion socket
* Author:      Ing. Misael Salazar
* File:        main.c
* Target:      ESP32
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "freertos/FreeRTOS.h"   
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"

#include "esp_wifi.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "esp_log.h"

// librerias para conexion socket
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define LED_BUILTIN 2
#define LED_ON      1
#define LED_OFF     0

#define TX0_PIN (GPIO_NUM_1)
#define RX0_PIN (GPIO_NUM_3)

#define TX2_PIN (GPIO_NUM_17)
#define RX2_PIN (GPIO_NUM_16)

#define SSID        "bluetele8857"
#define PASSWORD    "2C9549E3"
#define IP_SERVIDOR "192.168.0.193"
#define PORT        9999

static const char *TAG = "ESP LOG";
const TickType_t xDelay = 500 / portTICK_PERIOD_MS;
char arrayMsjsocket[1024] = {0};

static void uart2_task(void);
static void uart0_init(const uint32_t baudrate);
static void uart2_init(const uint32_t baudrate);

void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id,void *event_data);
void wifi_connection();

void send_message_socket(const char* msg);
//void send_message_socket();

bool IsNetworkConnect = false;

/*************************************************************************
Task para el led blink
return void
*************************************************************************/
static void task_ledblink()
{
    while (1)
    {        
        gpio_set_level(LED_BUILTIN, LED_ON);
        vTaskDelay(xDelay);
        gpio_set_level(LED_BUILTIN, LED_OFF);
        vTaskDelay(xDelay);                                                                       
    }
}

/*************************************************************************
Task para monitorear el rx del UART2
return void
*************************************************************************/
static void uart2_task(void)
{
    uint8_t *datarx = (uint8_t *) malloc(1024);

    while (1)
    {
        int len = uart_read_bytes(UART_NUM_2, datarx, (1024 - 1), 20 / portTICK_PERIOD_MS);        
		
        if (len > 0)
        {   
            datarx[len] = '\0';         
            sprintf(arrayMsjsocket, (const char *)datarx);
            arrayMsjsocket[1024] = '\0';
            ESP_LOGI(TAG, "Enviando mensaje por socket");
            // uart_write_bytes(UART_NUM_0, (const char *)datarx, len);           // LOG
            
            uart_write_bytes(UART_NUM_0, arrayMsjsocket, 1024);
			send_message_socket(arrayMsjsocket);
            memset(arrayMsjsocket, 0, 1024);
        }
    }
}

/*************************************************************************
Handler de eventos del wifi
return void
*************************************************************************/
void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id,void *event_data)
{
    if (event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        // se intenta reconectar
        IsNetworkConnect = false;
        ESP_LOGI(TAG, "Conexion perdida...");
        esp_wifi_connect();
    }
    else if (event_id == IP_EVENT_STA_GOT_IP)
    {
        IsNetworkConnect = true;
        ESP_LOGI(TAG, "Conectado a la red");
    }
}

/*************************************************************************
Se conecta a la red
return void
*************************************************************************/
void wifi_connection()
{        
	uart_write_bytes(UART_NUM_0, "Iniciando wifi...", strlen("Iniciando wifi..."));           // LOG
    esp_netif_init();                    
    esp_event_loop_create_default();     
    esp_netif_create_default_wifi_sta(); 
    wifi_init_config_t wifi_initiation = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_initiation); //
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL);    // se agrega el handler
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler, NULL);
    
    wifi_config_t wifi_configuration = {
        .sta = {
            .ssid = SSID,
            .password = PASSWORD,            
           }    
        };
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_configuration);
    // 3 - Wi-Fi Start Phase
    esp_wifi_start();
    esp_wifi_set_mode(WIFI_MODE_STA);
    // 4- Wi-Fi Connect Phase
    esp_wifi_connect();
}

/*************************************************************************
Se envia el mensaje al socket
return void
*************************************************************************/
void send_message_socket(const char* msg)
{   
    int puerto, conexion;
    struct sockaddr_in cliente;                                // Declaración de la estructura con información para la conexion
    struct hostent *servidor;                                  // Declaración de la estructura con información del host
    char buffer[1024];
    
    puerto = PORT;
    servidor = gethostbyname(IP_SERVIDOR);                     // Asignacion de IP de servidor
    conexion = socket(AF_INET, SOCK_STREAM, 0);                // Asignación del socket
    if (conexion < 0)
    {
        ESP_LOGI(TAG, "Error al crear el socket");
        return;
    }

    memset(&cliente, 0, sizeof(cliente));
    cliente.sin_family = AF_INET;                               // asignacion del protocolo
    cliente.sin_port = htons(puerto);                           // asignacion del puerto
    bcopy((char *)servidor->h_addr, (char *)&cliente.sin_addr.s_addr, sizeof(servidor->h_length));

    int resultconnect = connect(conexion, (struct sockaddr *)&cliente, sizeof(cliente));          // se conecta al socket
    if (resultconnect != 0)
    {
        ESP_LOGI(TAG, "Error al conectarse al socket");
        return;
    }

    sprintf(buffer, msg);
    send(conexion, buffer, 1024, 0);                                           //envio
    close(conexion);                                                          // se cierra el socket
}

/*************************************************************************
Funcion para inicializar el uart0
return void
*************************************************************************/
void uart0_init(const uint32_t baudrate)
{   
    uart_config_t uart_config = {
        .baud_rate = baudrate,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };

    ESP_ERROR_CHECK(uart_param_config(UART_NUM_0, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_0, TX0_PIN, RX0_PIN, 0, 0));
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_0, 1024, 1024, 0, NULL, 0));
}

/*************************************************************************
Funcion para inicializar el uart2
return void
*************************************************************************/
void uart2_init(const uint32_t baudrate)
{
    uart_config_t uart_config = {
        .baud_rate = baudrate,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };
	
    ESP_ERROR_CHECK(uart_param_config(UART_NUM_2, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_2, TX2_PIN, RX2_PIN, 0, 0));
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_2, 1024, 1024, 0, NULL, 0));	
}

void app_main(void)
{
	uart_write_bytes(UART_NUM_0, "Iniciando sistema", strlen("Iniciando sistema"));           // LOG
	nvs_flash_init();
    gpio_set_direction(LED_BUILTIN, GPIO_MODE_OUTPUT);        // se define el pin 2 como salida
    uart0_init(9600);
    uart2_init(9600);
    
    // xTaskCreate(uart0_task, "uart0_task", 4096, NULL, 9, NULL);
	xTaskCreate(task_ledblink, "task_ledblink", 2048, NULL, 11, NULL);
    xTaskCreate(uart2_task, "uart2_task", 4096, NULL, 10, NULL);
	wifi_connection();
}