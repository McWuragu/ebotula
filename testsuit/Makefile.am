check_PROGRAMS = testsuit


AUTOMAKE_OPTIONS = subdir-objects


testsuit_SOURCES = \
	testsuit.c \
	utilities_test.c \
	../src/utilities.c \
	../src/extract.c 

noinst_HEADERS =  \
	include/testsuit.h \
	include/utilities_test.h \
	../src/include/utilities.h \
	../src/include/extract.h
	
testsuit_LDADD = $(CUNIT_LIB)
testsuit_CFLAGS = -I../src/include -I./include

