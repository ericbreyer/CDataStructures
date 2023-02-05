#include "../llist/llist.h"
#include "../llist/llnode/llnode.h"
#include "../rbtree/rbtree.h"
//#include "../bloom/bloom.h"
#include "ref.h"
#include <sys/resource.h>
void rbtree_test_print(struct rbtree *map) {
    FILE *f = fopen("./test/test.txt", "w+");
    map->print(map, f);

    char *actual = malloc(sizeof *actual * strlen(ref));

    fseek(f, 0, SEEK_SET);
    fread(actual, sizeof *ref, strlen(ref), f);
    fclose(f);

    if (strcmp(actual, ref) != 0) {
        fprintf(stderr, "%-35s", "❌ rbtree Print Failed ");
        free(actual);
        return;
    }
    fprintf(stdout, "%-35s", "✅ rbtree Print Passed ");
    free(actual);
}

void rbtree_test_getKeys(struct rbtree *map) {
    map = map->copy(map);
    int *actual = NULL;
    int expected[] = {15, 23, 31, 39, 43, 45, 47, 48, 49, 46, 44, 41, 42, 40, 35, 37, 38, 36, 33, 34, 32, 27, 29, 30, 28, 25, 26, 24, 19, 21, 22, 20, 17, 18, 16, 7, 11, 13, 14, 12, 9, 10, 8, 3, 5, 6, 4, 1, 2, 0};
    int numKeys = map->getKeys(map, &actual);

    for (int i = 0; i < numKeys; ++i) {
        if (actual[i] != expected[i]) {
            fprintf(stderr, "%-35s", "❌ rbtree Get Keys Failed at idx %d, expected: %d, got: %d ", i, expected[i], actual[i]);
            destroy_rbtree(map);
            free(map);
            return;
        }
    }
    fprintf(stdout, "%-35s", "✅ rbtree Get Keys Passed ");
    destroy_rbtree(map);
    free(map);
}

void rbtree_test_clear(struct rbtree *map) {

    struct rbtree *test = map->copy(map);

    test->clear(test);
    int *dummy = NULL;
    int numKeys = test->getKeys(test, &dummy);

    if (numKeys) {
        fprintf(stderr, "%-35s", "❌ rbtree Clear Failed expected no keys got %d keys ", numKeys);
        destroy_rbtree(test);
        free(test);
        return;
    }

    fprintf(stdout, "%-35s", "✅ rbtree Clear Passed ");
    destroy_rbtree(test);
    free(test);
}

void rbtree_test_copy(struct rbtree *map) {

    struct rbtree *test = map->copy(map);

    int *actual = NULL;
    int expected[] = {15, 23, 31, 39, 43, 45, 47, 48, 49, 46, 44, 41, 42, 40, 35, 37, 38, 36, 33, 34, 32, 27, 29, 30, 28, 25, 26, 24, 19, 21, 22, 20, 17, 18, 16, 7, 11, 13, 14, 12, 9, 10, 8, 3, 5, 6, 4, 1, 2, 0};
    int numKeys = test->getKeys(test, &actual);

    for (int i = 0; i < numKeys; ++i) {
        if (actual[i] != expected[i]) {
            fprintf(stderr, "%-35s", "❌ rbtree Copy Failed for copy at idx %d, expected: %d, got: %d ", i, expected[i], actual[i]);
            destroy_rbtree(test);
            free(test);
            return;
        }
    }
    int numKeys2 = map->getKeys(map, &actual);
    for (int i = 0; i < numKeys2; ++i) {
        if (actual[i] != expected[i]) {
            fprintf(stderr, "%-35s", "❌ rbtree Copy Failed for original at idx %d, expected: %d, got: %d ", i, expected[i], actual[i]);
            destroy_rbtree(test);
            free(test);
            return;
        }
    }

    destroy_rbtree(test);
    free(test);
    fprintf(stdout, "%-35s", "✅ rbtree Copy Passed ");
}

void rbtree_test_contains(struct rbtree *map) {
    map = map->copy(map);
    if (map->contains(map, 4) == 0) {
        fprintf(stderr, "%-35s", "❌ rbtree Contains failed: expected 4 to be in the map ");
        destroy_rbtree(map);
        free(map);
        return;
    }
    if (map->contains(map, 400)) {
        fprintf(stderr, "%-35s", "❌ rbtree Contains failed: expected 400 not to be in the map ");
        destroy_rbtree(map);
        free(map);
        return;
    }

    fprintf(stdout, "%-35s", "✅ rbtree Contains Passed ");
    destroy_rbtree(map);
    free(map);
}

