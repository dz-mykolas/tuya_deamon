BIN:=test
SRC:=$(wildcard *.c)
OBJ:=$(SRC:.c=.o)
CPPFLAGS:=-Iutils -Iinterface -Iinclude
LDFLAGS:=-L./lib \
-Wl,-rpath,./lib
LDLIBS:=-llink_core \
-lmiddleware_implementation \
-lplatform_port \
-lutils_modules

.PHONY: all clean $(BIN)

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CPPFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)
clean:
	$(RM) $(OBJ) $(BIN) example.txt