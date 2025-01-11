/**
 * @file fsm_ultrasound.c
 * @brief Ultrasound sensor FSM main file.
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
// Comparison function for qsort
int _compare(const void *a, const void *b)
{
    return (*(uint32_t *)a - *(uint32_t *)b);
}

/* State machine input or transition functions */

/* State machine output or action functions */

/* Public functions -----------------------------------------------------------*/
/* Other auxiliary functions */
/* Para inicializar la FSM */
fsm_ultrasound_t *fsm_ultrasound_new()
{
    fsm_ultrasound_t *p_fsm = malloc(sizeof(fsm_ultrasound_t)); /* Hago malloc para que reserve memoria de todo resto de cosas de la FSM, aunque lo interpreto como fsm_t (el primer elto de la estructura) */
    return p_fsm;
}

fsm_ultrasound_t *fsm_ultrasound_init(uint32_t ultrasound_id)
{
    // Create the FSM
    fsm_ultrasound_t *p_fsm = fsm_ultrasound_new();

    /* TO-DO alumnos: */
    // Init the FSM

    // Initialize the fields of the FSM structure
}