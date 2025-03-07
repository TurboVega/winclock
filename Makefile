# ----------------------------
# Makefile Options
# ----------------------------

NAME = winclock
DESCRIPTION = "Agon Windows Clock Example"
COMPRESSED = NO
HAS_ARG_PROCESSING = 0
HAS_EXIT_HANDLER = 0

INIT_LOC = 047000
BSSHEAP_LOW = 04A000
BSSHEAP_HIGH = 04A3FF

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

#Unsure why the copy of this in makefile.mk doesn't cover this already...
ifeq ($(OS),Windows_NT)
COPYDIR ?= ( xcopy $1 $2 /S /Q /Y /I /K 1>nul 2>nul || call )
else
COPYDIR ?= cp -r $1 $2
endif

# Override the CRT0 for AgWin purposes
LDCRT0 ?= $(call NATIVEPATH,src/crt0.src)

# ----------------------------

include $(shell cedev-config --makefile)
