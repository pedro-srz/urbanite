/**
 * @file fsm_display.c
 * @brief Display system FSM main file.
 * @author alumno1
 * @author alumno2
 * @date fecha
 */

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */

/* HW dependent includes */

/* Project includes */

/* Typedefs --------------------------------------------------------------------*/

/* Private functions -----------------------------------------------------------*/

/* State machine input or transition functions */

/* State machine output or action functions */

/* Other auxiliary functions */
fsm_display_t *fsm_display_new()
{
    fsm_display_t *p_fsm = malloc(sizeof(fsm_display_t)); /* Hago malloc para que reserve memoria de todo resto de cosas de la FSM, aunque lo interpreto como fsm_t (el primer elto de la estructura) */
    return p_fsm;
}