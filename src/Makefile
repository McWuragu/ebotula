#########################################################################
VERSION=0.05-alpha
DATABASEPATH=./
CONFDIR=./

SRC_DIR=./
MAKEFLAGS+= -j2
CC=gcc
CFLAGS=	-Wall -g -D_DEBUG -DVERSION=\"$(VERSION)\" \
	-DDATABASEPATH=\"$(DATABASEPATH)\" \
	-DCONFDIR=\"$(CONFDIR)\" 
LDFLAGS= -lpthread -lgdbm -lcrypt

TARGET=ebotula

#########################################################################
.SUFFIXES: .C .c .o .d

vpath %.c $(SRC_DIR)	

OBJS=   parameter.o \
	network.o \
	irc.o \
	command.o \
	dbaccess.o \
	utilities.o \
	extract.o \
	ircbot.o 

DEPS= $(patsubst %.o,%.d,$(OBJS))
#########################################################################


%.o: %.c
	$(CC) -c  -pipe $(CFLAGS) $< -o $@

%.d: %.c
	@ echo dep $<
	@$(CC) -MM $(CFLAGS) $< > $@


$(TARGET) : $(DEPS) $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(LDFLAGS) -o $(TARGET)

.PHONY : clean
clean :
	rm -f $(TARGET) $(OBJS) $(DEPS) *~ core

explain:
	@echo "The following information represents your program:"
	@echo "Final executable name: $(TARGET)"
	@echo "Object files:     $(OBJS)"
	@echo "Dependency files:   $(DEPS)"

