check_PROGRAMS = testsuit
testsuit_SOURCES = \
	testsuit.c \
	utilities_test.c
	           
include_HEADERS = 
	\testsuit.h 
	\utilities_test.h

testsuit_LDADD = $(CUNIT_LIB)
testsuit_CFLAGS = -I ./include/ -I ../src