void rbtree_test_getValue(struct rbtree *map) {
    map = map->copy(map);
    int actual = map->getValue(map, 4);
    if (actual != 4) {
        fprintf(stderr, "%-35s", "❌ rbtree Get Value failed: expected 4 -> 4 got 4 -> %d ", actual);
        destroy_rbtree(map);
        free(map);
        return;
    }

    fprintf(stdout, "%-35s", "✅ rbtree Get Value Passed ");
    destroy_rbtree(map);
    free(map);
}

void rbtree_test_tryGetValue(struct rbtree *map) {
    map = map->copy(map);
    int actual;
    if (!map->tryGetValue(map, 4, &actual) || actual != 4) {
        fprintf(stderr, "%-35s", "❌ rbtree Try Get Value failed: expected 4 -> 4 got 4 -> %d ", actual);
        destroy_rbtree(map);
        free(map);
        return;
    }

    if (map->tryGetValue(map, 400, &actual)) {
        fprintf(stderr, "%-35s", "❌ rbtree Try Get Value failed: expected 400 not to be in the map ");
        destroy_rbtree(map);
        free(map);
        return;
    }

    fprintf(stdout, "%-35s", "✅ rbtree Try Get Value Passed ");
    destroy_rbtree(map);
    free(map);
}

void rbtree_test_setValue(struct rbtree *map) {
    map = map->copy(map);
    map->setValue(map, 5, 2);
    int actual = map->getValue(map, 5);
    if (actual != 2) {
        fprintf(stderr, "%-35s", "❌ rbtree Set Value failed: expected 5 -> 2 got 5 -> %d ", actual);
        destroy_rbtree(map);
        free(map);
        return;
    }

    if (map->setValue(map, 400, 2)) {
        fprintf(stderr, "%-35s", "❌ rbtree Set Value failed: expected 400 not to be in the map ");
        destroy_rbtree(map);
        free(map);
        return;
    }

    fprintf(stdout, "%-35s", "✅ rbtree Set Value Passed ");
    destroy_rbtree(map);
    free(map);
}

void rbtree_test_insert(struct rbtree *map) {
    map = map->copy(map);
    map->insert(map, 50, 2);
    int actual = map->getValue(map, 50);
    if (actual != 2) {
        fprintf(stderr, "%-35s", "❌ rbtree Insert failed: expected 50 -> 2 got 50 -> %d ", actual);
        destroy_rbtree(map);
        free(map);
        return;
    }

    if (map->insert(map, 40, 2)) {
        fprintf(stderr, "%-35s", "❌ rbtree Insert failed: expected 40 to be in the map ");
        destroy_rbtree(map);
        free(map);
        return;
    }

    fprintf(stdout, "%-35s", "✅ rbtree Insert Passed ");
    destroy_rbtree(map);
    free(map);
}

void rbtree_test_remove(struct rbtree *map) {
    map = map->copy(map);
    if (!map->remove(map, 30)) {
        fprintf(stderr, "%-35s", "❌ rbtree Remove failed: expected 30 in map ");
        destroy_rbtree(map);
        free(map);
        return;
    }

    if (map->remove(map, 400)) {
        fprintf(stderr, "%-35s", "❌ rbtree Remove failed: expected 400 not to be in the map ");
        destroy_rbtree(map);
        free(map);
        return;
    }

    fprintf(stdout, "%-35s", "✅ rbtree Remove Passed ");
    destroy_rbtree(map);
    free(map);
}

void rbtree_test_checkValid(struct rbtree *map) {
    map = map->copy(map);
    if (!map->checkValid(map)) {
        fprintf(stderr, "%-35s", "❌ rbtree Check Valid failed ");
        destroy_rbtree(map);
        free(map);
        return;
    }

    fprintf(stdout, "%-35s", "✅ rbtree Check Valid Passed ");
    destroy_rbtree(map);
    free(map);
}

