/**
 * @file port_ultrasound.h
 * @brief Header for the portable functions to interact with the HW of the ultrasound sensors. The functions must be implemented in the platform-specific code.
 * @author Nicolás Román Prieto
 * @author Pedro Suárez Delgado
 * @date 27/03/2025
 */
#ifndef PORT_ULTRASOUND_H_
#define PORT_ULTRASOUND_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>

/* Defines and enums ----------------------------------------------------------*/
#define PORT_REAR_PARKING_SENSOR_ID 0 /*!< User ultrasound sensor identifier that represents the rear sensor (activation of the parking aid system).*/
#define PORT_PARKING_SENSOR_TRIGGER_UP_US 10 /*!< Trigger time in microseconds for the ultrasound sensor.*/
#define PORT_PARKING_SENSOR_TIMEOUT_MS 100 /*!< Timeout time in milliseconds for the ultrasound sensor.*/
#define SPEED_OF_SOUND_MS 343 /*!< Speed of sound in meters per second.*/

/* Function prototypes and explanation -------------------------------------------------*/
void port_ultrasound_init(uint32_t ultrasound_id); /*!< Configure the HW specifications of a given ultrasound sensor.*/
void port_ultrasound_start_measurement(uint32_t ultrasound_id); /*!< Start the measurement of the ultrasound sensor.*/
void port_ultrasound_stop_trigger_timer(uint32_t ultrasound_id); /*!< Stop the trigger timer of the ultrasound sensor.*/
void port_ultrasound_stop_echo_timer(uint32_t ultrasound_id); /*!< Stop the echo timer of the ultrasound sensor.*/
void port_ultrasound_start_new_measurement_timer(void); /*!< Start the timer for a new measurement of the ultrasound sensor.*/
void port_ultrasound_stop_new_measurement_timer(void); /*!< Stop the timer for a new measurement of the ultrasound sensor.*/
void port_ultrasound_reset_echo_ticks(uint32_t ultrasound_id); /*!< Reset the echo ticks of the ultrasound sensor.*/
void port_ultrasound_stop_ultrasound(uint32_t ultrasound_id); /*!< Stop the ultrasound sensor.*/
bool port_ultrasound_get_trigger_ready(uint32_t ultrasound_id); /*!< Get the status of the trigger of the ultrasound sensor.*/
void port_ultrasound_set_trigger_ready(uint32_t ultrasound_id, bool trigger_ready); /*!< Set the status of the trigger of the ultrasound sensor.*/
bool port_ultrasound_get_trigger_end(uint32_t ultrasound_id); /*!< Get the status of the trigger end of the ultrasound sensor.*/
void port_utrasound_set_trigger_end(uint32_t ultrasound_id, bool trigger_end); /*!< Set the status of the trigger end of the ultrasound sensor.*/
uint32_t port_ultrasound_get_echo_init_tick(uint32_t ultrasound_id); /*!< Get the initial tick of the echo of the ultrasound sensor.*/
void port_ultrasound_set_echo_init_tick(uint32_t ultrasound_id, uint32_t echo_init_tick); /*!< Set the initial tick of the echo of the ultrasound sensor.*/
uint32_t port_ultrasound_get_echo_end_tick(uint32_t ultrasound_id); /*!< Get the final tick of the echo of the ultrasound sensor.*/
void port_ultrasound_set_echo_end_tick(uint32_t ultrasound_id, uint32_t echo_end_tick); /*!< Set the final tick of the echo of the ultrasound sensor.*/
bool port_ultrasound_get_echo_received(uint32_t ultrasound_id); /*!< Get the status of the echo of the ultrasound sensor.*/
void port_ultrasound_set_echo_received(uint32_t ultrasound_id, bool echo_recieved); /*!< Set the status of the echo of the ultrasound sensor.*/
uint32_t port_ultrasound_get_echo_overflows(uint32_t ultrasound_id); /*!< Get the number of overflows of the echo of the ultrasound sensor.*/
void port_ultrasound_set_echo_overflows(uint32_t ultrasound_id, uint32_t echo_overflows); /*!< Set the number of overflows of the echo of the ultrasound sensor.*/


#endif /* PORT_ULTRASOUND_H_ */
