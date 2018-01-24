#
#
#
# Optimization methods for the recovery of XOR-based erasure codes
#
# Author: lyfdamon
# Since:  2017/12/26
#
#

# subdir and objects in curent dir
SRC_DIR = src
INC_DIR = include
OBJ_DIR = $(CURDIR)/obj

ifneq ($(OBJ_DIR),)
$(shell mkdir -p $(OBJ_DIR))
endif

_DEPS   = cauchy.cpp galois.cpp jerasure.cpp liberation.cpp reed_sol.cpp
DEPS    = $(patsubst %, $(SRC_DIR)/%, $(_DEPS))

_OBJ    = cauchy.o galois.o jerasure.o liberation.o reed_sol.o
OBJ     = $(patsubst %, $(OBJ_DIR)/%, $(_OBJ))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS)
	$(CC) -c $< $(CFLAGS) -o $@ -I$(INC_DIR)

all:$(OBJ)
	
.PHONY: all clean
	
clean:cleansubdirs
	rm -f $(OBJ)

include $(MAKEINCLUDE)
