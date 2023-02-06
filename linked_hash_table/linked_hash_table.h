#pragma once
#include "../llist/llist.h"
#include "../IMap.h"
#include <string.h>

struct IMap * construct_LinkedHashTable();
void destroy_LinkedHashTable(struct IMap * this);