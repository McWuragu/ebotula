#########################################################################

SRC_DIR=./
#MAKEFLAGS+= -j2
CC=gcc
CFLAGS=	-Wall 
LDFLAGS= -lgdbm

TARGET=gdbmview

#########################################################################
.SUFFIXES: .C .c .o .d

vpath %.c $(SRC_DIR)	

OBJS=   gdbmview.o 

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

