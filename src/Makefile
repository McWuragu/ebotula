
SRC_DIR=.
INCLUDS=-I $(SRC_DIR)
OPTS=-Wall -g $(INCLUDS) -D_DEBUG
LIBS=-lpthread -lgdbm -lcrypt
TARGET=ebotula
MAKEFLAGS+= -j2

#########################################################################
.SUFFIXES: .C .c .o .d

vpath %.c $(SRC_DIR)	

OBJS=   parameter.o \
	network.o \
	irc.o \
	command.o \
	dbaccess.o \
	ircbot.o 

%.o: %.c
	@echo Gcc $<
	@gcc -c  -pipe $(OPTS) $< -o $@

%.d: %.c
	@echo DEP $<
	@gcc -MM $(OPTS) $< | tee $@.tmp > $@
	@sed <$@.tmp s/\\.o/.d/ >> $@
	@rm $@.tmp

$(TARGET) : $(OBJS) $(DEPS)
	gcc $(OBJS) $(OPTS) $(LIBS) -o $(TARGET)

.PHONY : clean
clean :
	rm -f $(TARGET) $(OBJS) $(DEPS) *~ core

explain:
	@echo "The following information represents your program:"
	@echo "Final executable name: $(TARGET)"
	@echo "Object files:     $(OBJS)"
	@echo "Dependency files:   $(DEPS)"

