/**
 * @file interr.c
 * @brief Interrupt service routines for the STM32F4 platform.
 * @author SDG2. Román Cárdenas (r.cardenas@upm.es) and Josué Pagán (j.pagan@upm.es)
 * @date 2025-01-01
 */
// Include HW dependencies:
#include "port_system.h"
#include "stm32f4_system.h"
#include "stm32f4_ultrasound.h"

// Include headers of different port elements:
#include "stm32f4_button.c"
#include "stm32f4_ultrasound.c"

//------------------------------------------------------
// INTERRUPT SERVICE ROUTINES

/**
 * @brief Interrupt service routine for the EXTI0.
 * 
 * @note This ISR is called when the EXTI0 generates an interrupt.
 * The program flow jumps to this ISR and calls the callback function for the EXTI0.
 */
void EXTI15_10_IRQHandler(void)
{
    /* ISR parking button */
    if (port_button_get_value(PORT_PARKING_BUTTON_ID))
    {
        // Set the flag to false
        port_button_set_pressed(PORT_PARKING_BUTTON_ID, !port_button_get_value(PORT_PARKING_BUTTON_ID));
    } else {
        // Set the flag to true
        port_button_set_pressed(PORT_PARKING_BUTTON_ID, !port_button_get_value(PORT_PARKING_BUTTON_ID));
    }
    // Clear the pending interrupt for the button pin
    port_button_clear_pending_interrupt(PORT_PARKING_BUTTON_ID);
}

/**
 * @brief Interrupt service routine for the Timer 2.
 * 
 * @note This ISR is called when the Timer 2 generates an interrupt.
 * The program flow jumps to this ISR and calls the callback function for the Timer 2.
 */
void TIM2_IRQHandler(void)
{
    if (TIM2->SR & TIM_SR_UIF)
    {
        TIM2->SR &= ~TIM_SR_UIF;
        port_ultrasound_set_echo_overflows(PORT_REAR_PARKING_SENSOR_ID, port_ultrasound_get_echo_overflows(PORT_REAR_PARKING_SENSOR_ID) + 1);
    }
    if (TIM2->SR & TIM_SR_CC2IF)
    {
        if (port_ultrasound_get_echo_init_tick(PORT_REAR_PARKING_SENSOR_ID) == port_ultrasound_get_echo_end_tick(PORT_REAR_PARKING_SENSOR_ID))
        {
            port_ultrasound_set_echo_init_tick(PORT_REAR_PARKING_SENSOR_ID, TIM2->CCR2);
        }
        else
        {
            port_ultrasound_set_echo_end_tick(PORT_REAR_PARKING_SENSOR_ID, TIM2->CCR2);
            port_ultrasound_set_echo_received(PORT_REAR_PARKING_SENSOR_ID, true);
        }

    }
}

/**
 * @brief Interrupt service routine for the Timer 3.
 * 
 * @note This ISR is called when the Timer 3 generates an interrupt.
 * The program flow jumps to this ISR and calls the callback function for the Timer 3.
 */
void TIM3_IRQHandler(void)
{
    if (TIM3->SR & TIM_SR_UIF)
    {
        TIM3->SR &= ~TIM_SR_UIF;
        port_utrasound_set_trigger_end(PORT_REAR_PARKING_SENSOR_ID, true);
    }
}

/**
 * @brief Interrupt service routine for the Timer 5.
 * 
 * @note This ISR is called when the Timer 5 generates an interrupt.
 * The program flow jumps to this ISR and calls the callback function for the Timer 5.
 */
void TIM5_IRQHandler(void)
{
    if (TIM5->SR & TIM_SR_UIF)
    {
        TIM5->SR &= ~TIM_SR_UIF;
        port_ultrasound_set_trigger_ready(PORT_REAR_PARKING_SENSOR_ID, true);
    }
}


//------------------------------------------------------
/**
 * @brief Interrupt service routine for the System tick timer (SysTick).
 *
 * @note This ISR is called when the SysTick timer generates an interrupt.
 * The program flow jumps to this ISR and increments the tick counter by one millisecond.
 *
 * @warning **The variable `msTicks` must be declared volatile!** Just because it is modified by a call of an ISR, in order to avoid [*race conditions*](https://en.wikipedia.org/wiki/Race_condition). **Added to the definition** after *static*.
 *
 */
void SysTick_Handler(void)
{
    // Increment the System tick counter in 1 count
    port_system_set_millis(port_system_get_millis() + 1);
}