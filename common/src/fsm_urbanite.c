
/**
 * @file fsm_display.c
 * @brief Display system FSM main file.
 * @author nicolas.roman
 * @author pe.suarez
 * @date 22-05-2025
 */

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdlib.h>
#include <stdio.h>

/* HW dependent includes */
#include "port_system.h"

/* Project includes */
#include "fsm.h"
#include "fsm_urbanite.h"

/* Typedefs --------------------------------------------------------------------*/
typedef struct fsm_urbanite_t {
    fsm_t f; /*!< FSM structure */
    fsm_button_t *p_fsm_button; /*!< Pointer to the button FSM */
    uint32_t on_off_press_time_ms; /*!< Time in ms to press the button to turn on/off */
    uint32_t pause_display_time_ms; /*!< Time in ms to pause the display */
    bool is_paused; /*!< Flag to indicate if the display is paused */
    fsm_ultrasound_t *p_fsm_ultrasound_rear; /*!< Pointer to the rear ultrasound FSM */
    fsm_display_t *p_fsm_display_rear; /*!< Pointer to the rear display FSM */
} fsm_urbanite_t;

/* Private functions -----------------------------------------------------------*/
/**
 * @brief Create a new urbanite FSM.
 * 
 * @param p_fsm_button Pointer to the button FSM.
 * @param on_off_press_time_ms Time in ms to press the button to turn on/off.
 */
static bool check_on(fsm_t *p_this)
{
    fsm_urbanite_t *p_fsm = (fsm_urbanite_t *)(p_this);
    if ((fsm_button_get_duration(p_fsm->p_fsm_button) > 0) && (fsm_button_get_duration(p_fsm->p_fsm_button) > p_fsm->on_off_press_time_ms))
    {
        return true;
    } else
    {
        return false;
    }
}

/**
 * @brief Check if the display is off.
 * 
 * @param p_this Pointer to the FSM.
 * @return true if the display is off, false otherwise.
 */
static bool check_off(fsm_t *p_this)
{
    return check_on(p_this);
}

/**
 * @brief Check if the ultrasound sensor has a new measurement ready.
 * 
 * @param p_this Pointer to the FSM.
 * @return true if the ultrasound sensor has a new measurement ready, false otherwise.
 */
static bool check_new_measure(fsm_t *p_this)
{
    fsm_urbanite_t *p_fsm = (fsm_urbanite_t *)(p_this);
    return fsm_ultrasound_get_new_measurement_ready(p_fsm->p_fsm_ultrasound_rear);
}

/**
 * @brief Check if any activity is detected.
 * 
 * @param p_this Pointer to the FSM.
 * @return true if any activity is detected, false otherwise.
 */
static bool check_activity(fsm_t *p_this)
{
    fsm_urbanite_t *p_fsm = (fsm_urbanite_t *)(p_this);
    bool check_button = fsm_button_check_activity(p_fsm->p_fsm_button);
    bool check_display = fsm_display_check_activity(p_fsm->p_fsm_display_rear);
    bool check_ultrasound = fsm_ultrasound_check_activity(p_fsm->p_fsm_ultrasound_rear);
    
    if (check_ultrasound || check_display || check_button)
    {
        return true;
    } else
    {
        return false;
    }
}

/**
 * @brief Check if there is no activity detected.
 * 
 * @param p_this Pointer to the FSM.
 * @return true if there is no activity detected, false otherwise.
 */
static bool check_no_activity(fsm_t *p_this)
{
    return !check_activity(p_this);
}

/**
 * @brief Check if the display is paused.
 * 
 * @param p_this Pointer to the FSM.
 * @return true if the display is paused, false otherwise.
 */
static bool check_pause_display(fsm_t * p_this)
{
    fsm_urbanite_t *p_fsm = (fsm_urbanite_t *)(p_this);
    if ((fsm_button_get_duration(p_fsm->p_fsm_button) > 0) && (fsm_button_get_duration(p_fsm->p_fsm_button) > p_fsm->pause_display_time_ms) && (fsm_button_get_duration(p_fsm->p_fsm_button) < p_fsm->on_off_press_time_ms))
    {
        return true;
    } else
    {
        return false;
    }
}

