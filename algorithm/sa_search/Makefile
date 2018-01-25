#
#
#
# Optimization methods for the recovery of XOR-based erasure codes
#
# Author: lyfdamon
# Since: 2018/1/10
#
#

# subdir and objects in current dir
SRC_DIR = src
INC_DIR = include
OBJ_DIR = $(CURDIR)/obj

ifneq ($(OBJ_DIR),)
$(shell mkdir -p $(OBJ_DIR))
endif

_DEPS    = sa_search.cpp
DEPS     = $(patsubst %, $(SRC_DIR)/%, $(_DEPS))

_OBJ    = sa_search.o
OBJ     = $(patsubst %, $(OBJ_DIR)/%, $(_OBJ))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS)
	$(CC) -c $< $(CFLAGS) -o $@ -I$(INC_DIR)

all:$(OBJ)

.PHONY: all clean

clean:cleansubdirs
	rm -rf $(OBJ_DIR)

include $(MAKEINCLUDE)
