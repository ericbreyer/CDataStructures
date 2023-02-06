#pragma once
#include <stdbool.h>
struct IMap {
    /**
     * @brief pretty print a representation of the IMap
     */
    void (*print)(struct IMap * mapThis, FILE * out,void (*printKey)(void * thing,FILE * out),void (*printVal)(void * thing,FILE * out));

    int (*getKeys)(struct IMap *this, void ***keys);

    /**
     * @brief recursively deletes every node to clear the tree, this tree object still lives
     */
    void (*clear)(struct IMap *this);
    /**
     * @brief makes a new tree with the same contents as this one
     *
     * @return struct IMap*, a pointer to a new tree with the same contents
     */
    struct IMap *(*copy)(struct IMap *this);
    /**
     * @brief checks if the key is contained within the set/map
     *
     * @param[in] key the key to look for
     * @return true if the key is in the set/map
     * @return false otherwise
     */
    bool (*contains)(struct IMap *this, void * key);
    /**
     * @brief get the value of a key in the map.
     *
     * NOTE: this method has undefined behavior if the key is not in the map
     *
     * @param[in] key the key to look for
     * @return V the value of the key if it exists in the map, undefined value otherwise
     */
    void * (*getValue)(struct IMap *this, void * key);
    /**
     * @brief gets the key's value if it is in the map
     *
     * @param[in] key the key to look for
     * @param[out] out the location to put the value if found
     * @return true if the key is in the map
     * @return false otherwise
     */
    bool (*tryGetValue)(struct IMap *this, void * key, void * *out);
    /**
     * @brief Set the value of the key if it is in the map
     *
     * @param[in] key the key to find
     * @param[in] value the value to set
     * @return true if the key is in the map
     * @return false otherwise
     */
    bool (*setValue)(struct IMap *this, void * key, void * value);
    /**
     * @brief insert a key/key-value pair in the map if it does not exist in the map
     *
     * @param[in] key the key to insert
     * @param[in] value [OPTIONAL] the value to insert (optional if only representing a set)
     * @return true if the key is not in the map/set and the value was inserted
     * @return false otherwise
     */
    bool (*insert)(struct IMap *this, void * key, void * value);

    /**
     * @brief removes the key from the set/map if it exists
     *
     * @param[in] key the key to remove
     * @return true if the key was in the map and removed
     * @return false otherwise
     */
    bool (*remove)(struct IMap *this, void * key);
    void (*destroy)(struct IMap *this);
};