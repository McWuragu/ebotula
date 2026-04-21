check_PROGRAMS = testsuit
TESTS = testsuit
AUTOMAKE_OPTIONS = subdir-objects

# testsuit/Makefile.am
AM_CPPFLAGS = -I$(top_srcdir)/src/include

testsuit_SOURCES = \
	testsuit.c \
	utilities_test.c \
	queue_test.c \
	extract_test.c \
	callbacklist_test.c \
	configparser_test.c \
	parser_pipeline_test.c \
	../src/utilities.c \
	../src/queue.c \
	../src/extract.c \
	../src/callbacklist.c \
	../src/configparser.c \
	../src/parser.c \
	../src/cmdlist.c
	 

noinst_HEADERS =  \
	include/testsuit.h \
	include/utilities_test.h \
	include/queue_test.h \
	include/extract_test.h \
	include/callbacklist_test.h \
	include/configparser_test.h \
	include/parser_pipeline_test.h \
	../src/include/utilities.h \
	../src/include/queue.h \
	../src/include/extract.h \
	../src/include/callbacklist.h \
	../src/include/callback.h \
	../src/include/configparser.h \
	../src/include/parser.h \
	../src/include/cmdlist.h \
	../src/include/cmdtypes.h \
	../src/include/command.h \
	../src/include/handles.h \
	../src/include/ctcp.h
	

testsuit_LDADD = $(GDBM_LIB) $(CRYPT_LIB) $(PTHREAD_LIB) $(RT_LIB) $(CUNIT_LIB)
testsuit_LDFLAGS =$(DFLAGS) $(PFLAGS)
testsuit_CFLAGS = $(DFLAGS) $(PFLAGS) -I$(top_srcdir)/testsuit/include -I$(top_srcdir)/src/include
