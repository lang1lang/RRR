#
#
#
# Optimization methods for the recovery of XOR-based erasure codes
#
# Author: lyfdamon
# Since: 2017/12/26
#
#

# TOP_PATH
TOP_PATH = $(shell pwd)

# SUB_DIR
JERASURE_DIR  = $(TOP_PATH)/jerasure
ALGORITHM_DIR = $(TOP_PATH)/algorithm
EXAMPLE_DIR   = $(TOP_PATH)/Example
FILEOUTPUT    = $(TOP_PATH)/FileOutput

BIN_DIR       = $(TOP_PATH)/bin
ifneq ($(BIN_DIR),)
$(shell mkdir -p $(BIN_DIR))
endif

RESULT_DIR    = $(TOP_PATH)/Result
ifneq ($(RESULT_DIR),)
$(shell mkdir -p $(RESULT_DIR))
$(shell mkdir -p $(RESULT_DIR)/Hill-Climbing)
$(shell mkdir -p $(RESULT_DIR)/Simulated-Annealing)
$(shell mkdir -p $(RESULT_DIR)/Enumeration)
endif

SUBDIRS     =  $(JERASURE_DIR) $(ALGORITHM_DIR) $(EXAMPLE_DIR) $(FILEOUTPUT)
AFTERDIRS   =  $(EXAMPLE_DIR)
OBJECTS     =

#
# COMPILATION RULES
#
default: all

all:subdirs $(OBJECTS)
	${CC} $(EXAMPLE_DIR)/obj/climb_test.o $(JERASURE_DIR)/obj/*.o $(ALGORITHM_DIR)/climb_search/obj/*.o $(FILEOUTPUT)/obj/*.o $(LDFLAGS) -o ${BIN_DIR}/climb_test
	${CC} $(EXAMPLE_DIR)/obj/sa_test.o $(JERASURE_DIR)/obj/*.o $(ALGORITHM_DIR)/sa_search/obj/*.o $(FILEOUTPUT)/obj/*.o $(LDFLAGS) -o ${BIN_DIR}/sa_test
	${CC} $(EXAMPLE_DIR)/obj/enumeration_test.o $(JERASURE_DIR)/obj/*.o $(ALGORITHM_DIR)/enumeration_search/obj/*.o $(FILEOUTPUT)/obj/*.o $(LDFLAGS) -o ${BIN_DIR}/enumeration_test
# Help message
help:
	@echo "optimization method for XOR-code"
	@echo 
	@echo "Target rules"
	@echo "    all   - Compiles and generates binary files"
	@echo "    clean - Clean the project by removing binaries"
	@echo "    help  - Prints a help message with target rules"

#Rule for clean the project
clean:cleansubdirs
	rm -f ${TARGET} ${OBJECTS}

# path of "make build scripts"
# NOTE, use absolute path, export once, use in all subdirs
export TOP_PATH
export MAKEINCLUDE = $(TOP_PATH)/Makefile.build 

# Include Makefile.build
include $(MAKEINCLUDE)
