#include "types.h"
#include "constants.h"


Component add_independent_component(ComponentType type, double value, uint8_t node_neg, uint8_t node_pos, bool is_target){
    Component comp;
    comp.type = type;
    comp.value = value;
    comp.node_neg = node_neg;
    comp.node_pos = node_pos;
    comp.is_target = is_target;
    comp.control_node_neg = NULL_VALUE;
    comp.control_node_pos = NULL_VALUE;
    comp.control_id = NULL_VALUE;
    return comp;
}

Component add_component(ComponentType type, double value, uint8_t node_neg, uint8_t node_pos, uint8_t control_node_pos, uint8_t control_node_neg, uint8_t control_id, bool is_target){
    Component comp = add_independent_component(type,value,node_neg,node_pos,is_target);
    comp.control_node_neg = control_node_neg;
    comp.control_node_pos = control_node_pos;
    comp.control_id = control_id;
    return comp;
}



