check_PROGRAMS = testsuit
testsuit_SOURCES = \
	testsuit.c \
	utilities_test.c \
	../src/utilities.c \
	../src/extract.c 
	           
include_HEADERS = 
	testsuit.h \
	utilities_test.h \
	utitities.h \
	extract.h
	

testsuit_LDADD = $(CUNIT_LIB)
testsuit_CFLAGS = -I ./include
