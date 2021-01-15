# SPDX-License-Identifier: GPL-2.0
#
# Do "make help" for targets and options.

CFLAGS += -g -O2 -Wall -fPIC -Iinclude -D_GNU_SOURCE

LIBS += -lm

SOFLAGS += -shared

ifeq "$(S)" "1"
S_CFLAGS += -fsanitize=address -fsanitize=leak -fsanitize=undefined	\
	  -fsanitize-address-use-after-scope -fstack-check
endif

DEP_CFLAGS += $(CFLAGS) $(BASIC_CFLAGS)
ALL_CFLAGS += $(DEP_CFLAGS) $(HAVE_CFLAGS) $(S_CFLAGS)

EXAMPLE :=								\
	example/ALERT.PRG						\
	example/HELLO.TOS						\
	example/WINDOW.PRG						\
	example/XBRA.PRG

TOSLIBC := toslibc.a

.PHONY: all
all: $(TOSLIBC) $(EXAMPLE)

include check/Makefile
include lib/Makefile
include example/Makefile

.PHONY: version
version:
	@script/version

.PHONY: clean
clean:
	$(QUIET_RM)$(RM) -f *.a */*.o* */*/*.o* lib/toslibc/version.c	\
		check/68000.actual					\
		*/*.PRG */*.TOS */*.TTP					\
		GPATH GRTAGS GTAGS

.PHONY: gtags
gtags:
	$(QUIET_GEN)gtags

.PHONY: help
help:
	@echo "Targets:"
	@echo "  all            - compile TOS/libc (default)"
	@echo "  version        - display TOS/libc version"
	@echo "  clean          - remove generated files"
	@echo
	@echo "Options:"
	@echo "  V              - set to 1 to compile verbosely"
	@echo "  S              - set to 1 for sanitation checks"
	@echo "  CROSS_COMPILE  - set m68k cross compiler to use for Atari ST code"
	@echo
	@echo "Example:"
	@echo "  make CROSS_COMPILE=m68k-elf-"

V             = @
Q             = $(V:1=)
QUIET_AR      = $(Q:@=@echo    '  AR      '$@;)
QUIET_AS      = $(Q:@=@echo    '  AS      '$@;)
QUIET_CC      = $(Q:@=@echo    '  CC      '$@;)
QUIET_GEN     = $(Q:@=@echo    '  GEN     '$@;)
QUIET_LINK    = $(Q:@=@echo    '  LD      '$@;)
QUIET_RM      = $(Q:@=@echo    '  RM      '$@;)
QUIET_CHECK   = $(Q:@=@echo    '  CHECK   '$@;)
QUIET_TEST    = $(Q:@=@echo    '  TEST    '$@;)

BASIC_CFLAGS += -Wp,-MD,$(@D)/$(@F).d -MT $(@D)/$(@F)

ifneq "$(MAKECMDGOALS)" "clean"
    DEP_FILES := $(shell find . -name '*'.d -printf '%P\n' | sort)
    $(if $(DEP_FILES),$(eval include $(DEP_FILES)))
endif
