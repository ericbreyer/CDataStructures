#include "../linked_hash_table/linked_hash_table.h"
#include "../llist/llist.h"
#include "../llist/llnode/llnode.h"
#include "../rbtree/rbtree.h"
// #include "../bloom/bloom.h"
#include "ref.h"
#include <sys/resource.h>

void printInt(void *thing, FILE *out) {
    fprintf(out, "%d", thing);
}

void rbtree_test_getKeys() {
    struct IMap *map = construct_rbtree();
    for (int i = 0; i < 50; ++i) {
        map->insert(map, i, i);
    }
    void **actual = NULL;
    int expected[] = {15, 23, 31, 39, 43, 45, 47, 48, 49, 46, 44, 41, 42, 40, 35, 37, 38, 36, 33, 34, 32, 27, 29, 30, 28, 25, 26, 24, 19, 21, 22, 20, 17, 18, 16, 7, 11, 13, 14, 12, 9, 10, 8, 3, 5, 6, 4, 1, 2, 0};
    int numKeys = map->getKeys(map, &actual);

    for (int i = 0; i < numKeys; ++i) {
        if (actual[i] != expected[i]) {
            char * t = malloc(100);
            sprintf(t, "❌ rbtree Get Keys Failed at idx %d, expected: %d, got: %d ", i, expected[i], actual[i]);
            fprintf(stderr, "%-45s", t);
            map->destroy(map);
            free(map);
            free(t);
            return;
        }
    }
    fprintf(stdout, "%-45s", "✅ rbtree Get Keys Passed ");
    map->destroy(map);
    free(map);
}
void rbtree_test_clear() {
    struct IMap *test = construct_rbtree();
    for (int i = 0; i < 50; ++i) {
        test->insert(test, i, i);
    }

    test->clear(test);
    int *dummy = NULL;
    int numKeys = test->getKeys(test, &dummy);

    if (numKeys) {
        fprintf(stderr, "%-45s", "❌ rbtree Clear Failed expected no keys got %d keys ", numKeys);
        test->destroy(test);
        free(test);
        return;
    }

    fprintf(stdout, "%-45s", "✅ rbtree Clear Passed ");
    test->destroy(test);
    free(test);
}
void rbtree_test_copy() {
    struct IMap *map = construct_rbtree();
    for (int i = 0; i < 50; ++i) {
        map->insert(map, i, i);
    }
    struct IMap *test = map->copy(map);
    void **actual = NULL;
    int expected[] = {15, 23, 31, 39, 43, 45, 47, 48, 49, 46, 44, 41, 42, 40, 35, 37, 38, 36, 33, 34, 32, 27, 29, 30, 28, 25, 26, 24, 19, 21, 22, 20, 17, 18, 16, 7, 11, 13, 14, 12, 9, 10, 8, 3, 5, 6, 4, 1, 2, 0};
    int numKeys = test->getKeys(test, &actual);

    for (int i = 0; i < numKeys; ++i) {
        if (actual[i] != expected[i]) {
            char * t = malloc(100);
            sprintf(t, "❌ rbtree Copy Failed for copy at idx %d, expected: %d, got: %d ", i, expected[i], actual[i]);
            fprintf(stderr, "%-45s", t);
            map->destroy(test);
            free(test);
            free(t);
            return;
        }
    }
    int numKeys2 = map->getKeys(map, &actual);
    for (int i = 0; i < numKeys2; ++i) {
        if (actual[i] != expected[i]) {
            fprintf(stderr, "%-45s", "❌ rbtree Copy Failed for original at idx %d, expected: %d, got: %d ", i, expected[i], actual[i]);
            map->destroy(test);
            free(test);
            return;
        }
    }

    map->destroy(test);
    free(test);
    fprintf(stdout, "%-45s", "✅ rbtree Copy Passed ");
}
void rbtree_test_contains() {
    struct IMap *map = construct_rbtree();
    for (int i = 0; i < 50; ++i) {
        map->insert(map, i, i);
    }
    if (map->contains(map, 4) == 0) {
        fprintf(stderr, "%-45s", "❌ rbtree Contains failed: expected 4 to be in the map ");
        map->destroy(map);
        free(map);
        return;
    }
    if (map->contains(map, 400)) {
        fprintf(stderr, "%-45s", "❌ rbtree Contains failed: expected 400 not to be in the map ");
        map->destroy(map);
        free(map);
        return;
    }

    fprintf(stdout, "%-45s", "✅ rbtree Contains Passed ");
    map->destroy(map);
    free(map);
}
void rbtree_test_getValue() {
    struct IMap *map = construct_rbtree();
    for (int i = 0; i < 50; ++i) {
        map->insert(map, i, i);
    }
    int actual = map->getValue(map, 4);
    if (actual != 4) {
        fprintf(stderr, "%-45s", "❌ rbtree Get Value failed: expected 4 -> 4 got 4 -> %d ", actual);
        map->destroy(map);
        free(map);
        return;
    }

    fprintf(stdout, "%-45s", "✅ rbtree Get Value Passed ");
    map->destroy(map);
    free(map);
}
void rbtree_test_tryGetValue() {
    struct IMap *map = construct_rbtree();
    for (int i = 0; i < 50; ++i) {
        map->insert(map, i, i);
    }
    int actual;
    if (!map->tryGetValue(map, 4, &actual) || actual != 4) {
        fprintf(stderr, "%-45s", "❌ rbtree Try Get Value failed: expected 4 -> 4 got 4 -> %d ", actual);
        map->destroy(map);
        free(map);
        return;
    }

    if (map->tryGetValue(map, 400, &actual)) {
        fprintf(stderr, "%-45s", "❌ rbtree Try Get Value failed: expected 400 not to be in the map ");
        map->destroy(map);
        free(map);
        return;
    }

    fprintf(stdout, "%-45s", "✅ rbtree Try Get Value Passed ");
    map->destroy(map);
    free(map);
}
void rbtree_test_setValue() {
    struct IMap *map = construct_rbtree();
    for (int i = 0; i < 50; ++i) {
        map->insert(map, i, i);
    }
    map->setValue(map, 5, 2);
    int actual = map->getValue(map, 5);
    if (actual != 2) {
        fprintf(stderr, "%-45s", "❌ rbtree Set Value failed: expected 5 -> 2 got 5 -> %d ", actual);
        map->destroy(map);
        free(map);
        return;
    }

    if (map->setValue(map, 400, 2)) {
        fprintf(stderr, "%-45s", "❌ rbtree Set Value failed: expected 400 not to be in the map ");
        map->destroy(map);
        free(map);
        return;
    }

    fprintf(stdout, "%-45s", "✅ rbtree Set Value Passed ");
    map->destroy(map);
    free(map);
}
void rbtree_test_insert() {
    struct IMap *map = construct_rbtree();
    for (int i = 0; i < 50; ++i) {
        map->insert(map, i, i);
    }
    map->insert(map, 50, 2);
    int actual = map->getValue(map, 50);
    if (actual != 2) {
        fprintf(stderr, "%-45s", "❌ rbtree Insert failed: expected 50 -> 2 got 50 -> %d ", actual);
        map->destroy(map);
        free(map);
        return;
    }

    if (map->insert(map, 40, 2)) {
        fprintf(stderr, "%-45s", "❌ rbtree Insert failed: expected 40 to be in the map ");
        map->destroy(map);
        free(map);
        return;
    }

    fprintf(stdout, "%-45s", "✅ rbtree Insert Passed ");
    map->destroy(map);
    free(map);
}
void rbtree_test_remove() {
    struct IMap *map = construct_rbtree();
    for (int i = 0; i < 50; ++i) {
        map->insert(map, i, i);
    }
    if (!map->remove(map, 30)) {
        fprintf(stderr, "%-45s", "❌ rbtree Remove failed: expected 30 in map ");
        map->destroy(map);
        free(map);
        return;
    }

    if (map->remove(map, 400)) {
        fprintf(stderr, "%-45s", "❌ rbtree Remove failed: expected 400 not to be in the map ");
        map->destroy(map);
        free(map);
        return;
    }

    fprintf(stdout, "%-45s", "✅ rbtree Remove Passed ");
    map->destroy(map);
    free(map);
}

