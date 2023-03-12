BIN:=tuya_daemon
SRC:=$(wildcard *.c)
OBJ:=$(SRC:.c=.o)
CPPFLAGS:=-I../utils -I../interface -I../include
LDFLAGS:=-L../lib \
-Wl,-rpath,../lib,--no-as-needed
LDLIBS:=$(wildcard ../lib/*.so)

.PHONY: all clean $(BIN)

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CPPFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)
clean:
	$(RM) $(OBJ) $(BIN) example.txt