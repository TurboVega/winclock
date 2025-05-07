# ----------------------------
# Makefile Options
# ----------------------------

NAME = winclock
DESCRIPTION = "Agon Windows Clock Example"
COMPRESSED = NO
HAS_ARG_PROCESSING = 0
HAS_EXIT_HANDLER = 0

# The RAM_START specifies where the app is loaded.
# The RAM_SIZE specifies how much RAM the app will use.
# The end of the app RAM is equal to RAM_START+RAM_SIZE.
# Some weak variables are used in case their values are
# overridden from the outside by strong variables.

WINCLOCK_RAM_START ?= 0x058000
RAM_START = $(WINCLOCK_RAM_START)

WINCLOCK_RAM_SIZE ?= 0x008000
RAM_SIZE = $(WINCLOCK_RAM_SIZE)

LDHAS_ARG_PROCESSING = 0
LDHAS_EXIT_HANDLER = 0

include $(shell agondev-config --makefile)

CFLAGS += -Wall -Wextra -Oz -DAGWIN_APP -DRAM_START=$(RAM_START) -DRAM_SIZE=$(RAM_SIZE)
CXXFLAGS += -Wall -Wextra -Oz -DAGWIN_APP -DRAM_START=$(RAM_START) -DRAM_SIZE=$(RAM_SIZE)
