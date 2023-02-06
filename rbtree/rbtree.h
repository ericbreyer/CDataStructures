#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../IMap.h"
#pragma once
#define STRICT_CHECK
#ifndef STRICT_CHECK
#define assert void
#endif

struct IMap *construct_rbtree();