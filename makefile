NAME ?= SWEEPER
DESCRIPTION ?= "Minesweeper"
COMPRESSED ?= YES
ARCHIVED ?= YES

ifndef CEDEV
$(error CEDEV environment path variable is not set)
endif

include $(CEDEV)/meta/makefile.mk