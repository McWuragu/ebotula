SUBDIRS=lang
bin_PROGRAMS = ebotula
ebotula_SOURCES= \
	lang/messages_de.h \
	lang/irchelp_de.h \
	lang/messages_eng.h \
	lang/irchelp_eng.h \
	messages.c \
	account.c \
	command.c \
	dbaccess.c \
	extract.c \
	handles.c \
	ircbot.c \
	irc.c \
	network.c \
	parameter.c \
	parser.c \
	timing.c \
	utilities.c \
	queue.c \
	callback.c \
	callbacklist.c\
	type.h \
	irchelp.h \
	messages.h \
	account.h \
	command.h \
	dbaccess.h \
	extract.h \
	handles.h \
	ircbot.h \
	irc.h \
	network.h \
	parameter.h \
	parser.h \
	timing.h \
	utilities.h \
	queue.h \
	callback.h\
	callbacklist.h \
	baseconfig.c \
	baseconfig.h 


ebotula_LDADD= $(GDBM_LIB) $(CRYPT_LIB) $(PTHREAD_LIB) $(RT_LIB)
ebotula_LDFLAGS=$(DFLAGS) $(PFLAGS)
ebotula_CFLAGS= $(DFLAGS) $(PFLAGS)
