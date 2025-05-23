
/**
 * @file stm32f4_display.c
 * @brief Portable functions to interact with the display system FSM library. All portable functions must be implemented in this file.
 * @author alumno1
 * @author alumno2
 * @date fecha
 */

/* Standard C includes */
#include <stdio.h>
#include <math.h>

/* HW dependent includes */
#include "port_display.h"
#include "port_system.h"

/* Microcontroller dependent includes */
#include "stm32f4_system.h"
#include "stm32f4_display.h"

/* Defines --------------------------------------------------------------------*/
#define MAX_ARR_VALUE 65535

/* Typedefs --------------------------------------------------------------------*/
typedef struct stm32f4_display_hw_t {
    GPIO_TypeDef *p_port_red;
    uint8_t pin_red;
    GPIO_TypeDef *p_port_green;
    uint8_t pin_green;
    GPIO_TypeDef *p_port_blue;
    uint8_t pin_blue;
} stm32f4_display_hw_t;

/* Global variables */
static stm32f4_display_hw_t displays_arr[] = {
    [PORT_REAR_PARKING_DISPLAY_ID] = {
        .p_port_red = STM32F4_REAR_PARKING_DISPLAY_RGB_R_GPIO,
        .pin_red = STM32F4_REAR_PARKING_DISPLAY_RGB_R_PIN,
        .p_port_green = STM32F4_REAR_PARKING_DISPLAY_RGB_G_GPIO,
        .pin_green = STM32F4_REAR_PARKING_DISPLAY_RGB_G_PIN,
        .p_port_blue = STM32F4_REAR_PARKING_DISPLAY_RGB_B_GPIO,
        .pin_blue = STM32F4_REAR_PARKING_DISPLAY_RGB_B_PIN
    }
};

/* Private functions -----------------------------------------------------------*/
stm32f4_display_hw_t *_stm32f4_display_get(uint32_t display_id)
{
    // Return the pointer to the display with the given ID. If the ID is not valid, return NULL.
    if (display_id < sizeof(displays_arr) / sizeof(displays_arr[0]))
    {
        return &displays_arr[display_id];
    }
    else
    {
        return NULL;
    }
}

/* Public functions -----------------------------------------------------------*/
void _timer_pwm_config(uint32_t display_id)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
    TIM4->CR1 &= ~TIM_CR1_CEN;
    TIM4->CR1 |= TIM_CR1_ARPE;
    TIM4->CNT = 0;
    double f_clk = (double)SystemCoreClock;
    double f_timer = (double)(50);
    double psc = (double)((f_clk / ((MAX_ARR_VALUE + 1) * f_timer)) - 1);
    psc = round(psc);
    double arr = (double)((f_clk) / ((psc + 1) * f_timer)) - 1;
    arr = round(arr);
    if (arr > MAX_ARR_VALUE)
    {
        psc = psc + 1;
        psc = round(psc);
        arr = (f_clk / ((psc + 1) * f_timer)) - 1;
        arr = round(arr);
    }
    TIM4->PSC = psc;
    TIM4->ARR = arr;
    TIM4->CCER &= ~TIM_CCER_CC1E;
    TIM4->CCER &= ~TIM_CCER_CC3E;
    TIM4->CCER &= ~TIM_CCER_CC4E;
    TIM4->CCER &= ~TIM_CCER_CC1P;
    TIM4->CCER &= ~TIM_CCER_CC1NP;
    TIM4->CCER &= ~TIM_CCER_CC3P;
    TIM4->CCER &= ~TIM_CCER_CC3NP;
    TIM4->CCER &= ~TIM_CCER_CC4P;
    TIM4->CCER &= ~TIM_CCER_CC4NP;
    TIM4->CCMR1 |= (TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2);
    TIM4->CCMR1 |= TIM_CCMR1_OC1PE;
    TIM4->CCMR2 |= (TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2);
    TIM4->CCMR2 |= TIM_CCMR2_OC3PE;
    TIM4->CCMR2 |= (TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_2);
    TIM4->CCMR2 |= TIM_CCMR2_OC4PE;
    TIM4->EGR |= TIM_EGR_UG;
}

void port_display_set_rgb(uint32_t display_id, rgb_color_t color)
{
    uint8_t red = color.r;
    uint8_t green = color.g;
    uint8_t blue = color.b;
    uint16_t arr = TIM4->ARR;
    if (display_id == PORT_REAR_PARKING_DISPLAY_ID)
    {
        TIM4->CR1 &= ~TIM_CR1_CEN;
        if ((red == 0) && (green == 0) && (blue == 0))
        {
            TIM4->CCER &= ~(TIM_CCER_CC1E);
            TIM4->CCER &= ~(TIM_CCER_CC2E);
            TIM4->CCER &= ~(TIM_CCER_CC3E);
        } else
        {
            if (red == 0)
            {
                TIM4->CCER &= ~(TIM_CCER_CC1E);
            } else
            {
                uint32_t ccr1 = ((red * (arr + 1)) / PORT_DISPLAY_RGB_MAX_VALUE) - 1;
                TIM4->CCR1 = ccr1;
                TIM4->CCER |= TIM_CCER_CC1E;
            }
            if (green == 0)
            {
                TIM4->CCER &= ~(TIM_CCER_CC3E);
            } else
            {
                uint32_t ccr3 = ((red * (arr + 1)) / PORT_DISPLAY_RGB_MAX_VALUE) - 1;
                TIM4->CCR3 = ccr3;
                TIM4->CCER |= TIM_CCER_CC3E;
            }
            if (blue == 0)
            {
                TIM4->CCER &= ~(TIM_CCER_CC4E);
            } else
            {
                uint32_t ccr4 = ((red * (arr + 1)) / PORT_DISPLAY_RGB_MAX_VALUE) - 1;
                TIM4->CCR4 = ccr4;
                TIM4->CCER |= TIM_CCER_CC4E;
            }
            TIM4->EGR |= TIM_EGR_UG;
            TIM4->CR1 |= TIM_CR1_CEN;
        }
    }
}

void port_display_init(uint32_t display_id)
{
    /* Get the display */
    stm32f4_display_hw_t *p_display = _stm32f4_display_get(display_id);
    stm32f4_system_gpio_config(p_display->p_port_red, p_display->pin_red, STM32F4_GPIO_MODE_AF, STM32F4_GPIO_PUPDR_NOPULL);
    stm32f4_system_gpio_config(p_display->p_port_green, p_display->pin_green, STM32F4_GPIO_MODE_AF, STM32F4_GPIO_PUPDR_NOPULL);
    stm32f4_system_gpio_config(p_display->p_port_blue, p_display->pin_blue, STM32F4_GPIO_MODE_AF, STM32F4_GPIO_PUPDR_NOPULL);
    stm32f4_system_gpio_config_alternate(p_display->p_port_red, p_display->pin_red, STM32F4_AF2);
    stm32f4_system_gpio_config_alternate(p_display->p_port_green, p_display->pin_green, STM32F4_AF2);
    stm32f4_system_gpio_config_alternate(p_display->p_port_blue, p_display->pin_blue, STM32F4_AF2);
    _timer_pwm_config(display_id);
    port_display_set_rgb(display_id, COLOR_OFF);
}
