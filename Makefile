BOARD ?= org.proficnc.uwb_1.0

CSRC = $(shell find src -name "*.c") $(shell find uavcan/src -name "*.c")
INCDIR = ./include ./uavcan/include

include modules/omd_common/include.mk
