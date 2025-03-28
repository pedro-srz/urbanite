/**
 * @file fsm_ultrasound.h
 * @brief Header for fsm_ultrasound.c file.
 * @author alumno1
 * @author alumno2
 * @date fecha
 */

 #ifndef FSM_ULTRASOUND_H_
 #define FSM_ULTRASOUND_H_
 
 /* Includes ------------------------------------------------------------------*/
 /* Standard C includes */
#include <stdint.h>
#include <stdbool.h>
#include "fsm.h"
 
 /* Defines and enums ----------------------------------------------------------*/
#define FSM_ULTRASOUND_NUM_MEASUREMENTS 5 /* Number of measurements to store */
 
 /* Typedefs --------------------------------------------------------------------*/
typedef struct fsm_ultrasound_t fsm_ultrasound_t; /*!< Declaration of the ultrasound FSM struct */

enum FSM_ULTRASOUND
{
    WAIT_START = 0,
    TRIGGER_START,
    WAIT_ECHO_START,
    WAIT_ECHO_END,
    SET_DISTANCE
};

 /* Function prototypes and explanation -------------------------------------------------*/
fsm_ultrasound_t *fsm_ultrasound_new(uint32_t ultrasound_id); /* Constructor */
void fsm_ultrasound_destroy(fsm_ultrasound_t *p_fsm); /* Destructor */
uint32_t fsm_ultrasound_get_distance(fsm_ultrasound_t *p_fsm); /* Getter for the distance */
void fsm_ultrasound_fire(fsm_ultrasound_t *p_fsm); /* Fire function */
bool fsm_ultrasound_get_status(fsm_ultrasound_t *p_fsm); /* Getter for the status */
void fsm_ultrasound_set_status(fsm_ultrasound_t *p_fsm, bool status); /* Setter for the status */
bool fsm_ultrasound_get_ready(fsm_ultrasound_t *p_fsm); /* Getter for the ready flag */
bool fsm_ultrasound_get_new_measurement_ready(fsm_ultrasound_t *p_fsm); /* Getter for the new measurement ready flag */
void fsm_ultrasound_stop(fsm_ultrasound_t *p_fsm); /* Stop function */
void fsm_ultrasound_start(fsm_ultrasound_t *p_fsm); /* Start function */
fsm_t *fsm_ultrasound_get_inner_fsm(fsm_ultrasound_t *p_fsm); /* Getter for the inner FSM */
uint32_t fsm_ultrasound_get_state(fsm_ultrasound_t *p_fsm); /* Getter for the state */
void fsm_ultrasound_set_state(fsm_ultrasound_t *p_fsm, int8_t state); /* Setter for the state */
bool fsm_ultrasound_check_activity(fsm_ultrasound_t *p_fsm); /* Check if the FSM is active */
 
 /**
  * @brief Set the state of the ultrasound FSM.
  *
  * This function sets the current state of the ultrasound FSM.
  *
  * > &nbsp;&nbsp;&nbsp;&nbsp;💡 This function is important because the struct is private and external functions such as those of the unit tests cannot access the state of the FSM directly. \n
  * 
  * @param p_fsm Pointer to an `fsm_ultrasound_t` struct.
  * @param state New state of the ultrasound FSM.
  */
 void fsm_ultrasound_set_state(fsm_ultrasound_t *p_fsm, int8_t state);
 
 #endif /* FSM_ULTRASOUND_H_ */
 