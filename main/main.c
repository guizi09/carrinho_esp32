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
#include "driver/ledc.h" // Biblioteca para PWM
#include "D:\Projetos\05_esp\09_fic\carrinho_esp32\components\ps3\src\include\ps3.h"




// Definição de algumas variaveis relevantes
#define true 1
#define false 0
#define high 1
#define low 0

// Definição do número das portas
#define gpioIn1_l298 33
#define gpioIn2_l298 25

#define gpioPWM_servo 27


// Definição das caracteristicaas do PWM

#define LEDC_HS_TIMER    LEDC_TIMER_0
#define LEDC_HS_MODE     LEDC_HIGH_SPEED_MODE
#define LEDC_HS_CH0_CHANNEL  LEDC_CHANNEL_0
#define LEDC_HS_CH1_CHANNEL  LEDC_CHANNEL_1
#define LEDC_HS_CH2_CHANNEL  LEDC_CHANNEL_2

// Definição de Variaveis Auxiliares
char PWM_servo_duty= 0;     //duty cycle do servo (TEM QUE SER ENTRE 5% E 10%)
char PWM_motor_duty= 0;     //duty cycle do motor
char driving_mode= 0;       // 0=parado, 1= pra frente, 2= pra tras




// Função para inicializar o esp
void initialize();

// Função utilizada para definir o nível logico da sáida do esp para controlar a PONT H L298N
void control_wheel(int In1_l298, int In2_l298, float PWM_l298, float PWM_servo);

// Função utilizada para receber via bluetooth os comandos do controle
void receiver_remote_control(ps3_t ps3, ps3_event_t event);


void app_main(void)
{

    // Configurar o nível de log para DEBUG
    esp_log_level_set("INFO", ESP_LOG_DEBUG);
    ESP_LOGI("INFO","Inicializando a aplicação");

    initialize();
    
    //Configurações da biblioteca PS3
    ps3SetEventCallback(receiver_remote_control);
    uint8_t new_mac[8] = {0x11,0x22,0x33,0x44,0x55,0x66};
    ps3SetBluetoothMacAddress(new_mac);
    ps3Init();



    for(;;)
    {
        
    }

}


void initialize()
{
    printf("\n----------------------- Incialização dos parametros-----------------------\n");
    
    // Variaveis locais para definição das GPIO dos PWM
    char LEDC_HS_CH0_GPIO, LEDC_HS_CH1_GPIO, LEDC_HS_CH2_GPIO;
    
    LEDC_HS_CH0_GPIO = gpioIn1_l298;
    LEDC_HS_CH1_GPIO = gpioIn2_l298;
    LEDC_HS_CH2_GPIO = gpioPWM_servo;

    // Configurar a LEDC
    ledc_timer_config_t ledc_timer = 
    {
        .duty_resolution = LEDC_TIMER_4_BIT,
        .freq_hz = 50,
        .speed_mode = LEDC_HS_MODE,
        .timer_num = LEDC_HS_TIMER
    };
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel[3] = 
    {
        {
            .channel    = LEDC_HS_CH0_CHANNEL,
            .duty       = 0,
            .gpio_num   = LEDC_HS_CH0_GPIO,
            .speed_mode = LEDC_HS_MODE,
            .hpoint     = 0,
            .timer_sel  = LEDC_HS_TIMER
        },
        {
            .channel    = LEDC_HS_CH1_CHANNEL,
            .duty       = 0,
            .gpio_num   = LEDC_HS_CH1_GPIO,
            .speed_mode = LEDC_HS_MODE,
            .hpoint     = 0,
            .timer_sel  = LEDC_HS_TIMER
        },
        {
            .channel    = LEDC_HS_CH2_CHANNEL,
            .duty       = 0,
            .gpio_num   = LEDC_HS_CH2_GPIO,
            .speed_mode = LEDC_HS_MODE,
            .hpoint     = 0,
            .timer_sel  = LEDC_HS_TIMER
        },
    };

    for (int ch = 0; ch < 3; ch++) 
    {
        ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel[ch]));
    }


    printf("\n----------------------- Incialização dos parametros -----------------------\n");
}

