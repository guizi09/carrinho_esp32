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
#include "ps3.h"

//inicialização do retorno do evento do controle
ps3SetEventCallback(controller_event_cb);
ps3Init();


// Definição de algumas variaveis relevantes
#define true 1
#define false 0
#define high 1
#define low 0

// Definição do número das portas
#define gpioIn1_l298 14
#define gpioIn2_l298 15

#define gpioPWM_servo 16


// Definição das caracteristicaas do PWM
#define LEDC_TIMER_BIT_NUM 10
#define LEDC_TIMER_FREQ_HZ 1000000
#define LEDC_CHANNEL_0 0
#define LEDC_CHANNEL_1 1
#define LEDC_CLK_Source_1 RC_FAST_CLK // clock de 8MHz que tem maior precisao
#define LEDC_hpoint 15

#define LEDC_SPEED_MODE LEDC_HIGH_SPEED_MODE  // 

// Função para inicializar o esp
void initialize();

// Função utilizada para definir o nível logico da sáida do esp para controlar a PONT H L298N
void control_wheel(int In1_l298, int In2_l298, float PWM_l298, float PWM_servo);

// Função utilizada para receber via bluetooth os comandos do controle
void receiver_remote_control();

void app_main(void)
{

    // Configurar o nível de log para DEBUG
    esp_log_level_set("INFO", ESP_LOG_DEBUG);
    ESP_LOGI("INFO","Inicializando a aplicação");

    initialize();
    

    char PWM_servo_duty= 0;     //duty cycle do servo (TEM QUE SER ENTRE 5% E 10%)
    char PWM_motor_duty= 0;     //duty cycle do motor
    char driving_mode= 0;       // 0=parado, 1= pra frente, 2= pra tras
    char brake= false;          // ta freiando?

    for(;;)
    {
        receiver_remote_control();
        

        ledc_set_duty_and_update(/INSERIR SPEEDMODE/,LEDC_CHANNEL,/INSERIR VAR COM DUTY CYCLE/,LEDC_hpoint);
    }

}


void initialize()
{
    printf("\n----------------------- Incialização dos parametros-----------------------\n");

    // Comfiguração das portas gpio
    gpio_config_t io_config_gpio;

    io_config_gpio.pin_bit_mask = (1ULL << gpioIn1_l298);
    io_config_gpio.mode = GPIO_MODE_OUTPUT;
    io_config_gpio.pull_up_en = GPIO_PULLUP_DISABLE;
    io_config_gpio.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_config_gpio.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&io_config_gpio);

    io_config_gpio.pin_bit_mask = (1ULL << gpioIn2_l298);
    io_config_gpio.mode = GPIO_MODE_OUTPUT;
    io_config_gpio.pull_up_en = GPIO_PULLUP_DISABLE;
    io_config_gpio.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_config_gpio.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&io_config_gpio);

    //Definição do valor lógico inicial das portas
    gpio_set_level(gpioIn1_l298, low);
    gpio_set_level(gpioIn2_l298, low);

    //Configuração das portas de PWM
    
    // ************     Configuração do LEDC CANAL 0
    ledc_timer_config_t ledc_timer = 
    {
        .speed_mode = LEDC_SPEED_MODE, 
        .duty_resolution = 4,               //numero de bits que representam duty cycle, pra nosso projeto ta suficiente 16 valores de duty cycle
        .bit_num = LEDC_TIMER_BIT_NUM,
        .timer_num = 0,
        .freq_hz = LEDC_TIMER_FREQ_HZ,
        .clk_cfg = LEDC_CLK_Source_1,
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    ledc_channel_config_t ledc_channel_0 = 
    {
        .gpio_num = gpioPWM_l298_1, // Número do pino GPIO do motor
        .speed_mode = LEDC_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = 0,
        .duty = 0,
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));

    // ************     Configuração do LEDC CANAL 1

    ledc_timer_config_t ledc_timer = 
    {
        .speed_mode = LEDC_SPEED_MODE, 
        .duty_resolution = 4,               //numero de bits que representam duty cycle, pra nosso projeto ta suficiente 16 valores de duty cycle
        .bit_num = LEDC_TIMER_BIT_NUM,
        .timer_num = 0,
        .freq_hz = LEDC_TIMER_FREQ_HZ,
        .clk_cfg = LEDC_CLK_Source_1,
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    ledc_channel_config_t ledc_channel_1 = 
    {
        .gpio_num = gpioPWM_l298_2, // Número do pino GPIO do motor
        .speed_mode = LEDC_SPEED_MODE,
        .channel = LEDC_CHANNEL_1,
        .timer_sel = 0,
        .duty = 0,
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));

    ledc_channel_config_t ledc_channel = 
    {
        .channel = LEDC_CHANNEL,
        .duty = 0,
        .gpio_num = gpioPWM_servo, // Número do pino GPIO do servo
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .timer_sel = LEDC_TIMER_0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));

    printf("\n----------------------- Incialização dos parametros -----------------------\n");
}

void control_wheel(int In1_l298, int In2_l298, float PWM_l298, float PWM_servo)
{
    printf("\n----------------------- Função do Controle do Carrinho -----------------------\n");

    // Função para controlar a ponte H para o motor ficar no sentido direto
    if((In1_l298 == high) && (In2_l298 == low))
    {
        printf("Acionamento do motor no sentido direto");
        gpio_set_level(gpioIn1_l298, high);
        gpio_set_level(gpioIn2_l298, low);
    };  

    // Função para controlar a ponte H para o motor ficar no sentido reverso
    if((In1_l298 == low) && (In2_l298 == high))
    {
        printf("Acionamento do motor no sentido inverso");
        gpio_set_level(gpioIn1_l298, low);
        gpio_set_level(gpioIn2_l298, high);
    }; 

    // Função para controlar a ponte H para o motor freiar
    if(((In1_l298 == low) && (In2_l298 == low)) || ((In1_l298 == high) && (In2_l298 == high)) )
    {
        printf("Acionamento do freio");
        gpio_set_level(gpioIn1_l298, low);
        gpio_set_level(gpioIn2_l298, low);
    }; 

    printf("\n----------------------- Função do Controle do Carrinho -----------------------\n");
}

void receiver_remote_control()
{
    while (!ps3IsConnected())
    {
        // Prevent the Task Watchdog from triggering
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }

    controller_event_cb()


}

void controller_event_cb( ps3_t ps3, ps3_event_t event )
{
    if ( ps3.status.battery >= ps3_status_battery_high )
    print("The controller still has sufficient battery charge");

    if ( ps3.status.charging )
    print("Controller is charging");

    if ( ps3.button.triangle )
    print("Currently pressing the trangle button");

    if ( ps3.analog.stick.lx < 0 )
    print("Currently pulling analog stick to the left");

    if ( event.button_down.cross )
    print("The user started pressing the X button");

    if ( event.button_up.cross )
    print("The user released the X button");

    if ( event.analog_changed.stick.lx )
    print("The user has moved the left stick sideways");
}