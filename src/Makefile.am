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
	callbacklist.c \
	configparser.c \
	baseconfig.c 

noinst_HEADERS = config.h \
	include/type.h \
	include/cmdtypes.h \
	include/cmdlist.h \
	include/irchelp.h \
	include/messages.h \
	include/account.h \
	include/command.h \
	include/ctcp.h \
	include/handles.h \
	include/dbaccess.h \
	include/extract.h \
	include/ircbot.h \
	include/irc.h \
	include/network.h \
	include/parameter.h \
	include/parser.h \
	include/timing.h \
	include/utilities.h \
	include/queue.h \
	include/callback.h\
	include/callbacklist.h \
	include/baseconfig.h \
	include/configparser.h


ebotula_LDADD= $(GDBM_LIB) $(CRYPT_LIB) $(PTHREAD_LIB) $(RT_LIB)
ebotula_LDFLAGS=$(DFLAGS) $(PFLAGS)
ebotula_CFLAGS= $(DFLAGS) $(PFLAGS) -I./include

