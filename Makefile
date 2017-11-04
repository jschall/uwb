BOARD ?= org.proficnc.uwb_1.0

CSRC = $(shell find src -name "*.c") $(shell find uavcan/src -name "*.c")
INCDIR = ./include ./uavcan/include

include omd_common/include.mk

.PHONY: update_submodules
PRE_BUILD_RULE: update_submodules
update_submodules:
	git submodule init && git submodule update
