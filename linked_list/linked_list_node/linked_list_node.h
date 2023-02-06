#include "../../generic.h"
/**
 * @brief a class that represents a linked list node
 */
struct linked_list_node {
    /**
     * @brief the next node in the list
     */
    struct linked_list_node *next; // = nullptr;
    /**
     * @brief the prev node in the list
     */
    struct linked_list_node *prev; // = nullptr;
    /**
     * @brief the value stored in this node
     */
    generic_t value;

};