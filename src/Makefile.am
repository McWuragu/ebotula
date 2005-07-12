bin_PROGRAMS = ebotula
ebotula_SOURCES= \
	messages.c \
	account.c \
	cmdlist.c \
	command.c \
	ctcp.c \
	handles.c \
	dbaccess.c \
	extract.c \
	irchelp.c \
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
	cmdtypes.h \
	cmdlist.h \
	irchelp.h \
	messages.h \
	account.h \
	command.h \
	ctcp.h \
	handles.h \
	dbaccess.h \
	extract.h \
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
	baseconfig.h \
	configparser.c \
	configparser.h


ebotula_LDADD= $(GDBM_LIB) $(CRYPT_LIB) $(PTHREAD_LIB) $(RT_LIB)
ebotula_LDFLAGS=$(DFLAGS) $(PFLAGS)
ebotula_CFLAGS= $(DFLAGS) $(PFLAGS)
