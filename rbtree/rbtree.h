#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma once
#define STRICT_CHECK
#ifndef STRICT_CHECK
#define assert void
#endif

struct rbnode;

struct rbtree {
    /**
     * @brief a pointer to the root node
     */
    struct rbnode *root; // = nullptr;

    // public:
    /**
     * @brief pretty print a representation of the rbtree
     */
    void (*print)(struct rbtree *this, FILE * out);

    int (*getKeys)(struct rbtree *this, int **keys);

    /**
     * @brief recursively deletes every node to clear the tree, this tree object still lives
     */
    void (*clear)(struct rbtree *this);
    /**
     * @brief makes a new tree with the same contents as this one
     *
     * @return struct rbtree*, a pointer to a new tree with the same contents
     */
    struct rbtree *(*copy)(struct rbtree *this);
    /**
     * @brief checks if the key is contained within the set/map
     *
     * @param[in] key the key to look for
     * @return true if the key is in the set/map
     * @return false otherwise
     */
    int (*contains)(struct rbtree *this, int key);
    /**
     * @brief get the value of a key in the map.
     *
     * NOTE: this method has undefined behavior if the key is not in the map
     *
     * @param[in] key the key to look for
     * @return V the value of the key if it exists in the map, undefined value otherwise
     */
    int (*getValue)(struct rbtree *this, int key);
    /**
     * @brief gets the key's value if it is in the map
     *
     * @param[in] key the key to look for
     * @param[out] out the location to put the value if found
     * @return true if the key is in the map
     * @return false otherwise
     */
    int (*tryGetValue)(struct rbtree *this, int key, int *out);
    /**
     * @brief Set the value of the key if it is in the map
     *
     * @param[in] key the key to find
     * @param[in] value the value to set
     * @return true if the key is in the map
     * @return false otherwise
     */
    int (*setValue)(struct rbtree *this, int key, int value);
    /**
     * @brief insert a key/key-value pair in the map if it does not exist in the map
     *
     * @param[in] key the key to insert
     * @param[in] value [OPTIONAL] the value to insert (optional if only representing a set)
     * @return true if the key is not in the map/set and the value was inserted
     * @return false otherwise
     */
    int (*insert)(struct rbtree *this, int key, int value);

    /**
     * @brief removes the key from the set/map if it exists
     *
     * @param[in] key the key to remove
     * @return true if the key was in the map and removed
     * @return false otherwise
     */
    int (*remove)(struct rbtree *this, int key);
    /**
     * @brief checks if the tree is valid in terms of black height property and red red property
     *
     * @return true of the tree is valid
     * @return false otherwise
     */
    int (*checkValid)(struct rbtree *this);
};

void destroy_rbtree(struct rbtree *this);
struct rbtree *construct_rbtree();