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

/* Other auxiliary functions */
void fsm_ultrasound_init(fsm_t *p_this, uint32_t ultrasound_id)
{
    // Initialize the FSM
    fsm_init(p_this, fsm_trans_ultrasound);

    // Retrieve the ultrasound
    fsm_ultrasound_t *p_fsm = (fsm_ultrasound_t *)(p_this);

    /* TO-DO alumnos: */
    // Initialize the fields of the FSM structure
}

/* Public functions -----------------------------------------------------------*/
fsm_ultrasound_t *fsm_ultrasound_new(uint32_t ultrasound_id)
{
    fsm_t *p_fsm = malloc(sizeof(fsm_ultrasound_t)); /* Do malloc to reserve memory of all other FSM elements, although it is interpreted as fsm_t (the first element of the structure) */
    fsm_ultrasound_init(p_fsm, ultrasound_id);                  /* Initialize the FSM */
    return (fsm_ultrasound_t *)(p_fsm);                         /* Composite pattern: return the fsm_t pointer as a fsm_ultrasound_t pointer */
}