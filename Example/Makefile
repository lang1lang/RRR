#
#
#
# Optimization methods for the recovery of XOR-based erasure codes
#
# Author: lyfdamon
# Since: 2018/1/2
#
#

# subdir and objects in current dir
SRC_DIR = .
OBJ_DIR = $(CURDIR)/obj

jerasure_include           = ../jerasure/include
climb_search_include       = ../algorithm/climb_search/include
sa_search_include          = ../algorithm/sa_search/include
enumeration_search_include = ../algorithm/enumeration_search/include
fileoutput_include         = ../FileOutput/include
jerasure_obj               = ../jerasure/obj
climb_search_obj           = ../algorithm/climb_search/obj
sa_search_obj              = ../algorithm/sa_search/obj
enumeration_search_obj     = ../algorithm/enumeration_search/obj
fileoutput_obj             = ../FileOutput/obj


ifneq ($(OBJ_DIR),)
$(shell mkdir -p $(OBJ_DIR))
endif

_OBJ    = climb_test.o sa_test.o enumeration_test.o 
OBJ     = $(patsubst %, $(OBJ_DIR)/%, $(_OBJ))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -c $< $(CFLAGS)  -o $@ -I$(jerasure_include) -I$(jerasure_obj) -I$(climb_search_include) -I$(climb_search_obj) -I$(sa_search_include) -I$(sa_search_obj) -I$(enumeration_search_include) -I$(enumeration_search_obj) -I$(fileoutput_include) -I$(fileoutput_obj)

all:$(OBJ)
	
.PHONY:clean

clean:cleansubdirs
	rm -f $(OBJECTS)

include $(MAKEINCLUDE)
