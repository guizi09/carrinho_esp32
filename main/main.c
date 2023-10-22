/* 
Project: Remote Car
Start date: 10/21/2023
Author: Guilherme Silva Oliveira / Pedro Augusto Matos Bittencourt Costa
*/

#include <stdio.h> // Biblioteca padrão para entrada/saida
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"


// Definição de algumas variaveis relevantes
#define high 1
#define low 0
#define GPIO19 19

// Função para inicializar o esp
void initialize();

// Função utilizada para definir o nível logico da sáida do esp para controlar a PONT H L298N
void control_wheel(int in1, int in2, int in3, int in4);

// Função utilizada para receber via bluetooth os comandos do controle
void receiver_remote_control();

void app_main(void)
{

    // Configurar o nível de log para DEBUG
    esp_log_level_set("INFO", ESP_LOG_DEBUG);
    ESP_LOGI("INFO","Inicializando a aplicação");

    initialize();
    
    // Função para ler depurar a leitura do nível da porta lógica
    int level;

    for(;;)
    {
        level = high;
        printf("->O nível lógico da porta 16 é: %d\n->Trocando o nível logico.....\n", level);
        gpio_set_level(GPIO19, level);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
        
        level = low;
        printf("->O nível lógico da porta 16 é: %d\n->Trocando o nível logico.....\n", level);
        gpio_set_level(GPIO19, level);
        vTaskDelay(5000 / portTICK_PERIOD_MS);

    }


}


void initialize()
{
    // Comfiguração da porta GPIO19
    gpio_config_t io_config_16;

    io_config_16.pin_bit_mask = (1ULL << GPIO19);
    io_config_16.mode = GPIO_MODE_OUTPUT;
    io_config_16.pull_up_en = GPIO_PULLUP_DISABLE;
    io_config_16.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_config_16.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&io_config_16);

    //Definição do valor lógico inicial das portas
    gpio_set_level(GPIO19, low);
}

void control_wheel(int in1, int in2, int in3, int in4)
{
    printf("Função de controle do motor ainda em andamento");  
}

void receiver_remote_control()
{
    printf("Função de recepção ainda em andamento");
}

