#include <stdbool.h>
#include <stdlib.h>

struct Node {
    void * key;
    void * value;
    struct Node * next;
};

struct ChainingList {
    struct Node * root;
};

bool ChainingList_insert(struct ChainingList * this, void *key, void *value);
bool ChainingList_setValue(struct ChainingList * this, void *key, void *value);
bool ChainingList_remove(struct ChainingList * this, void *key);
bool ChainingList_tryGet(struct ChainingList * this, void *key, void **out);
int ChainingList_keys(struct ChainingList * this, void **keys);

struct ChainingList * construct_ChainingList();
void destroy_ChainingList(struct ChainingList * this);