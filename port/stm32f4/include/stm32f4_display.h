
/**
 * @file stm32f4_display.h
 * @brief Header for stm32f4_display.c file.
 * @author alumno1
 * @author alumno2
 * @date fecha
 */
#ifndef STM32F4_DISPLAY_SYSTEM_H_
#define STM32F4_DISPLAY_SYSTEM_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>

/* HW dependent includes */
#include "stm32f4xx.h"

/* Defines and enums ----------------------------------------------------------*/
/* Defines */
#define STM32F4_REAR_PARKING_DISPLAY_RGB_R_GPIO GPIOB /*< GPIO port for rear parking display RGB R */
#define STM32F4_REAR_PARKING_DISPLAY_RGB_R_PIN 6 /*< GPIO pin for rear parking display RGB R */
#define STM32F4_REAR_PARKING_DISPLAY_RGB_G_GPIO GPIOB /*< GPIO port for rear parking display RGB G */
#define STM32F4_REAR_PARKING_DISPLAY_RGB_G_PIN 8 /*< GPIO pin for rear parking display RGB G */
#define STM32F4_REAR_PARKING_DISPLAY_RGB_B_GPIO GPIOB /*< GPIO port for rear parking display RGB B */
#define STM32F4_REAR_PARKING_DISPLAY_RGB_B_PIN 9 /*< GPIO pin for rear parking display RGB B */

#endif /* STM32F4_DISPLAY_SYSTEM_H_ */
