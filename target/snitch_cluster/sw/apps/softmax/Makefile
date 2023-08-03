
# Usage of absolute paths is required to externally include this Makefile
MK_DIR   := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
DATA_DIR := $(realpath $(MK_DIR)/data)
SRC_DIR  := $(realpath $(MK_DIR)/src)

LENGTH ?= 16

APP     ?= softmax
SRCS    ?= $(realpath $(SRC_DIR)/softmax.c)
INCDIRS ?= $(DATA_DIR) $(SRC_DIR)

$(DATA_DIR)/data.h: $(DATA_DIR)/datagen.py
	$< $(LENGTH) > $@

.PHONY: clean-data clean

include ../common.mk

clean-data:
	rm -f $(DATA_DIR)/data.h

clean: clean-data