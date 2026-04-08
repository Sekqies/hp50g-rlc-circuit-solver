#include "constants.h"
#include "types.h"
#include <complex.h>

#define J I

Component component_list[MAXIMUM_COMPONENTS];

double complex mna_matrix[MATRIX_SIZE][MATRIX_SIZE];

double complex z_vector[MATRIX_SIZE];

double complex determine_condutance(const Component* comp){
    const Component component = *comp;
    switch(component.type){
        case RESISTOR:
            return 1/component.value;
            break; 
    }
}

double add_conductances(){ 

}

void zero_initialize(double** mna_matrix){
    for(uint16_t i = 0; i < MATRIX_SIZE; ++i){
        for(uint16_t j = 0; j < MATRIX_SIZE; ++j){
            mna_matrix[i][j] = 0;
        }
    }
}

bool is_voltage_src(const Component* comp){
    return (comp->type == VOLTAGE_SRC || comp->type == VOLTAGE_CONTROLLED_VOLTAGE_SRC || comp->type == CURRENT_CONTROLLED_VOLTAGE_SRC);
}

void build_mna_matrix(double** mna_matrix, double* z_vector, const Component* component_list, const uint8_t num_nodes, const uint8_t num_voltages, const uint8_t num_components){
    const uint8_t size = num_voltages + num_nodes;

    //G matrix

    for(uint8_t i = 0; i < num_voltages; ++i){
        const Component comp = component_list[i];
        const double complex condutance = determine_condutance(&comp);
        mna_matrix[comp.node_neg][comp.node_pos] -= condutance;
        mna_matrix[comp.node_pos][comp.node_neg] -= condutance;
        mna_matrix[comp.node_pos][comp.node_pos] += condutance;
    }
    const uint16_t starting_index = num_nodes;
    
    // B matrix
    
    for(uint8_t i = 0; i < num_components; ++i){
        const Component comp = component_list[i];
        if(!is_voltage_src(&comp)){
            continue;
        }
        mna_matrix[i + starting_index][comp.node_pos] = 1;
        mna_matrix[i + starting_index][comp.node_neg] = -1;
    }

    // C matrix

    for(uint8_t i = 0; i < num_components; ++i){
        const Component comp = component_list[i];
        if(!is_voltage_src(&comp)){
            continue;
        }
        mna_matrix[i][comp.node_pos + starting_index] = 1;
        mna_matrix[i][comp.node_neg + starting_index] = -1;
    }

    // D matrix

    for(uint8_t i = 0; i < num_nodes; ++i){
        for(uint8_t j =0; j < num_voltages; ++j){
            mna_matrix[i+starting_index][j+starting_index] = 0;
        }
    }

    for(uint8_t i = 0; i < num_nodes; ++i){
        
    }

    int voltage_index = 0;
    for(uint8_t i = 0; i < num_components; ++i){
        const Component comp = component_list[i];
        if(!is_voltage_src(&comp)) continue;
        z_vector[starting_index + voltage_index++] = comp.value;  
    }


}