void llist_test_copy(struct llist *list) {
    list = list->copy(list);
    int *actual = NULL;
    int expected[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    list->toArray(list, &actual);
    for (int i = 0; i < list->length(list); ++i) {
        if (actual[i] != expected[i]) {
            fprintf(stderr, "%-35s", "❌ llist Copy Failed for original at idx %d, expected: %d, got: %d ", i, expected[i], actual[i]);
            destroy_llist(list);
            free(list);
            return;
        }
    }
    fprintf(stdout, "%-35s", "✅ llist Copy Passed ");
    destroy_llist(list);
    free(list);
}
void llist_test_toArray(struct llist *list) {
    list = list->copy(list);
    int *actual = NULL;
    int expected[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    list->toArray(list, &actual);
    for (int i = 0; i < list->length(list); ++i) {
        if (actual[i] != expected[i]) {
            fprintf(stderr, "%-35s", "❌ llist To Array Failed for original at idx %d, expected: %d, got: %d ", i, expected[i], actual[i]);
            destroy_llist(list);
            free(list);
            return;
        }
    }
    fprintf(stdout, "%-35s", "✅ llist To Array Passed ");
    destroy_llist(list);
    free(list);
}
void llist_test_length(struct llist *list) {
    list = list->copy(list);
    int actual = list->length(list);
    if (actual != 10) {
        fprintf(stderr, "%-35s", "❌ llist length Failed expected: 10, got: %d ", actual);
        destroy_llist(list);
        free(list);
        return;
    }

    fprintf(stdout, "%-35s", "✅ llist length Passed ");
    destroy_llist(list);
    free(list);
}
void llist_test_print(struct llist *list) {
    FILE *f = fopen("./test/test.txt", "w+");
    list->print(list, f);
    char *ref = "[0, 1, 2, 3, 4, 5, 6, 7, 8, 9, \b\b]";
    char *actual = malloc(sizeof *actual * strlen(ref));

    fseek(f, 0, SEEK_SET);
    fread(actual, sizeof *ref, strlen(ref), f);
    fclose(f);

    if (strcmp(actual, ref) != 0) {
        fprintf(stderr, "%-35s", "❌ llist Print Failed expected: %s, got: %s ", ref, actual);
        free(actual);
        return;
    }
    fprintf(stdout, "%-35s", "✅ llist Print Passed ");
    free(actual);
}
void llist_test_clear(struct llist *list) {
    list = list->copy(list);
    list->clear(list);
    int actual = list->length(list);
    if (actual != 0) {
        fprintf(stderr, "%-35s", "❌ llist clear Failed ");
        destroy_llist(list);
        free(list);
        return;
    }
    fprintf(stdout, "%-35s", "✅ llist clear Passed ");
    destroy_llist(list);
    free(list);
}
void llist_test_contains(struct llist *list) {
    list = list->copy(list);
    int actual = list->contains(list, 5);
    if (actual != true) {
        fprintf(stderr, "%-35s", "❌ llist contains Failed: expected 5 in list ");
        destroy_llist(list);
        free(list);
        return;
    }
    actual = list->contains(list, 50);
    if (actual != false) {
        fprintf(stderr, "%-35s", "❌ llist contains Failed: expected 50 not in map ");
        destroy_llist(list);
        free(list);
        return;
    }
    fprintf(stdout, "%-35s", "✅ llist contains Passed ");
    destroy_llist(list);
    free(list);
}
void llist_test_find(struct llist *list) {
    list = list->copy(list);
    int actual = list->find(list, 5);
    if (actual != 5) {
        fprintf(stderr, "%-35s", "❌ llist find Failed: expected 5 at idx 5 got idx %d ", actual);
        destroy_llist(list);
        free(list);
        return;
    }
    actual = list->find(list, 50);
    if (actual != NOT_FOUND) {
        fprintf(stderr, "%-35s", "❌ llist find Failed: expected 50 not in list got 50 at idx %d ", actual);
        destroy_llist(list);
        free(list);
        return;
    }
    fprintf(stdout, "%-35s", "✅ llist find Passed ");
    destroy_llist(list);
    free(list);
}
void llist_test_get(struct llist *list) {
    list = list->copy(list);
    int actual = list->get(list, 5);
    if (actual != 5) {
        fprintf(stderr, "%-35s", "❌ llist get Failed: expected 5 at idx 5 got %d ", actual);
        destroy_llist(list);
        free(list);
        return;
    }
    fprintf(stdout, "%-35s", "✅ llist get Passed ");
    destroy_llist(list);
    free(list);
}
void llist_test_set(struct llist *list) {
    list = list->copy(list);
    list->set(list, 5, 10);
    int actual = list->get(list, 5);
    if (actual != 10) {
        fprintf(stderr, "%-35s", "❌ llist set Failed: expected 10 at idx 5 got %d ", actual);
        destroy_llist(list);
        free(list);
        return;
    }
    fprintf(stdout, "%-35s", "✅ llist set Passed ");
    destroy_llist(list);
    free(list);
}
void llist_test_append(struct llist *list) {
    list = list->copy(list);
    list->append(list, 10);
    int *actual = NULL;
    int expected[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    list->toArray(list, &actual);
    for (int i = 0; i < list->length(list); ++i) {
        if (actual[i] != expected[i]) {
            char *t = malloc(sizeof *t * 100);
            sprintf("❌ llist append Failed for original at idx %d, expected: %d, got: %d ", i, expected[i], actual[i]);
            fprintf(stderr, "%-35s", t);
            free(t);
            destroy_llist(list);
            free(list);
            return;
        }
    }
    fprintf(stdout, "%-35s", "✅ llist append Passed ");
    destroy_llist(list);
    free(list);
}
void llist_test_prepend(struct llist *list) {
    list = list->copy(list);
    list->prepend(list, 10);
    int *actual = NULL;
    int expected[] = {10, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    list->toArray(list, &actual);
    for (int i = 0; i < list->length(list); ++i) {
        if (actual[i] != expected[i]) {
            char *t = malloc(sizeof *t * 100);
            sprintf(t, "❌ llist prepend Failed for original at idx %d, expected: %d, got: %d ", i, expected[i], actual[i]);
            fprintf(stderr, "%-35s", t);
            free(t);
            destroy_llist(list);
            free(list);
            return;
        }
    }
    fprintf(stdout, "%-35s", "✅ llist prepend Passed ");
    destroy_llist(list);
    free(list);
}
void llist_test_insert(struct llist *list) {
    list = list->copy(list);
    list->insert(list, 5, 10);
    int *actual = NULL;
    int expected[] = {0, 1, 2, 3, 4, 10, 5, 6, 7, 8, 9};
    list->toArray(list, &actual);
    for (int i = 0; i < list->length(list); ++i) {
        if (actual[i] != expected[i]) {
            char *t = malloc(sizeof *t * 100);
            sprintf(t, "❌ llist insert Failed for original at idx %d, expected: %d, got: %d ", i, expected[i], actual[i]);
            fprintf(stderr, "%-35s", t);
            free(t);
            destroy_llist(list);
            free(list);
            return;
        }
    }
    fprintf(stdout, "%-35s", "✅ llist insert Passed ");
    destroy_llist(list);
    free(list);
}
void llist_test_remove(struct llist *list) {
    list = list->copy(list);
    list->remove(list, 5);
    int *actual = NULL;
    int expected[] = {0, 1, 2, 3, 4, 6, 7, 8, 9};
    list->toArray(list, &actual);
    for (int i = 0; i < list->length(list); ++i) {
        if (actual[i] != expected[i]) {
            char *t = malloc(sizeof *t * 100);
            sprintf(t, "❌ llist remove Failed for original at idx %d, expected: %d, got: %d ", i, expected[i], actual[i]);
            fprintf(stderr, "%-35s", t);
            free(t);
            destroy_llist(list);
            free(list);
            return;
        }
    }
    fprintf(stdout, "%-35s", "✅ llist remove Passed ");
    destroy_llist(list);
    free(list);
}
void llist_test_sort(struct llist *list) {
    list = list->copy(list);
    list->clear(list);
    for (int i = 99; i >= 0; --i) {
        list->append(list, i);
    }
    list->sort(list);
    int *actual;

    list->toArray(list, &actual);

    struct llist *ref = construct_llist();
    for (int i = 0; i < 100; ++i) {
        ref->append(ref, i);
    }
    int *expected;
    ref->toArray(ref, &expected);

    for (int i = 0; i < list->length(list); ++i) {
        if (actual[i] != expected[i]) {
            char *t = malloc(sizeof *t * 100);
            sprintf(t, "❌ llist sort Failed for original at idx %d, expected: %d, got: %d ", i, expected[i], actual[i]);
            fprintf(stderr, "%-35s", t);
            free(t);
            destroy_llist(list);
            free(list);
            return;
        }
    }
    fprintf(stdout, "%-35s", "✅ llist sort Passed ");

    destroy_llist(list);
    free(list);
    destroy_llist(ref);
    free(ref);
}

int main(void) {
    struct rbtree *map = construct_rbtree();
    for (int i = 0; i < 50; ++i) {
        map->insert(map, i, i);
    }
    struct llist *list = construct_llist();
    for (int i = 0; i < 10; ++i) {
        list->append(list, i);
    }
    printf("--------RBTREE TESTS-------- \n");
    rbtree_test_copy(map);
    rbtree_test_print(map);
    rbtree_test_getKeys(map);
    printf("\n");
    rbtree_test_clear(map);
    rbtree_test_contains(map);
    rbtree_test_getValue(map);
    printf("\n");
    rbtree_test_tryGetValue(map);
    rbtree_test_setValue(map);
    rbtree_test_insert(map);
    printf("\n");
    rbtree_test_remove(map);
    rbtree_test_checkValid(map);
    printf("\n");
    printf("--------LLIST TESTS-------- \n");
    llist_test_copy(list);
    llist_test_toArray(list);
    llist_test_length(list);
    printf("\n");
    llist_test_print(list);
    llist_test_clear(list);
    llist_test_contains(list);
    printf("\n");
    llist_test_find(list);
    llist_test_get(list);
    llist_test_set(list);
    printf("\n");
    llist_test_append(list);
    llist_test_prepend(list);
    llist_test_insert(list);
    printf("\n");
    llist_test_remove(list);
    llist_test_sort(list);

    return 0;
}