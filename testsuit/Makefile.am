bin_PROGRAMS = testsuit
check_PROGRAMS = testsuit
TESTS = testsuit
AUTOMAKE_OPTIONS = subdir-objects

# testsuit/Makefile.am
AM_CPPFLAGS = -I$(top_srcdir)/src/include

testsuit_SOURCES = \
	testsuit.c \
	utilities_test.c \
	queue_test.c \
	../src/utilities.c \
	../src/queue.c \
	../src/extract.c
	 

noinst_HEADERS =  \
	include/testsuit.h \
	include/utilities_test.h \
	include/queue_test.h \
	../src/include/utilities.h \
	../src/include/queue.h \
	../src/include/extract.h
	

testsuit_LDADD = $(GDBM_LIB) $(CRYPT_LIB) $(PTHREAD_LIB) $(RT_LIB) $(CUNIT_LIB)
testsuit_LDFLAGS =$(DFLAGS) $(PFLAGS)
testsuit_CFLAGS = $(DFLAGS) $(PFLAGS) -I$(top_srcdir)/testsuit/include -I$(top_srcdir)/src/include


