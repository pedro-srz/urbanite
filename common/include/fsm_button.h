/**
* @file fsm_button.h
* @brief Header for fsm_button.c file.
* @author alumno1
* @author alumno2
* @date fecha
*/

#ifndef FSM_BUTTON_H_
#define FSM_BUTTON_H_
 
/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>
 
/* Other includes */
#include "fsm.h"
 
/* Defines and enums ----------------------------------------------------------*/
/* Enums */
/*! Button FSM states */
enum {  
  	FSM_BUTTON,
    BUTTON_RELEASED = 0,
    BUTTON_RELEASED_WAIT,
    BUTTON_PRESSED,
    BUTTON_PRESSED_WAIT
};
 
/* Typedefs --------------------------------------------------------------------*/
typedef struct fsm_button_t fsm_button_t;
 
/* Function prototypes and explanation -------------------------------------------------*/
/*! Create a new button FSM. */
fsm_button_t *fsm_button_new(uint32_t debounce_time_ms, uint32_t button_id);
/*! Destroy a button FSM. */
void fsm_button_destroy(fsm_button_t *p_fsm); 
/*! Fire the button FSM. */
void fsm_button_fire(fsm_button_t *p_fsm);
/*! Get the inner FSM of the button FSM. */
fsm_t *fsm_button_get_inner_fsm(fsm_button_t *p_fsm);
/*! Get the current state of the button FSM. */
uint32_t fsm_button_get_state(fsm_button_t *p_fsm);
/*! Get the current duration of the button FSM. */
uint32_t fsm_button_get_duration(fsm_button_t *p_fsm);
/*! Reset the current duration of the button FSM. */
void fsm_button_reset_duration(fsm_button_t *p_fsm);
/*! Get the debounce time of the button FSM. */
uint32_t fsm_button_get_debounce_time_ms(fsm_button_t *p_fsm);
/*! Check if the button FSM has been active. */
bool fsm_button_check_activity(fsm_button_t *p_fsm);


#endif