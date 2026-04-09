#pragma once

#include "constants.h"
#include <stdint.h>
#include <stdbool.h>
#include <complex.h>

typedef enum{
    RESISTOR,
    INDUCTOR,
    CAPACITOR,
    VOLTAGE_SRC,
    CURRENT_SRC,
    CURRENT_CONTROLLED_VOLTAGE_SRC,
    VOLTAGE_CONTROLLED_VOLTAGE_SRC,
    CURRENT_CONTROLLED_CURRENT_SRC,
    VOLTAGE_CONTROLLED_CURRENT_SRC,
    SWITCH
} ComponentType;

enum ValueType{
    RESISTANCE,
    VOLTAGE,
    CURRENT
};

typedef struct {
    /**
     * @brief The numerical magnitude associated of the component.
     * * The physical meaning of this value depends on the type of the component
     * - RESISTOR: Resistance in Ohms 
     * - CAPACITOR: Capacitance in Faradays (F)
     * - INDUCTOR: Inductance in Henrys (H)
     * - VOLTAGE_SRC: Voltage in Volts (V)
     * - CURRENT_SRC: Current in Amperes (A)
     */
    double complex value;
    uint8_t type;

    uint8_t node_pos;
    uint8_t node_neg;

    uint8_t control_node_pos;
    uint8_t control_node_neg;
    uint8_t control_id;

    bool is_target;
} Component;

Component add_independent_component(ComponentType type, double complex value, uint8_t node_neg, uint8_t node_pos, bool is_target);

Component add_component(ComponentType type, double complex value, uint8_t node_neg, uint8_t node_pos, uint8_t control_node_pos, uint8_t control_node_neg, uint8_t control_id, bool is_target);