/**
 * @brief Check if the display is active in measure.
 * 
 * @param p_this Pointer to the FSM.
 * @return true if the display is active in measure, false otherwise.
 */
static bool check_activity_in_measure(fsm_t *p_this)
{
    return check_new_measure(p_this);
}

/**
 * @brief Display the distance measured by the ultrasound sensor.
 * 
 * @param p_this Pointer to the FSM.
 * 
 */
static void do_display_distance(fsm_t * p_this)
{
    fsm_urbanite_t *p_fsm = (fsm_urbanite_t *)(p_this);
    uint32_t distance_cm = fsm_ultrasound_get_distance(p_fsm->p_fsm_ultrasound_rear);
    if (p_fsm->is_paused)
    {
        if (distance_cm < (WARNING_MIN_CM / 2))
        {
            fsm_display_set_distance(p_fsm->p_fsm_display_rear, distance_cm);
            fsm_display_set_status(p_fsm->p_fsm_display_rear, true);
        } else
        {
            fsm_display_set_status(p_fsm->p_fsm_display_rear, false);
        }
    } else
    {
        fsm_display_set_distance(p_fsm->p_fsm_display_rear, distance_cm);
    }
    printf("[URBANITE][%ld] Distance: %ld cm\n", port_system_get_millis(), distance_cm);
}

/**
 * @brief Start a new measurement.
 * 
 * @param p_this Pointer to the FSM.
 */
static void do_start_up_measure	(fsm_t *p_this)
{
    fsm_urbanite_t *p_fsm = (fsm_urbanite_t *)(p_this);
    fsm_button_reset_duration(p_fsm->p_fsm_button);
    fsm_ultrasound_start(p_fsm->p_fsm_ultrasound_rear);
    fsm_display_set_status(p_fsm->p_fsm_display_rear, true);
    printf("[URBANITE][%ld] Urbanite system ON\n", port_system_get_millis());
}

/**
 * @brief Pause or resume the display.
 * 
 * @param p_this Pointer to the FSM.
 */
static void do_pause_display(fsm_t *p_this)
{
    fsm_urbanite_t *p_fsm = (fsm_urbanite_t *)(p_this);
    fsm_button_reset_duration(p_fsm->p_fsm_button);
    if (p_fsm->is_paused)
    {
        p_fsm->is_paused = false;
    } else
    {
        p_fsm->is_paused = true;
    }

    if (p_fsm->is_paused)
    {
        fsm_display_set_status(p_fsm->p_fsm_display_rear, false);
        printf("[URBANITE][%ld] Urbanite system display PAUSE\n", port_system_get_millis());

    } else
    {
        fsm_display_set_status(p_fsm->p_fsm_display_rear, true);
        printf("[URBANITE][%ld] Urbanite system display RESUME\n", port_system_get_millis());
    }
}

/**
 * @brief Stop the urbanite system.
 * 
 * @param p_this Pointer to the FSM.
 */
static void do_stop_urbanite(fsm_t *p_this)
{
    fsm_urbanite_t *p_fsm = (fsm_urbanite_t *)(p_this);
    fsm_button_reset_duration(p_fsm->p_fsm_button);
    fsm_ultrasound_stop(p_fsm->p_fsm_ultrasound_rear);
    fsm_display_set_status(p_fsm->p_fsm_display_rear, p_fsm->is_paused);
    if (p_fsm->is_paused)
    {
        p_fsm->is_paused = false;
    }
    printf("[URBANITE][%ld] Urbanite system OFF\n", port_system_get_millis());
}

/**
 * @brief Sleep the urbanite system.
 * 
 * @param p_this Pointer to the FSM.
 */
static void do_sleep_while_off(fsm_t *p_this)
{
    port_system_sleep();
}

/**
 * @brief Sleep the urbanite system.
 * 
 * @param p_this Pointer to the FSM.
 */
static void do_sleep_while_on(fsm_t *p_this)
{
    port_system_sleep();
}

/**
 * @brief Sleep the urbanite system.
 * 
 * @param p_this Pointer to the FSM.
 */
static void do_sleep_while_measure(fsm_t *p_this)
{
    port_system_sleep();
}

