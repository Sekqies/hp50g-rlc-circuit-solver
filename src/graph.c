#include "constants.h"
#include "types.h"
#include "matrix_operations.h"
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

bool is_current_src(const Component* comp){
    return comp->type == CURRENT_SRC;
}


/**
 * @brief Builds a Modified Nodal Analysis (MNA) matrix and it's corresponding z vector in the format A * x = z
 * @param mna_matrix The empty (zero-initialized) mna matrix to be filled
 * @param z_vector The empty (zero-initialized) z vector
 * @param component_list The netlist describing the connections between nodes. 0-indexed.
 * @param num_nodes The number of nodes
 * @param num_voltages The number of voltage sources
 * @param num_components The total number of components (effective size of array component_list)
 */
void build_mna_matrix(double complex** mna_matrix, double complex* z_vector, const Component* component_list, const uint8_t num_nodes, const uint8_t num_voltages, const uint8_t num_components){
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

    // z vector 

    // i vector
    for(uint8_t i = 0; i < num_components; ++i){
        const Component comp = component_list[i];
        if(!is_current_src(&comp)) continue;
        z_vector[comp.node_neg] += comp.value;
        z_vector[comp.node_pos] -= comp.value;  
    }

    int voltage_index = 0;
    // e vector
    for(uint8_t i = 0; i < num_components; ++i){
        const Component comp = component_list[i];
        if(!is_voltage_src(&comp)) continue;
        z_vector[starting_index + voltage_index++] = comp.value;  
    }
}

void solve_mna_system_mutates(double complex** mna_matrix, const double complex* z_vector, const uint8_t size, double complex* ans_vector){
    invert_matrix(mna_matrix,size,mna_matrix);
    multiply_matrix_vector(mna_matrix,z_vector,size,ans_vector);
}   

void solve_mna_system(const double complex** inverted_mna_matrix, const double complex* z_vector, const uint8_t size, double complex* ans_vector){
    multiply_matrix_vector(inverted_mna_matrix,z_vector,size,ans_vector);
} 