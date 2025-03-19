/**
 * @file interr.c
 * @brief Interrupt service routines for the STM32F4 platform.
 * @author SDG2. Román Cárdenas (r.cardenas@upm.es) and Josué Pagán (j.pagan@upm.es)
 * @date 2025-01-01
 */
// Include HW dependencies:
#include "port_system.h"
#include "stm32f4_system.h"

// Include headers of different port elements:
#include "stm32f4_button.c"

//------------------------------------------------------
// INTERRUPT SERVICE ROUTINES
void EXTI15_10_IRQHandler(void)
{
    /* ISR parking button */
    if (port_button_get_pending_interrupt(PORT_PARKING_BUTTON_ID))
    {
        // Clear the pending interrupt for the button pin
        port_button_clear_pending_interrupt(PORT_PARKING_BUTTON_ID);

        // Get the pressed status for the button pin
        bool pressed = port_button_get_pressed(PORT_PARKING_BUTTON_ID);

        // Set the flag to false
        port_button_set_pressed(PORT_PARKING_BUTTON_ID, pressed);
    }
}
//------------------------------------------------------
/**
 * @brief Interrupt service routine for the System tick timer (SysTick).
 *
 * @note This ISR is called when the SysTick timer generates an interrupt.
 * The program flow jumps to this ISR and increments the tick counter by one millisecond.
 *
 * > **TO-DO alumnos:**
 * >
 * > ✅ 1. **Increment the System tick counter `msTicks` in 1 count.** To do so, use the function `port_system_get_millis()` and `port_system_set_millis()`.
 *
 * @warning **The variable `msTicks` must be declared volatile!** Just because it is modified by a call of an ISR, in order to avoid [*race conditions*](https://en.wikipedia.org/wiki/Race_condition). **Added to the definition** after *static*.
 *
 */
void SysTick_Handler(void)
{
}