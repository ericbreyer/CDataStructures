#include "../rbtree/rbtree.h"
#include <sys/resource.h>

int main() {
    struct rbtree *map = construct_rbtree();
    for (int i = 0; i < 1000; ++i) {
        // printf("%d\n", i);
        map->insert(map, i, i);
    }
    map->print(map);
    printf("hellp world %'d\n", map->getValue(map, 1));
    return 0;
}