/**
 * @brief Sleep the urbanite system.
 * 
 * @param p_this Pointer to the FSM.
 */
static void do_sleep_off(fsm_t *p_this)
{
    port_system_sleep();
}

/* Other auxiliary functions */
fsm_trans_t fsm_trans_urbanite[] = {
    {OFF, check_on, MEASURE, do_start_up_measure},
    {OFF, check_no_activity, SLEEP_WHILE_OFF, do_sleep_off},
    {MEASURE, check_off, OFF, do_stop_urbanite},
    {MEASURE, check_pause_display, MEASURE, do_pause_display},
    {MEASURE, check_new_measure, MEASURE, do_display_distance},
    {MEASURE, check_no_activity, SLEEP_WHILE_ON, do_sleep_while_measure},
    {SLEEP_WHILE_ON, check_activity_in_measure, MEASURE, NULL},
    {SLEEP_WHILE_ON, check_no_activity, SLEEP_WHILE_ON, do_sleep_while_on},
    {SLEEP_WHILE_OFF, check_activity, OFF, NULL},
    {SLEEP_WHILE_OFF, check_no_activity, SLEEP_WHILE_OFF, do_sleep_while_off},
    {-1, NULL, -1, NULL}
};

static void fsm_urbanite_init (fsm_urbanite_t *p_fsm_urbanite, fsm_button_t *p_fsm_button, uint32_t on_off_press_time_ms, uint32_t pause_display_time_ms, fsm_ultrasound_t *p_fsm_ultrasound_rear, fsm_display_t *p_fsm_display_rear)
{
    fsm_init(&p_fsm_urbanite->f, fsm_trans_urbanite);
    p_fsm_urbanite->p_fsm_button = p_fsm_button;
    p_fsm_urbanite->on_off_press_time_ms = on_off_press_time_ms;
    p_fsm_urbanite->pause_display_time_ms = pause_display_time_ms;
    p_fsm_urbanite->is_paused = false;
    p_fsm_urbanite->p_fsm_ultrasound_rear = p_fsm_ultrasound_rear;
    p_fsm_urbanite->p_fsm_display_rear = p_fsm_display_rear;
}



/* Public functions -----------------------------------------------------------*/

/**
 * @brief Create a new urbanite FSM.
 * 
 * @param p_fsm_button Pointer to the button FSM.
 * @param on_off_press_time_ms Time in ms to press the button to turn on/off.
 * @param pause_display_time_ms Time in ms to pause the display.
 * @param p_fsm_ultrasound_rear Pointer to the rear ultrasound FSM.
 * @param p_fsm_display_rear Pointer to the rear display FSM.
 * 
 * @return Pointer to the urbanite FSM.
 */
fsm_urbanite_t* fsm_urbanite_new (fsm_button_t * p_fsm_button, uint32_t on_off_press_time_ms, uint32_t 	pause_display_time_ms, fsm_ultrasound_t * 	p_fsm_ultrasound_rear, fsm_display_t * 	p_fsm_display_rear )
{
    fsm_urbanite_t *p_fsm_urbanite = malloc(sizeof(fsm_urbanite_t)); /* Do malloc to reserve memory of all other FSM elements, although it is interpreted as fsm_t (the first element of the structure) */
    fsm_urbanite_init(p_fsm_urbanite, p_fsm_button, on_off_press_time_ms, pause_display_time_ms, p_fsm_ultrasound_rear, p_fsm_display_rear);   /* Initialize the FSM */
    return p_fsm_urbanite;                                       /* Composite pattern: return the fsm_t pointer as a fsm_urbanite_t pointer */
}	

/**
 * @brief Fire the urbanite FSM.
 * 
 * @param p_fsm Pointer to the urbanite FSM.
 */
void fsm_urbanite_fire	(fsm_urbanite_t * p_fsm)	
{
    fsm_fire(&p_fsm->f);
}

/**
 * @brief Destroy the urbanite FSM.
 * 
 * @param p_fsm Pointer to the urbanite FSM.
 */
void fsm_urbanite_destroy(fsm_urbanite_t * p_fsm)	
{
    free(p_fsm);
}