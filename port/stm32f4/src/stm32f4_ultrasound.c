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
#define PRIORITY_LEVEL_3 3
#define PRIORITY_LEVEL_4 4
#define PRIORITY_LEVEL_5 5
#define HIGH 1U /*!< High value */
#define LOW 0U  /*!< Low value */


/* Typedefs --------------------------------------------------------------------*/
typedef struct 
{
    GPIO_TypeDef *p_trigger_port; /*!< GPIO port of the trigger pin of the ultrasound sensor.*/
    GPIO_TypeDef *p_echo_port; /*!< GPIO port of the echo pin of the ultrasound sensor.*/
    uint8_t trigger_pin; /*!< GPIO pin of the trigger pin of the ultrasound sensor.*/
    uint8_t echo_pin; /*!< GPIO pin of the echo pin of the ultrasound sensor.*/
    uint8_t echo_alt_fun; /*!< Alternate function of the echo pin of the ultrasound sensor.*/
    bool trigger_ready; /*!< Flag to indicate if the trigger is ready to start a new measurement.*/
    bool trigger_end; /*!< Flag to indicate if the trigger has ended.*/
    bool echo_recieved; /*!< Flag to indicate if the echo has been recieved.*/
    uint32_t echo_init_tick; /*!< Initial tick of the echo signal.*/
    uint32_t echo_end_tick; /*!< Final tick of the echo signal.*/
    uint32_t echo_overflows; /*!< Number of overflows of the echo signal.*/
} stm32f4_ultrasound_hw_t; /*!< Struct that represents the HW characteristics of the ultrasound sensor.*/

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
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    TIM3->CR1 &= ~TIM_CR1_CEN;
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
    TIM3->EGR |= TIM_EGR_UG;
    TIM3->SR &= ~TIM_SR_UIF;
    TIM3->DIER |= TIM_DIER_UIE;
    NVIC_SetPriority(TIM3_IRQn, PRIORITY_LEVEL_4);

}

void _timer_new_measurement_setup()
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
    TIM5->CR1 &= ~TIM_CR1_CEN;
    TIM5->CR1 |= TIM_CR1_ARPE;
    TIM5->CNT = 0;
    double f_clk = (double)SystemCoreClock;
    double f_timer = 1/PORT_PARKING_SENSOR_TIMEOUT_MS;
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
    TIM5->PSC = psc;
    TIM5->ARR = arr;
    TIM5->SR &= ~TIM_SR_UIF;
    TIM5->DIER |= TIM_DIER_UIE;
    NVIC_SetPriority(TIM5_IRQn, PRIORITY_LEVEL_5);
    TIM5->EGR |= TIM_EGR_UG;
}

void _timer_echo_setup()
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    TIM2->CR1 &= ~TIM_CR1_CEN;
    TIM2->PSC = 16;
    TIM2->ARR = MAX_ARR_VALUE;
    TIM2->CR1 |= TIM_CR1_ARPE;
    TIM2->CCMR1 |= TIM_CCMR1_CC2S_0;
    TIM2->CCMR1 &= ~TIM_CCMR1_IC2F;
    TIM2->CCER |= (TIM_CCER_CC2P | TIM_CCER_CC2NP);
    TIM2->CCMR1 &= ~TIM_CCMR1_IC2F;
    TIM2->CCER |= TIM_CCER_CC2E;
    TIM2->DIER |= TIM_DIER_CC2IE;
    TIM2->DIER |= TIM_DIER_UIE;
    NVIC_SetPriority(TIM2_IRQn, PRIORITY_LEVEL_3);
    TIM2->EGR |= TIM_EGR_UG;

}

/* Public functions -----------------------------------------------------------*/
void port_ultrasound_init(uint32_t ultrasound_id)
{
    /* Get the ultrasound sensor */
    stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
    p_ultrasound->p_trigger_port = STM32F4_REAR_PARKING_SENSOR_TRIGGER_GPIO;
    p_ultrasound->p_echo_port = STM32F4_REAR_PARKING_SENSOR_ECHO_GPIO;
    p_ultrasound->trigger_pin = STM32F4_REAR_PARKING_SENSOR_TRIGGER_PIN;
    p_ultrasound->echo_pin = STM32F4_REAR_PARKING_SENSOR_ECHO_PIN;
    p_ultrasound->echo_alt_fun = 0;
    p_ultrasound->trigger_ready = true;
    p_ultrasound->trigger_end = false;
    p_ultrasound->echo_recieved = false;
    p_ultrasound->echo_init_tick = 0;
    p_ultrasound->echo_end_tick = 0;
    p_ultrasound->echo_overflows = 0;

    /* Trigger pin configuration */
    stm32f4_system_gpio_config(p_ultrasound->p_trigger_port, p_ultrasound->trigger_pin, STM32F4_GPIO_MODE_OUT, STM32F4_GPIO_PUPDR_NOPULL);

    /* Echo pin configuration */
    stm32f4_system_gpio_config(p_ultrasound->p_echo_port, p_ultrasound->echo_pin, STM32F4_GPIO_MODE_AF, STM32F4_GPIO_PUPDR_NOPULL);
    stm32f4_system_gpio_config_alternate(p_ultrasound->p_echo_port, p_ultrasound->echo_pin, p_ultrasound->echo_alt_fun);
    

    /* Configure timers */
    _timer_trigger_setup();
    _timer_new_measurement_setup();
    _timer_echo_setup();
}

