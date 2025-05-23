
/**
 * @file fsm_display.h
 * @brief Header for fsm_display.c file.
 * @author nicolas.roman
 * @author pe.suarez
 * @date 22-05-2025
 */

#ifndef FSM_URBANITE_H_
#define FSM_URBANITE_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>

/* Project includes */
#include "fsm_button.h"
#include "fsm_display.h"
#include "fsm_ultrasound.h"

/* Defines and enums ----------------------------------------------------------*/
/* Enums */
enum FSM_URBANITE {
    OFF = 0,
    MEASURE,
    SLEEP_WHILE_OFF,
    SLEEP_WHILE_ON
};

/* Defines --------------------------------------------------------------------*/


/* Typedefs --------------------------------------------------------------------*/
typedef struct fsm_urbanite_t fsm_urbanite_t;

/* Function prototypes and explanation -------------------------------------------------*/
fsm_urbanite_t *fsm_urbanite_new (fsm_button_t *p_fsm_button, uint32_t on_off_press_time_ms, uint32_t pause_display_time_ms, fsm_ultrasound_t *p_fsm_ultrasound_rear, fsm_display_t *p_fsm_display_rear); /*!< Create a new urbanite FSM. */
void fsm_urbanite_fire (fsm_urbanite_t *p_fsm); /*!< Fire the urbanite FSM. */
void fsm_urbanite_destroy (fsm_urbanite_t *p_fsm); /*!< Destroy the urbanite FSM. */

#endif /* FSM_URBANITE_H_ */