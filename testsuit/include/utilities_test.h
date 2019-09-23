#include "utilities.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>


int init_utilities(void);
int clean_utilities(void); 

void testTrim_no_space(void);
void testTrim_leading_space(void);
void testTrim_newline(void);
void testTrim_double_space(void);
void testTrim_sentence(void);