void port_ultrasound_start_measurement(uint32_t ultrasound_id)
{
    stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
    p_ultrasound->trigger_ready = true;
    TIM2->CNT = 0;
    TIM3->CNT = 0;
    TIM5->CNT = 0;
    if (p_ultrasound->trigger_ready)
    {
        _timer_trigger_setup();
        _timer_echo_setup();
    }
    stm32f4_system_gpio_write(p_ultrasound->p_trigger_port, p_ultrasound->trigger_pin, HIGH);
    NVIC_EnableIRQ(TIM2_IRQn);
    NVIC_EnableIRQ(TIM3_IRQn);
    TIM2->CR1 |= TIM_CR1_CEN;
    TIM3->CR1 |= TIM_CR1_CEN;
}

void port_ultrasound_stop_trigger_timer(uint32_t ultrasound_id)
{
    stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
    stm32f4_system_gpio_write(p_ultrasound->p_trigger_port, p_ultrasound->trigger_pin, LOW);
    TIM3->CR1 &= ~TIM_CR1_CEN;
}

void port_ultrasound_stop_echo_timer(uint32_t ultrasound_id)
{
    stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
    TIM2->CR1 &= ~TIM_CR1_CEN;
    if (p_ultrasound->echo_recieved)
    {
        _timer_echo_setup();
    }
}

void port_ultrasound_start_new_measurement_timer(void)
{
    NVIC_EnableIRQ(TIM5_IRQn);
    TIM5->CR1 |= TIM_CR1_CEN;
}

void port_ultrasound_stop_new_measurement_timer(void)
{
    TIM5->CR1 &= ~TIM_CR1_CEN;
}

void port_ultrasound_reset_echo_ticks(uint32_t ultrasound_id)
{
    stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
    p_ultrasound->echo_init_tick = 0;
    p_ultrasound->echo_end_tick = 0;
    p_ultrasound->echo_overflows = 0;
    p_ultrasound->echo_recieved = false;
}

void port_ultrasound_stop_ultrasound(uint32_t ultrasound_id)
{
    port_ultrasound_stop_trigger_timer(ultrasound_id);
    port_ultrasound_stop_echo_timer(ultrasound_id);
    port_ultrasound_stop_new_measurement_timer();
    port_ultrasound_reset_echo_ticks(ultrasound_id);
}

// Getters and setters functions
bool port_ultrasound_get_trigger_ready(uint32_t ultrasound_id)
{
    stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
    return p_ultrasound->trigger_ready;
}

void port_ultrasound_set_trigger_ready(uint32_t ultrasound_id, bool trigger_ready)
{
    stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
    p_ultrasound->trigger_ready = trigger_ready;
}

bool port_ultrasound_get_trigger_end(uint32_t ultrasound_id)
{
    stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
    return p_ultrasound->trigger_end;
}

void port_utrasound_set_trigger_end(uint32_t ultrasound_id, bool trigger_end)
{
    stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
    p_ultrasound->trigger_end = trigger_end;
}

uint32_t port_ultrasound_get_echo_init_tick(uint32_t ultrasound_id)
{
    stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
    return p_ultrasound->echo_init_tick;
}

void port_ultrasound_set_echo_init_tick(uint32_t ultrasound_id, uint32_t echo_init_tick)
{
    stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
    p_ultrasound->echo_init_tick = echo_init_tick;
}

uint32_t port_ultrasound_get_echo_end_tick(uint32_t ultrasound_id)
{
    stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
    return p_ultrasound->echo_end_tick;
}

void port_ultrasound_set_echo_end_tick(uint32_t ultrasound_id, uint32_t echo_end_tick)
{
    stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
    p_ultrasound->echo_end_tick = echo_end_tick;
}

bool port_ultrasound_get_echo_received(uint32_t ultrasound_id)
{
    stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
    return p_ultrasound->echo_recieved;
}

void port_ultrasound_set_echo_received(uint32_t ultrasound_id, bool echo_recieved)
{
    stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
    p_ultrasound->echo_recieved = echo_recieved;
}

uint32_t port_ultrasound_get_echo_overflows(uint32_t ultrasound_id)
{
    stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
    return p_ultrasound->echo_overflows;
}

void port_ultrasound_set_echo_overflows(uint32_t ultrasound_id, uint32_t echo_overflows)
{
    stm32f4_ultrasound_hw_t *p_ultrasound = _stm32f4_ultrasound_get(ultrasound_id);
    p_ultrasound->echo_overflows = echo_overflows;
}

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