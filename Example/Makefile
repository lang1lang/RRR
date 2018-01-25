#
#
#
# Optimization methods for the recovery of XOR-based erasure codes
#
# Author: lyfdamon
# Since: 2018/1/19
#
#

# subdir and objects in current dir
SRC_DIR = .
OBJ_DIR = $(CURDIR)/obj

include                    = -I../jerasure/include -I../algorithm/climb_search/include -I../algorithm/sa_search/include -I../algorithm/enumeration_search/include -I../FileOutput/include
obj                        = -I../jerasure/obj -I../algorithm/climb_search/obj -I../algorithm/sa_search/obj -I../algorithm/enumeration_search/obj -I../FileOutput/obj

ifneq ($(OBJ_DIR),)
$(shell mkdir -p $(OBJ_DIR))
endif

_OBJ    = climb_test.o sa_test.o enumeration_test.o 
OBJ     = $(patsubst %, $(OBJ_DIR)/%, $(_OBJ))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -c $< $(CFLAGS)  -o $@ $(include) $(obj)

all:$(OBJ)
	
.PHONY: all clean

clean:cleansubdirs
	rm -rf $(OBJ_DIR)

include $(MAKEINCLUDE)