void control_wheel(int In1_l298, int In2_l298, float PWM_l298, float PWM_servo)
{
    printf("\n----------------------- Função do Controle do Carrinho -----------------------\n");

    // Função para controlar a ponte H para o motor ficar no sentido direto
    if((In1_l298 == high) && (In2_l298 == low))
    {
        printf("Acionamento do motor no sentido direto");

        if(driving_mode != 1)
        {
            driving_mode = 1;
            PWM_motor_duty = 0;
        }

        if(driving_mode == 1)
        {
            if(PWM_motor_duty <=14)
            {
                PWM_motor_duty +=1;
            }   
        }
        
        ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_HS_CH0_CHANNEL, PWM_motor_duty);
        ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_HS_CH0_CHANNEL);

        ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_HS_CH1_CHANNEL, 0);
        ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_HS_CH1_CHANNEL);
       
    };  

    // Função para controlar a ponte H para o motor ficar no sentido reverso
    if((In1_l298 == low) && (In2_l298 == high))
    {
        printf("Acionamento do motor no sentido inverso");

        if(driving_mode != 2)
        {
            driving_mode = 2;
            PWM_motor_duty = 0;
        }

        if(driving_mode == 2)
        {
            if(PWM_motor_duty <=14)
            {
                PWM_motor_duty +=1;
            }   
        }
        
        ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_HS_CH1_CHANNEL, PWM_motor_duty);
        ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_HS_CH1_CHANNEL);

        ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_HS_CH0_CHANNEL, 0);
        ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_HS_CH0_CHANNEL);

    }; 

    // Função para controlar a ponte H para o motor freiar
    if(((In1_l298 == low) && (In2_l298 == low)) || ((In1_l298 == high) && (In2_l298 == high)) )
    {
        printf("Acionamento do freio");

        if(driving_mode != 0)
        {
            driving_mode = 0;
            PWM_motor_duty = 0;
        }

        ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_HS_CH0_CHANNEL, 0);
        ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_HS_CH0_CHANNEL);

        ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_HS_CH1_CHANNEL, 0);
        ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_HS_CH1_CHANNEL);
        
    }; 

    printf("\n----------------------- Função do Controle do Carrinho -----------------------\n");
}

void receiver_remote_control(ps3_t ps3, ps3_event_t event)
{
        // Event handling here...
    if ( event.button_down.cross )
        printf("The user started pressing the X button\r\n");


    if ( event.button_up.cross )
        printf("The user released the X button\r\n");


    if ( event.button_down.square )
        printf("The user started pressing the square button\r\n");


    if ( event.button_up.square )
        printf("The user released the square button\r\n");


    if ( event.button_down.triangle )
        printf("The user started pressing the triangle button\r\n");


    if ( event.button_up.triangle )
        printf("The user released the triangle button\r\n");


    if ( event.button_down.circle )
        printf("The user started pressing the circle button\r\n");


    if ( event.button_up.circle )
        printf("The user released the circle button\r\n");


    if ( event.button_down.up )
        printf("The user started pressing the up button\r\n");


    if ( event.button_up.up )
        printf("The user released the up button\r\n");


    if ( event.button_down.down )
        printf("The user started pressing the down button\r\n");


    if ( event.button_up.down )
        printf("The user released the down button\r\n");


    if ( event.button_down.left )
        printf("The user started pressing the left button\r\n");


    if ( event.button_up.left )
        printf("The user released the left button\r\n");


    if ( event.button_down.right )
        printf("The user started pressing the right button\r\n");


    if ( event.button_up.right )
        printf("The user released the right button\r\n");


    if ( event.button_down.l1 )
        printf("The user started pressing the l1 button\r\n");


    if ( event.button_up.l1 )
        printf("The user released the l1 button\r\n");


    if ( event.button_down.l2 )
        printf("The user started pressing the l2 button\r\n");


    if ( event.button_up.l2 )
        printf("The user released the l2 button\r\n");


    if ( event.button_down.r1 )
        printf("The user started pressing the r1 button\r\n");


    if ( event.button_up.r1 )
        printf("The user released the r1 button\r\n");


    if ( event.button_down.r2 )
        printf("The user started pressing the r2 button\r\n");


    if ( event.button_up.r2 )
        printf("The user released the r2 button\r\n");
}

