/**
 * @brief a class that represents a linked list node
 */
struct llnode {
    /**
     * @brief the next node in the list
     */
    struct llnode *next; // = nullptr;
    /**
     * @brief the prev node in the list
     */
    struct llnode *prev; // = nullptr;
    /**
     * @brief the value stored in this node
     */
    void * value;

};