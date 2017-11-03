BOARD_DIR := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
BOARDSRC = $(BOARD_DIR)/board.c
BOARDINC = $(BOARD_DIR)
TGT_MCU = stm32f302x8
