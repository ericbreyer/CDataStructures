#include "./linked_hash_table.h"
#include "./chaining_list/chaining_list.h"

struct LinkedHashTable  {

    struct IMap super;
    
    int numSlots;
    int numKeys;
    float alphaMax;
    struct ChainingList ** underlyingArray;

};

unsigned int LinkedHashTable_hash(struct LinkedHashTable * this, unsigned int x) {
    (void)this;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

float LinkedHashTable_loadFactor(struct LinkedHashTable * this) {
    return (float)this->numKeys / (float)this->numSlots;
}

void LinkedHashTable_resize(struct LinkedHashTable * this, float factor) {
    if (this->numSlots <= 10 && factor < 1) {
        return;
    }
    int prevSlots = this->numSlots;
    this->numSlots = this->numSlots * factor;
    this->numKeys = 0;
    struct ChainingList **oldUnderlyingArray = this->underlyingArray;
    this->underlyingArray = malloc(sizeof *this->underlyingArray * this->numSlots);
    for(int i = 0; i < this->numSlots; ++i) {
        this->underlyingArray[i] = construct_ChainingList();
    }
    for (int i = 0; i < prevSlots; i++) {
        struct ChainingList **temp = oldUnderlyingArray + i;
        struct Node *curr = (*temp)->root;

        while (curr != NULL) {
            this->super.insert((struct IMap *)this, curr->key, curr->value);
            curr = curr->next;
        }
    }
    for(int i = 0; i < prevSlots; ++i) {
        destroy_ChainingList(oldUnderlyingArray[i]);
        free(oldUnderlyingArray[i]);
    }
    free(oldUnderlyingArray);
}

void LinkedHashTable_print(struct IMap * mapThis, FILE * out,void (*printKey)(void * thing,FILE * out),void (*printVal)(void * thing,FILE * out)) {
    struct LinkedHashTable * this = (struct LinkedHashTable *)mapThis;
    fprintf(out, "{");
    for (int i = 0; i < this->numSlots; i++) {
        void **keys = malloc(sizeof(void *) * this->numKeys);
        int len = ChainingList_keys(this->underlyingArray[i], keys);
        for(int j = 0; j < len; ++j) {
            void * val;
            ChainingList_tryGet(this->underlyingArray[i], keys[j], &val);
            printKey(keys[j], out);
            fprintf(out, ": ");
            printVal(val, out);
            fprintf(out, ", ");
        }
    }
    fprintf(out, "}\n");
}

int LinkedHashTable_getKeys(struct IMap * mapThis, void ***keys) {
    struct LinkedHashTable * this = (struct LinkedHashTable *)mapThis;

    (*keys) = malloc(this->numKeys * sizeof(void*));
    int inPtr = 0;
    for (int i = 0; i < this->numSlots; i++) {
        inPtr += ChainingList_keys(this->underlyingArray[i], (*keys) + inPtr);
    }
    return this->numKeys;
};

void LinkedHashTable_clear(struct IMap * mapThis) {
    struct LinkedHashTable * this = (struct LinkedHashTable *)mapThis;
    for (int i = 0; i < this->numSlots; i++) {
        destroy_ChainingList(this->underlyingArray[i]);
    }
    this->numKeys = 0;
}

struct IMap * LinkedHashTable_copy(struct IMap * mapThis) {
    struct LinkedHashTable * this = (struct LinkedHashTable *)mapThis;

    struct IMap * newTable = construct_LinkedHashTable(this->numSlots);

    void **keys = NULL;
    int numKeys = mapThis->getKeys(mapThis, &keys);


    for (int i = 0; i < numKeys; ++i) {
        newTable->insert(newTable, keys[i], mapThis->getValue(mapThis, keys[i]));
    }

    return newTable;
}

bool LinkedHashTable_contains(struct IMap * mapThis, void * key) {
    struct LinkedHashTable * this = (struct LinkedHashTable *)mapThis;

    void * temp;
    int slot = LinkedHashTable_hash(this, key) % this->numSlots;
    return ChainingList_tryGet(this->underlyingArray[slot],key, &temp);
}

void * LinkedHashTable_getValue(struct IMap * mapThis, void * key) {
    struct LinkedHashTable * this = (struct LinkedHashTable *)mapThis;

    void * out;
    int slot = LinkedHashTable_hash(this, key) % this->numSlots;
    ChainingList_tryGet(this->underlyingArray[slot], key, &out);
    return out;
}

bool LinkedHashTable_tryGetValue(struct IMap * mapThis, void * key, void **out) {
    struct LinkedHashTable * this = (struct LinkedHashTable *)mapThis;

    int slot = LinkedHashTable_hash(this, key) % this->numSlots;
    return ChainingList_tryGet(this->underlyingArray[slot], key, out);
}

bool LinkedHashTable_setValue(struct IMap * mapThis, void * key, void * value) {
    struct LinkedHashTable * this = (struct LinkedHashTable *)mapThis;

    int slot = LinkedHashTable_hash(this, key) % this->numSlots;
    return ChainingList_setValue(this->underlyingArray[slot], key, value);
}

bool LinkedHashTable_insert(struct IMap * mapThis, void * key, void * value) {
    struct LinkedHashTable * this = (struct LinkedHashTable *)mapThis;
    int slot = LinkedHashTable_hash(this, key) % this->numSlots;
    bool ret = ChainingList_insert(this->underlyingArray[slot], key, value);
    if (ret) {
        this->numKeys++;
    }
    if (LinkedHashTable_loadFactor(this) - this->alphaMax > 0) {
        LinkedHashTable_resize(this, 2);
    }
    return ret;
}

bool LinkedHashTable_remove(struct IMap * mapThis, void * key) {
    struct LinkedHashTable * this = (struct LinkedHashTable *)mapThis;

    int slot = LinkedHashTable_hash(this, key) % this->numSlots;
    bool ret = ChainingList_remove(this->underlyingArray[slot], key);
    if (ret) {
        this->numKeys--;
    }
    if (LinkedHashTable_loadFactor(this) - (this->alphaMax / 4) < 0) {
        LinkedHashTable_resize(this, .5);
    }
    return ret;
}

void destroy_LinkedHashTable(struct IMap * this) {
    LinkedHashTable_clear(this);
}

static struct IMap vTable = {
    .print = LinkedHashTable_print,
    .clear = LinkedHashTable_clear,
    .contains = LinkedHashTable_contains,
    .destroy = destroy_LinkedHashTable,
    .getKeys = LinkedHashTable_getKeys,
    .getValue = LinkedHashTable_getValue,
    .tryGetValue = LinkedHashTable_tryGetValue,
    .insert = LinkedHashTable_insert,
    .remove = LinkedHashTable_remove,
    .setValue = LinkedHashTable_setValue,
    .copy = LinkedHashTable_copy
};


struct IMap * construct_LinkedHashTable() {
    struct LinkedHashTable * new = malloc(sizeof *new);
    new->numSlots = 10;
    new->numKeys = 0;
    new->alphaMax = 1.5;
    new->underlyingArray = malloc(sizeof *new->underlyingArray * new->numSlots);
    for(int i = 0; i < new->numSlots; ++i) {
        new->underlyingArray[i] = construct_ChainingList();
    }
    new->super = vTable;
    return(struct IMap *)new;
}