/**
 * @file port_system.h
 * @brief Header for port_system.c file.
 * @author SDG2. Román Cárdenas (r.cardenas@upm.es) and Josué Pagán (j.pagan@upm.es)
 * @date 2025-01-01
 */

#ifndef PORT_SYSTEM_H_
#define PORT_SYSTEM_H_

/* Includes del sistema */
#include <stdint.h>

/**
 * @brief Initializes the system.
 */
uint32_t port_system_init(void);

/**
 * @brief Returns the number of milliseconds since the system started.
 *
 * @retval number of milliseconds since the system started.
 */
uint32_t port_system_get_millis(void);

/**
 * @brief Sets the number of milliseconds since the system started.
 *
 * @param ms New number of milliseconds since the system started.
 */
void port_system_set_millis(uint32_t ms);

/**
 * @brief Delays the program execution for the specified number of milliseconds.
 *
 * @param ms Number of milliseconds to delay.
 */
void port_system_delay_ms(uint32_t ms);

/**
 * @brief Delays the program execution until the specified number of milliseconds since the system started.
 *
 * @param t Pointer to the variable that stores the number of milliseconds to delay until.
 * @param ms Number of milliseconds to delay until.
 *
 * @note This function modifies the value of the variable pointed by t to the number of milliseconds to delay until.
 * @note This function is useful to implement periodic tasks.
 */
void port_system_delay_until_ms(uint32_t *t, uint32_t ms);

/**
 * @brief Puts the system to sleep.
 */
void port_system_sleep(void);

/**
 * @brief Suspends the SysTick timer.
 */
void port_system_systick_suspend(void);

/**
 * @brief Resumes the SysTick timer.
 */
void port_system_systick_resume(void);

/**
 * @brief Puts the system in low power mode.
 */
void port_system_power_sleep(void);

#endif /* PORT_SYSTEM_H_ */

/**
 * @brief Puts the system in stop mode.
 */
void port_system_power_stop(void);

