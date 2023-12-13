bin_PROGRAMS = ebotula_db
ebotula_db_SOURCES=\
	delete.c \
	dump.c \
	exists.c \
	fetch.c \
	quote.c \
	store.c \
	main.c

noinst_HEADERS = \
	include/store.h \
	include/quote.h \
	include/fetch.h \
	include/main.h \
	include/dump.h \
	include/exists.h \
	include/delete.h 

ebotula_db_LDADD= $(GDBM_LIB)
ebotula_db_LDFLAGS=$(DFLAGS) $(PFLAGS)
ebotula_db_CFLAGS= $(DFLAGS) $(PFLAGS) -I./include


