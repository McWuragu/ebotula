bin_PROGRAMS = ebotula
ebotula_SOURCES= \
	type.h \
	irchelp.h \
	messages.h \
	account.c \
	account.h \
	command.c \
	command.h \
	dbaccess.c \
	dbaccess.h \
	extract.c \
	extract.h \
	handles.c \
	handles.h \
	ircbot.c \
	ircbot.h \
	irc.c \
	irc.h \
	network.c \
	network.h \
	parameter.c \
	parameter.h \
	parser.c \
	parser.h \
	timing.c \
	timing.h \
	utilities.c \
	utilities.h

ebotula_LDADD= $(GDBM_LIB) $(CRYPT_LIB) $(PTHREAD_LIB)

ebotula_CFLAGS= $(DFLAGS)
