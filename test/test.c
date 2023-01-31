#include "../rbtree/rbtree.h"

int main() {
    struct rbtree *map = construct_rbtree();
    map->insert(map, 4, 5);
    map->insert(map, 3, 5);
    map->insert(map, 2, 5);
    map->insert(map, 1, 5);
    // map->remove(map, 3);
    map->print(map);
    printf("hellp world %d\n", map->getValue(map, 1));
    return 0;
}