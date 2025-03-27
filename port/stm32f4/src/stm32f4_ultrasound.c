/**
 * @file stm32f4_ultrasound.c
 * @brief Portable functions to interact with the ultrasound FSM library. All portable functions must be implemented in this file.
 * @author Nicolás Román Prieto
 * @author Pedro Suárez Delgado
 * @date 27/03/2025
 */

/* Standard C includes */
#include <stdio.h>
#include <math.h>
#include "port_ultrasound.h"
#include "port_system.h"
#include "stm32f4_system.h"
#include "stm32f4_ultrasound.h"

/* HW dependent includes */

/* Microcontroller dependent includes */

/* Macros ---------------------------------------------------------------------*/
#define MAX_ARR_VALUE 65535
#define PRIORITY_LEVEL_4 4

/* Typedefs --------------------------------------------------------------------*/
typedef struct 
{
    GPIO_TypeDef *p_trigger_port;
    GPIO_TypeDef *p_echo_port;
    uint8_t trigger_pin;
    uint8_t echo_pin;
    uint8_t echo_alt_fun;
    bool trigger_ready;
    bool trigger_end;
    bool echo_recieved;
    uint32_t echo_init_tick;
    uint32_t echo_end_tick;
    uint32_t echo_overflows;
} stm32f4_ultrasound_hw_t;

/* Global variables */
static stm32f4_ultrasound_hw_t ultrasound_arr[] = {
    [PORT_REAR_PARKING_SENSOR_ID] = {.p_trigger_port = STM32F4_REAR_PARKING_SENSOR_TRIGGER_GPIO, .p_echo_port = STM32F4_REAR_PARKING_SENSOR_ECHO_GPIO, .trigger_pin = STM32F4_REAR_PARKING_SENSOR_TRIGGER_PIN, .echo_pin = STM32F4_REAR_PARKING_SENSOR_ECHO_PIN, .echo_alt_fun = 0, .trigger_ready = false, .trigger_end = false, .echo_recieved = false, .echo_init_tick = 0, .echo_end_tick = 0, .echo_overflows = 0},
};

/* Private functions ----------------------------------------------------------*/
/**
 * @brief Get the ultrasound sensor struct with the given ID.
 *
 * @param ultrasound_id Ultrasound sensor ID.
 *
 * @return Pointer to the ultrasound sensor struct.
 * @return NULL If the ultrasound sensor ID is not valid.
 */
static stm32f4_ultrasound_hw_t *_stm32f4_ultrasound_get(uint32_t ultrasound_id)
{
    if (ultrasound_id < sizeof(ultrasound_arr) / sizeof(ultrasound_arr[0]))
    {
        return &ultrasound_arr[ultrasound_id];
    }
    else
    {
        return NULL;
    }
}

static void _timer_trigger_setup()
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    TIM3->CR1 = 0;
    TIM3->CR1 |= TIM_CR1_ARPE;
    TIM3->CNT = 0;
    double f_clk = (double)SystemCoreClock;
    double f_timer = 1/PORT_PARKING_SENSOR_TRIGGER_UP_US;
    double psc = (f_clk / (f_timer * MAX_ARR_VALUE));
    psc = round(psc);
    double arr = (f_clk / (psc * f_timer));
    arr = round(arr);
    if (arr > MAX_ARR_VALUE)
    {
        psc = psc + 1;
        psc = round(psc);
        arr = (f_clk / (psc * f_timer));
        arr = round(arr);
    }
    TIM3->PSC = psc;
    TIM3->ARR = arr;
    TIM3->SR &= ~TIM_SR_UIF;
    TIM3->DIER |= TIM_DIER_UIE;
    NVIC_SetPriority(TIM3_IRQn, PRIORITY_LEVEL_4);
    TIM3->EGR |= TIM_EGR_UG;
}

/* Public functions -----------------------------------------------------------*/
void port_ultrasound_init(uint32_t ultrasound_id)
{
    /* Get the ultrasound sensor */
    stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);

    /* TO-DO alumnos: */

    /* Trigger pin configuration */

    /* Echo pin configuration */

    /* Configure timers */
}

// Getters and setters functions


// Util
void stm32f4_ultrasound_set_new_trigger_gpio(uint32_t ultrasound_id, GPIO_TypeDef *p_port, uint8_t pin)
{
    stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
    p_ultrasound->p_trigger_port = p_port;
    p_ultrasound->trigger_pin = pin;
}

void stm32f4_ultrasound_set_new_echo_gpio(uint32_t ultrasound_id, GPIO_TypeDef *p_port, uint8_t pin)
{
    stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
    p_ultrasound->p_echo_port = p_port;
    p_ultrasound->echo_pin = pin;
}