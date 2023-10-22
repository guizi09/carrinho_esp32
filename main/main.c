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

// Função para os parâmetros que inicializam o esp
void initialize();

// Função utilizada para definir o nível logico da sáida do esp para controlar a PONT H L298N
void control_wheel(int in1, int in2, int in3, int in4);

// Função utilizada para receber via bluetooth os comandos do controle
void receiver_remote_control();

void app_main(void)
{
    initialize();

    for(;;)
    {
        receiver_remote_control();
    }

    return
}

void control_wheel(int in1, int in2, int in3, int in4)
{

    return
}

void receiver_remote_control()
{
    return
}

void initialize()
{
    return
}