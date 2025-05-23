
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
#include "port_display.h"
#include "port_system.h"

/* Project includes */
#include "fsm.h"
#include "fsm_display.h"

/* Typedefs --------------------------------------------------------------------*/
typedef struct fsm_display_t
{
    fsm_t f; /*!< FSM structure */
    int32_t distance_cm; /*!< Distance in cm */
    bool new_color; /*!< Flag to indicate if a new color is set */
    bool status; /*!< FSM status */
    bool idle; /*!< FSM idle status */
    uint32_t display_id; /*!< Display ID */

} fsm_display_t;

/* Private functions -----------------------------------------------------------*/
void _compute_display_levels(rgb_color_t *p_color, int32_t distance_cm)
{
    if (distance_cm >= DANGER_MIN_CM && distance_cm <= WARNING_MIN_CM)
    {
        *p_color = COLOR_RED;
    } else if (distance_cm > WARNING_MIN_CM && distance_cm <= NO_PROBLEM_MIN_CM)
    {
        *p_color = COLOR_YELLOW;
    } else if (distance_cm > NO_PROBLEM_MIN_CM && distance_cm <= INFO_MIN_CM)
    {
        *p_color = COLOR_GREEN;
    } else if (distance_cm > INFO_MIN_CM && distance_cm <= OK_MIN_CM)
    {
        *p_color = COLOR_TURQUOISE;
    } else if (distance_cm > OK_MIN_CM && distance_cm <= OK_MAX_CM)
    {
        *p_color = COLOR_BLUE;
    } else {
        *p_color = COLOR_OFF;
    }
}


static bool check_set_new_color(fsm_t *p_this)
{
    fsm_display_t *p_fsm = (fsm_display_t *)(p_this);
    return p_fsm->new_color;
}

/**
 * @brief Check if the display FSM is active.
 * 
 * @param p_this Pointer to the FSM.
 * @return true if the FSM is active, false otherwise.
 */
static bool check_active(fsm_t *p_this)
{
    fsm_display_t *p_fsm = (fsm_display_t *)(p_this);
    return p_fsm->status;
}

static bool check_off(fsm_t *p_this)
{
    fsm_display_t *p_fsm = (fsm_display_t *)(p_this);
    return !p_fsm->status;
}

/**
 * @brief Set the display FSM to the ON state.
 * 
 * @param p_this Pointer to the FSM.
 */
static void do_set_on(fsm_t *p_this)
{
    fsm_display_t *p_fsm = (fsm_display_t *)(p_this);
    port_display_set_rgb(p_fsm->display_id, COLOR_OFF);
}

/**
 * @brief Set the display FSM to the OFF state.
 * 
 * @param p_this Pointer to the FSM.
 */
static void do_set_off(fsm_t *p_this)
{
    fsm_display_t *p_fsm = (fsm_display_t *)(p_this);
    port_display_set_rgb(p_fsm->display_id, COLOR_OFF);
    p_fsm->status = false;
    p_fsm->idle = true;
}

/**
 * @brief Set the display FSM to the SET_DISPLAY state.
 * 
 * @param p_this Pointer to the FSM.
 */
static void do_set_color(fsm_t *p_this)
{
    fsm_display_t *p_fsm = (fsm_display_t *)(p_this);
    rgb_color_t color = {0, 0, 0};
    _compute_display_levels(&color, p_fsm->distance_cm);
    port_display_set_rgb(p_fsm->display_id, color);
    p_fsm->new_color = false;
    p_fsm->idle = true;
}

/**
 * @brief Display FSM transition table.
 * 
 * 
 */static fsm_trans_t fsm_trans_display[] = {
    {WAIT_DISPLAY, check_active, SET_DISPLAY, do_set_on},
    {SET_DISPLAY, check_set_new_color, SET_DISPLAY, do_set_color},
    {SET_DISPLAY, check_off, WAIT_DISPLAY, do_set_off},
    {-1, NULL, -1, NULL},
};

/* State machine input or transition functions */

/* State machine output or action functions */

/* Other auxiliary functions */
/**
 * @brief Initialize the display FSM.
 * 
 * @param p_fsm_display Pointer to the display FSM.
 * @param display_id ID of the display.
 */
static void fsm_display_init(fsm_display_t *p_fsm_display, uint32_t display_id)
{
    fsm_init(&p_fsm_display->f, fsm_trans_display);
    p_fsm_display->display_id = display_id;
    p_fsm_display->distance_cm = -1;
    p_fsm_display->new_color = false;
    p_fsm_display->status = false;
    p_fsm_display->idle = false;
    port_display_init(display_id);
}


/* Public functions -----------------------------------------------------------*/
fsm_display_t *fsm_display_new(uint32_t display_id)
{
    fsm_display_t *p_fsm_display = malloc(sizeof(fsm_display_t)); /* Do malloc to reserve memory of all other FSM elements, although it is interpreted as fsm_t (the first element of the structure) */
    fsm_display_init(p_fsm_display, display_id); /* Initialize the FSM */
    return p_fsm_display;
}

/**
 * @brief Check if the display FSM has been active.
 * 
 * @param p_fsm Pointer to the display FSM.
 * @return true if the FSM is active, false otherwise.
 */
bool fsm_display_check_activity(fsm_display_t *p_fsm)
{
    if (p_fsm->status && p_fsm->idle == false)
    {
        return true;
    }
    return false;
}

/**
 * @brief Destroy the display FSM.
 * 
 * @param p_fsm Pointer to the display FSM.
 */
void fsm_display_destroy(fsm_display_t *p_fsm)
{
    free(&p_fsm->f);
}

/**
 * @brief Set the distance for the display FSM.
 * 
 * @param p_fsm Pointer to the display FSM.
 * @param distance_cm Distance in cm.
 */
void fsm_display_set_distance(fsm_display_t *p_fsm, uint32_t distance_cm)
{
    p_fsm->distance_cm = distance_cm;
    p_fsm->new_color = true;
}

/**
 * @brief Fire the display FSM.
 * 
 * @param p_fsm Pointer to the display FSM.
 */
void fsm_display_fire(fsm_display_t *p_fsm)
{
    fsm_fire(&p_fsm->f);
}

/**
 * @brief Get the status of the display FSM.
 * 
 * @param p_fsm Pointer to the display FSM.
 * @return true if the FSM is active, false otherwise.
 */
bool fsm_display_get_status(fsm_display_t *p_fsm)
{
    return p_fsm->status;
}

/**
 * @brief Set the status of the display FSM.
 * 
 * @param p_fsm Pointer to the display FSM.
 * @param pause true to pause the FSM, false to resume it.
 */
void fsm_display_set_status(fsm_display_t *p_fsm, bool pause)
{
    p_fsm->status = pause;
}

/**
 * @brief Get the inner FSM of the display FSM.
 * 
 * @param p_fsm Pointer to the display FSM.
 * @return Pointer to the inner FSM.
 */
fsm_t *fsm_display_get_inner_fsm(fsm_display_t *p_fsm)
{
    return &p_fsm->f;
}

/**
 * @brief Get the current state of the display FSM.
 * 
 * @param p_fsm Pointer to the display FSM.
 * @return Current state of the FSM.
 */
uint32_t fsm_display_get_state(fsm_display_t *p_fsm)
{
    return p_fsm->f.current_state;
}

/**
 * @brief Set the current state of the display FSM.
 * 
 * @param p_fsm Pointer to the display FSM.
 * @param state New state of the FSM.
 */
void fsm_display_set_state(fsm_display_t *p_fsm, uint32_t state)
{
    p_fsm->f.current_state = state;
}
