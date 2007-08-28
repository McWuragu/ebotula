bin_PROGRAMS = ebotula_db
ebotula_db_SOURCES=\
	delete.c \
	delete.h \
	dump.c \
	dump.h \
	exists.c \
	exists.h \
	fetch.c \
	fetch.h \
	main.h \
	quote.c \
	quote.h \
	store.c \
	store.h \
	main.c

ebotula_db_LDADD= $(GDBM_LIB)
ebotula_db_LDFLAGS=$(DFLAGS) $(PFLAGS)
ebotula_db_CFLAGS= $(DFLAGS) $(PFLAGS)


