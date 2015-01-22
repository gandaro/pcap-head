include config.mk

SRC	= pcap-head.c
OBJ	= $(SRC:.c=.o)

all: pcap-head

.c.o:
	@echo CC $<
	@$(CC) -c $(CFLAGS) $<

pcap-head: $(OBJ)
	@echo CC -o $@
	@$(CC) -o $@ $(OBJ) ${LDFLAGS}

clean:
	@echo cleaning
	@rm -f $(OBJ) pcap-head


.PHONY: all clean
