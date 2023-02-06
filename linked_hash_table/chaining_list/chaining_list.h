#include <stdbool.h>
#include <stdlib.h>
#include "../../generic.h"

struct Node {
    generic_t key;
    generic_t value;
    struct Node * next;
};

struct ChainingList {
    struct Node * root;
};

bool ChainingList_insert(struct ChainingList * this, generic_t key, generic_t value);
bool ChainingList_setValue(struct ChainingList * this, generic_t key, generic_t value);
bool ChainingList_remove(struct ChainingList * this, generic_t key);
bool ChainingList_tryGet(struct ChainingList * this, generic_t key, generic_t*out);
int ChainingList_keys(struct ChainingList * this, generic_t*keys);

struct ChainingList * construct_ChainingList();
void destroy_ChainingList(struct ChainingList * this);