
#include <stdio.h>
#include <string.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include "utilities.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#ifndef TESTSUIT_H_
#define TESTSUIT_H_
typedef struct {
	void (*TestFkt)(void);
	const char pDesc[128];
} strTestDesc_t;

#endif
