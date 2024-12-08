#include <stdio.h>
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>
#include "esp_log.h"

#define boton 2
#define led 5

typedef enum //Se hacen los estados de la maquina de estado
{
    Es_apagado,
    Es_100ms,
    Es_500ms,
    Es_1000ms
}Es_led;

Es_led Es_actual=Es_apagado;        //Se inicializan los 3 estados
Es_led Es_pasado=Es_apagado;
Es_led Es_futuro=Es_100ms;

//Variables
uint8_t pre_boton =0;
uint8_t led_estado =0;
uint8_t contador =0;

//Funciones
void crear_hardware(void);
void llamar_temp_led(TimerHandle_t xTimer);
void llamar_temp_boton(TimerHandle_t xTimer);
void cambio_estado(void);

//Temporizadores
TimerHandle_t temp_led;
TimerHandle_t temp_boton;

 
void app_main(void)
{
    crear_hardware();

    temp_led=xTimerCreate("TEMPO_LED", pdMS_TO_TICKS(100), pdTRUE, NULL, llamar_temp_led );
    if (temp_led == NULL)
    {
        printf("Error al crear el temporizador del LED.\n");
        return;
    }

     temp_boton=xTimerCreate("TEMPO_BOTON", pdMS_TO_TICKS(10), pdTRUE, NULL, callback_temp_boton );
    if (temp_boton == NULL)
    {
        printf("Error al crear el temporizador del boton.\n");
        return;
    }

    if (xTimerStart(temp_led, 0) != pdPASS)
    {
        printf("Error al inicializar el temporizador del LED.\n");
        return;
    }
    
    if (xTimerStart(temp_led, 0) != pdPASS)
    {
        printf("Error al inicializar el temporizador del boton.\n");
        return;
    }
    
}

void crear_hardware(void)
{
    gpio_reset_pin(led);
    gpio_set_direction(led, GPIO_MODE_DEF_OUTPUT);

    gpio_reset_pin(boton);
    gpio_set_direction(boton, GPIO_MODE_INPUT);
    gpio_set_pull_mode(boton, GPIO_PULLUP_ONLY);
}

void llamar_temp_led(TimerHandle_t xTimer)
{
    switch (Es_actual)
    {
    case Es_apagado:
        gpio_set_level(led, 0);
        break;
    
    case Es_100ms:
        contador++;
        if (contador >= 1)
        {
            led_estado = !led_estado;
            gpio_set_level(led, led_estado);
            contador=0;
        }
        
        break;
    
    case Es_500ms:
        contador++;
        if (contador >= 5)
        {
            led_estado = !led_estado;
            gpio_set_level(led, led_estado);
            contador=0;
        }
        break;    
    
    case Es_1000ms:
        contador++;
        if (contador >= 10)
        {
            led_estado = !led_estado;
            gpio_set_level(led, led_estado);
            contador=0;
        }
        break;

    default:
        break;
    }
}

void llamar_temp_boton(TimerHandle_t xTimer)
{
    if (gpio_get_level(boton) == 0)
    {
        if (!pre_boton)
        {
            pre_boton = 1;
            cambio_estado();
        }
        
    } else 
        {
            pre_boton = 0;
        }
}

void cambio_estado(void)
{
    switch (Es_actual)
    {
    case Es_apagado:
        Es_actual=Es_100ms;
        Es_pasado=Es_apagado;
        Es_futuro=Es_500ms;
        printf("Estado actual:Es_100ms Estado futuro=Es_500ms\n");
        break;
    
    case Es_100ms:
        Es_actual=Es_500ms;
        Es_pasado=Es_100ms;
        Es_futuro=Es_1000ms;
        printf("Estado actual:Es_500ms Estado futuro=Es_1000ms\n");
        break;

    case Es_500ms:
        Es_actual=Es_1000ms;
        Es_pasado=Es_500ms;
        Es_futuro=Es_apagado;
        printf("Estado actual:Es_1000ms Estado futuro=Es_apagado\n");
        break;

    case Es_1000ms:
        Es_actual=Es_apagado;
        Es_pasado=Es_1000ms;
        Es_futuro=Es_100ms;
        printf("Estado actual:Es_apagado Estado futuro=Es_100ms\n");
        break;

    default:
        break;
    }
}
