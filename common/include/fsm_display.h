
/**
 * @file fsm_display.h
 * @brief Header for fsm_display.c file.
 * @author nicolas.roman
 * @author pe.suarez
 * @date 22-05-2025
 */

#ifndef FSM_DISPLAY_SYSTEM_H_
#define FSM_DISPLAY_SYSTEM_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>
#include "fsm.h"

/* Defines and enums ----------------------------------------------------------*/
/* Enums */
enum FSM_DISPLAY_SYSTEM {
    WAIT_DISPLAY = 0,
    SET_DISPLAY
};

/* Defines --------------------------------------------------------------------*/
#define DANGER_MIN_CM 0 /* Minimum danger distance in cm */
#define WARNING_MIN_CM 25 /* Minimum warning distance in cm */
#define NO_PROBLEM_MIN_CM 50 /* Minimum no problem distance in cm */
#define INFO_MIN_CM 150 /* Minimum info distance in cm */
#define OK_MIN_CM 175 /* Minimum ok distance in cm */
#define OK_MAX_CM 200 /* Maximum ok distance in cm */

/* Typedefs --------------------------------------------------------------------*/
typedef struct fsm_display_t fsm_display_t;

/* Function prototypes and explanation -------------------------------------------------*/
fsm_display_t *fsm_display_new(uint32_t display_id); /*!< Create a new display FSM. */
void fsm_display_destroy(fsm_display_t *p_fsm); /*!< Destroy a display FSM. */
void fsm_display_set_distance(fsm_display_t *p_fsm, uint32_t distance_cm); /*!< Set the distance for the display FSM. */
void fsm_display_fire(fsm_display_t *p_fsm); /*!< Fire the display FSM. */
bool fsm_display_get_status(fsm_display_t *p_fsm); /*!< Get the status of the display FSM. */
void fsm_display_set_status(fsm_display_t *p_fsm, bool pause); /*!< Set the status of the display FSM. */
bool fsm_display_check_activity(fsm_display_t *p_fsm); /*!< Check if the display FSM has been active. */
fsm_t *fsm_display_get_inner_fsm(fsm_display_t *p_fsm); /*!< Get the inner FSM of the display FSM. */
uint32_t fsm_display_get_state(fsm_display_t *p_fsm); /*!< Get the current state of the display FSM. */
void fsm_display_set_state(fsm_display_t *p_fsm, uint32_t state); /*!< Set the current state of the display FSM. */

#endif /* FSM_DISPLAY_SYSTEM_H_ */
