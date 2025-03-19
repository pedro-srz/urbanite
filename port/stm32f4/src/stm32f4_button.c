/**
 * @file stm32f4_button.c
 * @brief Portable functions to interact with the button FSM library. All portable functions must be implemented in this file.
 * @author Nicolás Román Prieto
 * @author Pedro Suárez Delgado
 * @date 17-03-2025
 */


/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stddef.h>

/* HW dependent includes */
#include "port_button.h" // Used to get general information about the buttons (ID, etc.)
#include "port_system.h" // Used to get the system tick

/* Microcontroller dependent includes */
#include "stm32f4_button.h" // Used to specific auxiliary functions based on our microcontroller button (STM32)
#include "stm32f4_system.h" // Used to interact with the clock and the rest of the system

/* Macros */
#define BUTTON_INPUT_MODE 0
#define BUTTON_NOPU_NOPD 0
#define PRIORITY_LEVEL_1 1
#define SUBPRIORITY_LEVEL_0 0

/* Typedefs --------------------------------------------------------------------*/
typedef struct
{
    GPIO_TypeDef *p_port;
    uint8_t pin;
    uint8_t pupd_mode;
    bool flag_pressed;
} stm32f4_button_hw_t;

/* Global variables ------------------------------------------------------------*/

/**
 * @brief Array of elements that represents the HW characteristics of the buttons connected to the STM32F4 platform.
 * 
 * 
 * This must be hidden from the user, so it is declared as a static. To access the elements of this array, use the function
    `_stm32f4_button-get()`.
 */
static stm32f4_button_hw_t buttons_arr[] = {
    [PORT_PARKING_BUTTON_ID] = {.p_port = STM32F4_PARKING_BUTTON_GPIO, .pin = STM32F4_PARKING_BUTTON_PIN, .pupd_mode = STM32F4_GPIO_PUPDR_NOPULL},
};

/* Private functions ----------------------------------------------------------*/
/**
 * @brief Get the button status struct with the given ID.
 *
 * @param button_id Button ID.
 *
 * @return Pointer to the button state struct.
 * @return NULL If the button ID is not valid.
 */

stm32f4_button_hw_t *_stm32f4_button_get(uint32_t button_id)
{
    // Return the pointer to the button with the given ID. If the ID is not valid, return NULL.
    if (button_id < sizeof(buttons_arr) / sizeof(buttons_arr[0]))
    {
        return &buttons_arr[button_id];
    }
    else
    {
        return NULL;
    }
}

/* Public functions -----------------------------------------------------------*/
void port_button_init(uint32_t button_id)
{
    // Retrieve the button struct using the private function and the button ID
    stm32f4_button_hw_t *p_button = _stm32f4_button_get(button_id);
    stm32f4_system_gpio_config(p_button->p_port, p_button->pin, BUTTON_INPUT_MODE, BUTTON_NOPU_NOPD);
    stm32f4_system_gpio_config_exti(p_button->p_port, p_button->pin, STM32F4_TRIGGER_ENABLE_INTERR_REQ);
    stm32f4_system_gpio_exti_enable(p_button->pin, PRIORITY_LEVEL_1, SUBPRIORITY_LEVEL_0);
}

void port_button_clear_pending_interrupt(uint32_t button_id)
{
    stm32f4_button_hw_t *p_button = _stm32f4_button_get(button_id);
    if (p_button != NULL)
    {
        // Clear the pending interrupt for the button pin
        EXTI->PR = BIT_POS_TO_MASK(p_button->pin);
    }
}

void port_button_disable_interrupt(uint32_t button_id)
{
    stm32f4_button_hw_t *p_button = _stm32f4_button_get(button_id);
    if (p_button != NULL)
    {
        // Disable the interrupt for the button pin
        stm32f4_system_gpio_exti_disable(p_button->pin);
    }
}

bool port_button_get_pending_interrupt(uint32_t button_id)
{
    stm32f4_button_hw_t *p_button = _stm32f4_button_get(button_id);
    if (p_button != NULL)
    {
        // Return the pending interrupt for the button pin
        return (EXTI->PR & BIT_POS_TO_MASK(p_button->pin)) != 0;
    }
    else
    {
        return false;
    }
}

bool port_button_get_pressed(uint32_t button_id)
{
    stm32f4_button_hw_t *p_button = _stm32f4_button_get(button_id);
    if (p_button != NULL)
    {
        // Return the pressed status for the button pin
        return p_button->flag_pressed;
    }
    else
    {
        return false;
    }
}

bool port_button_get_value(uint32_t button_id)
{
    stm32f4_button_hw_t *p_button = _stm32f4_button_get(button_id);
    if (p_button != NULL)
    {
        // Return the value of the button pin
        return stm32f4_system_gpio_read(p_button->p_port, p_button->pin);
    }
    else
    {
        return false;
    }
}

void port_button_set_pressed(uint32_t button_id, bool pressed)
{
    stm32f4_button_hw_t *p_button = _stm32f4_button_get(button_id);
    if (p_button != NULL)
    {
        // Set the pressed status for the button pin
        p_button->flag_pressed = pressed;
    }
}

void stm32f4_button_set_new_gpio(uint32_t button_id, GPIO_TypeDef *p_port, uint8_t pin)
{
    stm32f4_button_hw_t *p_button = _stm32f4_button_get(button_id);
    p_button->p_port = p_port;
    p_button->pin = pin;
}