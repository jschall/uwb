BOARD_DIR := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
BOARDSRC = $(BOARD_DIR)/board.c
BOARDINC = $(BOARD_DIR)
LDSCRIPT = $(OMD_COMMON_DIR)/configs/stm32f302x8/bl.ld