void llist_test_copy() {
        struct llist *list = construct_llist();
    for (int i = 0; i < 10; ++i) {
        list->append(list, i);
    }
    list = list->copy(list);
    void **actual = NULL;
    int expected[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    list->toArray(list, &actual);
    for (int i = 0; i < list->length(list); ++i) {
        if (actual[i] != expected[i]) {
            fprintf(stderr, "%-45s", "❌ llist Copy Failed for original at idx %d, expected: %d, got: %d ", i, expected[i], (int)actual[i]);
            destroy_llist(list);
            free(list);
            return;
        }
    }
    fprintf(stdout, "%-45s", "✅ llist Copy Passed ");
    destroy_llist(list);
    free(list);
}
void llist_test_toArray() {
        struct llist *list = construct_llist();
    for (int i = 0; i < 10; ++i) {
        list->append(list, i);
    }
    void **actual = NULL;
    int expected[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    list->toArray(list, &actual);
    for (int i = 0; i < list->length(list); ++i) {
        if (actual[i] != expected[i]) {
            fprintf(stderr, "%-45s", "❌ llist To Array Failed for original at idx %d, expected: %d, got: %d ", i, expected[i], (int)actual[i]);
            destroy_llist(list);
            free(list);
            return;
        }
    }
    fprintf(stdout, "%-45s", "✅ llist To Array Passed ");
    destroy_llist(list);
    free(list);
}
void llist_test_length() {
        struct llist *list = construct_llist();
    for (int i = 0; i < 10; ++i) {
        list->append(list, i);
    }
    int actual = list->length(list);
    if (actual != 10) {
        fprintf(stderr, "%-45s", "❌ llist length Failed expected: 10, got: %d ", actual);
        destroy_llist(list);
        free(list);
        return;
    }

    fprintf(stdout, "%-45s", "✅ llist length Passed ");
    destroy_llist(list);
    free(list);
}
void llist_test_clear() {
        struct llist *list = construct_llist();
    for (int i = 0; i < 10; ++i) {
        list->append(list, i);
    }
    list->clear(list);
    int actual = list->length(list);
    if (actual != 0) {
        fprintf(stderr, "%-45s", "❌ llist clear Failed ");
        destroy_llist(list);
        free(list);
        return;
    }
    fprintf(stdout, "%-45s", "✅ llist clear Passed ");
    destroy_llist(list);
    free(list);
}
void llist_test_contains() {
        struct llist *list = construct_llist();
    for (int i = 0; i < 10; ++i) {
        list->append(list, i);
    }
    int actual = list->contains(list, 5);
    if (actual != true) {
        fprintf(stderr, "%-45s", "❌ llist contains Failed: expected 5 in list ");
        destroy_llist(list);
        free(list);
        return;
    }
    actual = list->contains(list, 50);
    if (actual != false) {
        fprintf(stderr, "%-45s", "❌ llist contains Failed: expected 50 not in map ");
        destroy_llist(list);
        free(list);
        return;
    }
    fprintf(stdout, "%-45s", "✅ llist contains Passed ");
    destroy_llist(list);
    free(list);
}
void llist_test_find() {
        struct llist *list = construct_llist();
    for (int i = 0; i < 10; ++i) {
        list->append(list, i);
    }
    list = list->copy(list);
    int actual = list->find(list, 5);
    if (actual != 5) {
        fprintf(stderr, "%-45s", "❌ llist find Failed: expected 5 at idx 5 got idx %d ", actual);
        destroy_llist(list);
        free(list);
        return;
    }
    actual = list->find(list, 50);
    if (actual != NOT_FOUND) {
        fprintf(stderr, "%-45s", "❌ llist find Failed: expected 50 not in list got 50 at idx %d ", (int)actual);
        destroy_llist(list);
        free(list);
        return;
    }
    fprintf(stdout, "%-45s", "✅ llist find Passed ");
    destroy_llist(list);
    free(list);
}
void llist_test_get() {
        struct llist *list = construct_llist();
    for (int i = 0; i < 10; ++i) {
        list->append(list, i);
    }
    int actual = list->get(list, 5);
    if (actual != 5) {
        fprintf(stderr, "%-45s", "❌ llist get Failed: expected 5 at idx 5 got %d ", (int)actual);
        destroy_llist(list);
        free(list);
        return;
    }
    fprintf(stdout, "%-45s", "✅ llist get Passed ");
    destroy_llist(list);
    free(list);
}
void llist_test_set() {
        struct llist *list = construct_llist();
    for (int i = 0; i < 10; ++i) {
        list->append(list, i);
    }
    list->set(list, 5, 10);
    int actual = list->get(list, 5);
    if (actual != 10) {
        fprintf(stderr, "%-45s", "❌ llist set Failed: expected 10 at idx 5 got %d ", (int)actual);
        destroy_llist(list);
        free(list);
        return;
    }
    fprintf(stdout, "%-45s", "✅ llist set Passed ");
    destroy_llist(list);
    free(list);
}
void llist_test_append() {
        struct llist *list = construct_llist();
    for (int i = 0; i < 10; ++i) {
        list->append(list, i);
    }
    list->append(list, 10);
    void **actual = NULL;
    int expected[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    list->toArray(list, &actual);
    for (int i = 0; i < list->length(list); ++i) {
        if ((int)actual[i] != expected[i]) {
            char *t = malloc(sizeof *t * 100);
            sprintf("❌ llist append Failed for original at idx %d, expected: %d, got: %d ", i, expected[i], (int)actual[i]);
            fprintf(stderr, "%-45s", t);
            free(t);
            destroy_llist(list);
            free(list);
            return;
        }
    }
    fprintf(stdout, "%-45s", "✅ llist append Passed ");
    destroy_llist(list);
    free(list);
}
void llist_test_prepend() {
        struct llist *list = construct_llist();
    for (int i = 0; i < 10; ++i) {
        list->append(list, i);
    }
    list->prepend(list, 10);
    void **actual = NULL;
    int expected[] = {10, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    list->toArray(list, &actual);
    for (int i = 0; i < list->length(list); ++i) {
        if ((int)actual[i] != expected[i]) {
            char *t = malloc(sizeof *t * 100);
            sprintf(t, "❌ llist prepend Failed for original at idx %d, expected: %d, got: %d ", i, expected[i], (int)actual[i]);
            fprintf(stderr, "%-45s", t);
            free(t);
            destroy_llist(list);
            free(list);
            return;
        }
    }
    fprintf(stdout, "%-45s", "✅ llist prepend Passed ");
    destroy_llist(list);
    free(list);
}
void llist_test_insert() {
        struct llist *list = construct_llist();
    for (int i = 0; i < 10; ++i) {
        list->append(list, i);
    }
    list->insert(list, 5, 10);
    void **actual = NULL;
    int expected[] = {0, 1, 2, 3, 4, 10, 5, 6, 7, 8, 9};
    list->toArray(list, &actual);
    for (int i = 0; i < list->length(list); ++i) {
        if ((int)actual[i] != expected[i]) {
            char *t = malloc(sizeof *t * 100);
            sprintf(t, "❌ llist insert Failed for original at idx %d, expected: %d, got: %d ", i, expected[i], (int)actual[i]);
            fprintf(stderr, "%-45s", t);
            free(t);
            destroy_llist(list);
            free(list);
            return;
        }
    }
    fprintf(stdout, "%-45s", "✅ llist insert Passed ");
    destroy_llist(list);
    free(list);
}
void llist_test_remove() {
        struct llist *list = construct_llist();
    for (int i = 0; i < 10; ++i) {
        list->append(list, i);
    }
    list->remove(list, 5);
    void **actual = NULL;
    int expected[] = {0, 1, 2, 3, 4, 6, 7, 8, 9};
    list->toArray(list, &actual);
    for (int i = 0; i < list->length(list); ++i) {
        if ((int)actual[i] != expected[i]) {
            char *t = malloc(sizeof *t * 100);
            sprintf(t, "❌ llist remove Failed for original at idx %d, expected: %d, got: %d ", i, expected[i], (int)actual[i]);
            fprintf(stderr, "%-45s", t);
            free(t);
            destroy_llist(list);
            free(list);
            return;
        }
    }
    fprintf(stdout, "%-45s", "✅ llist remove Passed ");
    destroy_llist(list);
    free(list);
}
void llist_test_sort() {
        struct llist *list = construct_llist();
    for (int i = 0; i < 10; ++i) {
        list->append(list, i);
    }
    list->clear(list);
    for (int i = 99; i >= 0; --i) {
        list->append(list, i);
    }
    list->sort(list);
    void **actual;

    list->toArray(list, &actual);

    struct llist *ref = construct_llist();
    for (int i = 0; i < 100; ++i) {
        ref->append(ref, i);
    }
    void **expected;
    ref->toArray(ref, &expected);

    for (int i = 0; i < list->length(list); ++i) {
        if ((int)actual[i] != (int)expected[i]) {
            char *t = malloc(sizeof *t * 100);
            sprintf(t, "❌ llist sort Failed for original at idx %d, expected: %d, got: %d ", i, (int)expected[i], (int)actual[i]);
            fprintf(stderr, "%-45s", t);
            free(t);
            destroy_llist(list);
            free(list);
            return;
        }
    }
    fprintf(stdout, "%-45s", "✅ llist sort Passed ");

    destroy_llist(list);
    free(list);
    destroy_llist(ref);
    free(ref);
}

void LinkedHashTable_test_getKeys() {
    struct IMap *map = construct_LinkedHashTable();
    for (int i = 0; i < 50; ++i) {
        map->insert(map, i, i);
    }
    void **actual = NULL;
    int expected[] = {0, 43, 45, 5, 21, 7, 10, 26, 42, 14, 33, 6, 20, 32, 38, 8, 49, 1, 31, 36, 40, 39, 41, 11, 22, 47, 13, 48, 17, 34, 24, 4, 19, 30, 46, 9, 12, 18, 25, 3, 15, 27, 28, 2, 23, 35, 37, 44, 29, 16};
    int numKeys = map->getKeys(map, &actual);

    for (int i = 0; i < numKeys; ++i) {
        if (actual[i] != expected[i]) {
            char * t = malloc(100);
            sprintf(t, "❌ LinkedHashTable Get Keys Failed at idx %d, expected: %d, got: %d ", i, expected[i], actual[i]);
            fprintf(stderr, "%-45s", t);
            map->destroy(map);
            free(map);
            free(t);
            return;
        }
    }
    fprintf(stdout, "%-45s", "✅ LinkedHashTable Get Keys Passed ");
    map->destroy(map);
    free(map);
}
void LinkedHashTable_test_clear() {
    struct IMap *test = construct_LinkedHashTable();
    for (int i = 0; i < 50; ++i) {
        test->insert(test, i, i);
    }

    test->clear(test);
    void **dummy = NULL;
    int numKeys = test->getKeys(test, &dummy);

    if (numKeys) {
        fprintf(stderr, "%-45s", "❌ LinkedHashTable Clear Failed expected no keys got %d keys ", numKeys);
        test->destroy(test);
        free(test);
        return;
    }

    fprintf(stdout, "%-45s", "✅ LinkedHashTable Clear Passed ");
    test->destroy(test);
    free(test);
}
void LinkedHashTable_test_copy() {
    struct IMap *map = construct_LinkedHashTable();
    for (int i = 0; i < 50; ++i) {
        map->insert(map, i, i);
    }
    struct IMap *test = map->copy(map);
    void **actual = NULL;
    int expected[] = {0, 43, 45, 5, 21, 7, 10, 26, 42, 14, 33, 6, 20, 32, 38, 8, 49, 1, 31, 36, 40, 39, 41, 11, 22, 47, 13, 48, 17, 34, 24, 4, 19, 30, 46, 9, 12, 18, 25, 3, 15, 27, 28, 2, 23, 35, 37, 44, 29, 16};
    int numKeys = test->getKeys(test, &actual);

    for (int i = 0; i < numKeys; ++i) {
        if (actual[i] != expected[i]) {
            char * t = malloc(100);
            sprintf(t, "❌ LinkedHashTable Copy Failed for copy at idx %d, expected: %d, got: %d ", i, expected[i], actual[i]);
            fprintf(stderr, "%-45s", t);
            map->destroy(test);
            free(test);
            free(t);
            return;
        }
    }
    int numKeys2 = map->getKeys(map, &actual);
    for (int i = 0; i < numKeys2; ++i) {
        if (actual[i] != expected[i]) {
            fprintf(stderr, "%-45s", "❌ LinkedHashTable Copy Failed for original at idx %d, expected: %d, got: %d ", i, expected[i], actual[i]);
            map->destroy(test);
            free(test);
            return;
        }
    }

    map->destroy(test);
    free(test);
    fprintf(stdout, "%-45s", "✅ LinkedHashTable Copy Passed ");
}
void LinkedHashTable_test_contains() {
    struct IMap *map = construct_LinkedHashTable();
    for (int i = 0; i < 50; ++i) {
        map->insert(map, i, i);
    }
    if (map->contains(map, 4) == 0) {
        fprintf(stderr, "%-45s", "❌ LinkedHashTable Contains failed: expected 4 to be in the map ");
        map->destroy(map);
        free(map);
        return;
    }
    if (map->contains(map, 400)) {
        fprintf(stderr, "%-45s", "❌ LinkedHashTable Contains failed: expected 400 not to be in the map ");
        map->destroy(map);
        free(map);
        return;
    }

    fprintf(stdout, "%-45s", "✅ LinkedHashTable Contains Passed ");
    map->destroy(map);
    free(map);
}
void LinkedHashTable_test_getValue() {
    struct IMap *map = construct_LinkedHashTable();
    for (int i = 0; i < 50; ++i) {
        map->insert(map, i, i);
    }
    int actual = map->getValue(map, 4);
    if (actual != 4) {
        fprintf(stderr, "%-45s", "❌ LinkedHashTable Get Value failed: expected 4 -> 4 got 4 -> %d ", actual);
        map->destroy(map);
        free(map);
        return;
    }

    fprintf(stdout, "%-45s", "✅ LinkedHashTable Get Value Passed ");
    map->destroy(map);
    free(map);
}
void LinkedHashTable_test_tryGetValue() {
    struct IMap *map = construct_LinkedHashTable();
    for (int i = 0; i < 50; ++i) {
        map->insert(map, i, i);
    }
    int actual;
    if (!map->tryGetValue(map, 4, &actual) || actual != 4) {
        fprintf(stderr, "%-45s", "❌ LinkedHashTable Try Get Value failed: expected 4 -> 4 got 4 -> %d ", actual);
        map->destroy(map);
        free(map);
        return;
    }

    if (map->tryGetValue(map, 400, &actual)) {
        fprintf(stderr, "%-45s", "❌ LinkedHashTable Try Get Value failed: expected 400 not to be in the map ");
        map->destroy(map);
        free(map);
        return;
    }

    fprintf(stdout, "%-45s", "✅ LinkedHashTable Try Get Value Passed ");
    map->destroy(map);
    free(map);
}
void LinkedHashTable_test_setValue() {
    struct IMap *map = construct_LinkedHashTable();
    for (int i = 0; i < 50; ++i) {
        map->insert(map, i, i);
    }
    map->setValue(map, 5, 2);
    int actual = map->getValue(map, 5);
    if (actual != 2) {
        fprintf(stderr, "%-45s", "❌ LinkedHashTable Set Value failed: expected 5 -> 2 got 5 -> %d ", actual);
        map->destroy(map);
        free(map);
        return;
    }

    if (map->setValue(map, 400, 2)) {
        fprintf(stderr, "%-45s", "❌ LinkedHashTable Set Value failed: expected 400 not to be in the map ");
        map->destroy(map);
        free(map);
        return;
    }

    fprintf(stdout, "%-45s", "✅ LinkedHashTable Set Value Passed ");
    map->destroy(map);
    free(map);
}
void LinkedHashTable_test_insert() {
    struct IMap *map = construct_LinkedHashTable();
    for (int i = 0; i < 50; ++i) {
        map->insert(map, i, i);
    }
    map->insert(map, 50, 2);
    int actual = map->getValue(map, 50);
    if (actual != 2) {
        fprintf(stderr, "%-45s", "❌ LinkedHashTable Insert failed: expected 50 -> 2 got 50 -> %d ", actual);
        map->destroy(map);
        free(map);
        return;
    }

    if (map->insert(map, 40, 2)) {
        fprintf(stderr, "%-45s", "❌ LinkedHashTable Insert failed: expected 40 to be in the map ");
        map->destroy(map);
        free(map);
        return;
    }

    fprintf(stdout, "%-45s", "✅ LinkedHashTable Insert Passed ");
    map->destroy(map);
    free(map);
}
void LinkedHashTable_test_remove() {
    struct IMap *map = construct_LinkedHashTable();
    for (int i = 0; i < 50; ++i) {
        map->insert(map, i, i);
    }
    map->remove(map, 30);
    if (map->contains(map, 30)) {
        fprintf(stderr, "%-45s", "❌ LinkedHashTable Remove failed: expected 30 to be removed in map ");
        map->destroy(map);
        free(map);
        return;
    }

    if (map->remove(map, 400)) {
        fprintf(stderr, "%-45s", "❌ LinkedHashTable Remove failed: expected 400 not to be in the map ");
        map->destroy(map);
        free(map);
        return;
    }

    fprintf(stdout, "%-45s", "✅ LinkedHashTable Remove Passed ");
    map->destroy(map);
    free(map);
}


int main(void) {
    printf("--------RBTREE TESTS-------- \n");
    rbtree_test_copy();
    rbtree_test_remove();
    rbtree_test_getKeys();
    printf("\n");
    rbtree_test_clear();
    rbtree_test_contains();
    rbtree_test_getValue();
    printf("\n");
    rbtree_test_tryGetValue();
    rbtree_test_setValue();
    rbtree_test_insert();
    printf("\n");
    printf("--------LLIST TESTS-------- \n");
    llist_test_copy();
    llist_test_toArray();
    llist_test_length();
    printf("\n");
    llist_test_find();
    llist_test_get();
    llist_test_set();
    printf("\n");
    llist_test_append();
    llist_test_prepend();
    llist_test_insert();
    printf("\n");
    llist_test_remove();
    llist_test_sort();
    llist_test_clear();
    printf("\n");
    llist_test_contains();
    printf("\n");
     printf("--------LinkedHashTable TESTS-------- \n");
    LinkedHashTable_test_copy();
    LinkedHashTable_test_getKeys();
    LinkedHashTable_test_clear();
    printf("\n");
    LinkedHashTable_test_contains();
    LinkedHashTable_test_getValue();
    LinkedHashTable_test_tryGetValue();
        printf("\n");
    LinkedHashTable_test_setValue();
    LinkedHashTable_test_insert();
    LinkedHashTable_test_remove();
    printf("\n");

    return 0;
}