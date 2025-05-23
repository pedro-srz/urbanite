/**
 * @file port_display.h
 * @brief Header for the portable functions to interact with the HW of the display system. The functions must be implemented in the platform-specific code.
 * @author alumno1
 * @author alumno2
 * @date fecha
 */
#ifndef PORT_DISPLAY_SYSTEM_H_
#define PORT_DISPLAY_SYSTEM_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>

/* Typedefs --------------------------------------------------------------------*/
typedef struct rgb_color_t
{
    uint8_t r; /*!< Color red */
    uint8_t g; /*!< Color green */
    uint8_t b; /*!< Color blue */
} rgb_color_t; /*!< Struct that represents the LED colors. */

/* Defines and enums ----------------------------------------------------------*/
/* Defines */
#define PORT_REAR_PARKING_DISPLAY_ID 0 /*!< ID of the rear parking display. */
#define PORT_DISPLAY_RGB_MAX_VALUE 255 /*!< Maximum value of the RGB color. */
#define COLOR_RED (rgb_color_t){255, 0, 0} /*!< Color red. */
#define COLOR_GREEN (rgb_color_t){0, 255, 0} /*!< Color green. */
#define COLOR_BLUE (rgb_color_t){0, 0, 255} /*!< Color blue. */
#define COLOR_YELLOW (rgb_color_t){94, 94, 0} /*!< Color yellow. */
#define COLOR_TURQUOISE (rgb_color_t){25, 89, 82} /*!< Color turquoise. */
#define COLOR_OFF (rgb_color_t){0, 0, 0} /*!< Color off. */

/* Function prototypes and explanation -------------------------------------------------*/
void port_display_init(uint32_t display_id); /*!< Initialize the display system. */
void port_display_set_rgb(uint32_t display_id, rgb_color_t color); /*!< Set the RGB color of the display. */

#endif /* PORT_DISPLAY_SYSTEM_H_ */