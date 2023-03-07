SRC_DIR:=./src
LIB_DIR:=./lib

.PHONY: all clean bin clean_bin

all: bin

bin: 
	$(MAKE) -C $(SRC_DIR)

clean_bin:
	$(MAKE) -C $(SRC_DIR) clean

clean: clean_bin