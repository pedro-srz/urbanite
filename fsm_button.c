/**
 * @file fsm_button.c
 * @brief Button FSM main file.
 * @author alumno1
 * @author alumno2
 * @date fecha
 */

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */

/* HW dependent includes */
#include "port_button.h"
#include "port_system.h"

/* Project includes */
#include "fsm_button.h"

/* State machine input or transition functions */

/* State machine output or action functions */

/* Other auxiliary functions */

fsm_button_t *fsm_button_new()
{
    fsm_t *p_fsm = malloc(sizeof(fsm_button_t)); /* Do malloc to reserve memory of all other FSM elements, although it is interpreted as fsm_t (the first element of the structure) */
    return p_fsm;                                /* Composite pattern: return the fsm_t pointer as a fsm_button_t pointer */
}

fsm_button_t *fsm_button_init(uint32_t debounce_time_ms, uint32_t button_id)
{
    // Create the FSM
    fsm_button_t *p_fsm = fsm_button_new();
    fsm_init(&p_fsm->f, fsm_trans_button);

    /* TO-DO alumnos: */
}

/* FSM-interface functions. These functions are used to interact with the FSM */
void fsm_button_fire(fsm_button_t *p_fsm)
{
    return &p_fsm->f;
}

fsm_t *fsm_button_get_inner_fsm(fsm_button_t *p_fsm)
{
    return &p_fsm->f;
}

uint32_t fsm_button_get_state(fsm_button_t *p_fsm)
{
    return p_fsm->f.current_state;
}